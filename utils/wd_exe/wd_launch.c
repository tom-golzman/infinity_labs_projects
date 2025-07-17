/**
	Written By: Tom Golzman
	Date: 09/07/2025
	Reviewed By: 
**/

/*********************************** includes ***********************************/
#define _POSIX_C_SOURCE 200112L
#include <signal.h>	/* sigaction, sig_atomic_t */
#include <unistd.h>	/* getppid, execv, kill */
#include <assert.h>	/* assert */
#include <time.h>	/* time */
#include <string.h>	/* memset */

#include "utils.h"	/* ExitIfBad, Log */

#include "scheduler.h"

/*********************************** defines ***********************************/
enum { MAX_MISSES_IDX = 1, INTERVAL_IDX = 2, PPID_IDX = 3, CLIENT_PATH_IDX = 4, MIN_ARGC = 5 };

typedef struct watchdog
{
	int argc;
	char** argv;
	int max_misses;
	unsigned long interval;
	sched_t* scheduler;
	const char* client_exec_path;
	pid_t other_process_pid; /* client pid */
} wd_t;

static volatile sig_atomic_t g_counter = 1;

/******************************* private functions *****************************/
static void InitStruct(wd_t* wd, int argc, char* argv[]);
static void InitSchedulerWD(wd_t* wd);
static int SendSignalTaskWD(void* arg);
static int CheckCounterTaskWD(void* arg);
static void ReviveClient(void* arg);
static void SetSignalHandlers();
static void HandleSIGUSR1(int sig);

/************************************ main ************************************/
int main(int argc, char* argv[])
{
	/* create wd_t struct */
	wd_t wd;
	
	/* assert */
	assert(argc >= MIN_ARGC);
	
	/* set signal handlers */
	SetSignalHandlers();
	
	/* initialize struct fields */
	InitStruct(&wd, argc, argv);
		
	/* init wd scheduler */
	InitSchedulerWD(&wd);
	
	/* run scheduler */
	return SchedRun(wd.scheduler);
}

/************************************ Functions ************************************/
static void InitStruct(wd_t* wd, int argc, char* argv[])
{
	int max_misses = atoi(argv[MAX_MISSES_IDX]);
	unsigned long interval = (unsigned long)atoi(argv[INTERVAL_IDX]);
	pid_t other_process_pid = getppid();
	
	if (1 == other_process_pid)
	{
		
		execv(argv[CLIENT_PATH_IDX], argv);
	}
	
	ExitIfBad(0 != max_misses, FAIL, "wd_launch.c-> InitStruct(): atoi(max_misses) FAILED!\n");
	ExitIfBad(0 != interval, FAIL, "wd_launch.c-> InitStruct(): atoi(interval) FAILED!\n");
	ExitIfBad(0 != other_process_pid, FAIL, "wd_launch.c-> InitStruct(): atoi(other_process_pid) FAILED!\n");
		
	/* assert */
	assert(NULL != wd);
	assert(argc >= MIN_ARGC);
		
	/* initialize struct fields */
	wd->argc = argc - 4;
	wd->argv = &argv[4];
	wd->max_misses = max_misses;
	wd->interval = interval;
	wd->client_exec_path = argv[CLIENT_PATH_IDX];
	wd->other_process_pid = other_process_pid;
	
	wd->scheduler = SchedCreate();
	ExitIfBad(NULL != wd->scheduler, FAIL, "wd_launch.c-> InitStruct(): SchedCreate() FAILED!\n");
}

static void InitSchedulerWD(wd_t* wd)
{
	ilrd_uid_t task_uid = invalid_uid;
	time_t now = -1;
	
	/* assert */
	assert(NULL != wd);
	
	now = time(NULL);
	ExitIfBad(-1 != now, FAIL, "wd_launch.c-> InitSchedulerWD(): time() FAILED!\n");
	
	/* add task - send signal */
	task_uid = SchedAddTask(wd->scheduler, now + wd->interval, SendSignalTaskWD, (void*)wd, NULL, NULL, wd->interval);
	ExitIfBad(!UIDIsSame(task_uid, invalid_uid), FAIL, "wd_launch.c-> InitSchedulerWD(): AddTask(SendSignal) FAILED!\n");
	
	/* add task - check counter */
	task_uid = SchedAddTask(wd->scheduler, now + wd->interval, CheckCounterTaskWD, (void*)wd, NULL, NULL, wd->interval);
	ExitIfBad(!UIDIsSame(task_uid, invalid_uid), FAIL, "wd_launch.c-> InitSchedulerWD(): AddTask(CheckCounter) FAILED!\n");
}

static int SendSignalTaskWD(void* arg)
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);
	int status = -1;
	
	/* send signal to client */
	status = kill(wd->other_process_pid, SIGUSR1);
	LogIfBad(0 == status, "wd_launch.c-> SendSignalTaskWD(): kill() FAILED!\n");

	Log("WD sent signal");
	
	/* return TO_RESCHEDULE */
	return TO_RESCHEDULE;
}

static int CheckCounterTaskWD(void* arg)
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);
	
	/* increse g_counter */
	++g_counter;

	/* if g_counter is above max_misses */
	if (g_counter > wd->max_misses)
	{
		/* write to log */
		Log("wd_launch.c-> CheckCounterTaskWD(): Client didn't respond - exiting!\n");
		
		/* stop scheduler and cleanup  */
		SchedStop(wd->scheduler);
		
		/* call revive client */
		ReviveClient(wd);
		
		/* return NOT_RESCHEDULE */
		return NOT_RESCHEDULE;
	}
	
	/* return TO_RESCHEDULE */
	return TO_RESCHEDULE;
}

static void ReviveClient(void* arg)
{
	int status = -1;
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);

	/* kill client process */
	status = kill(wd->other_process_pid, SIGTERM);
	/*ExitIfBad(0 == status, FAIL, "wd_launch.c-> ReviveClient(): kill() FAILED!\n");
	 TODO: check errno and send SIGSTOP if failed */
	
	/* execv() */
	execvp(wd->client_exec_path, (wd->argv));
	
	/* if execv returns - failed */
	/* log */
	Log("wd_launch.c-> ReviveClient(): execv() FAILED!\n");
}

/************************************ Signal Handler ************************************/
static void SetSignalHandlers()
{
	struct sigaction sa;
	int status = 0;
	
	memset(&sa, 0, sizeof(sa));

	/* set handlers with sigaction */
	sa.sa_handler = HandleSIGUSR1;

	status = sigaction(SIGUSR1, &sa, NULL);
	ExitIfBad(0 == status, FAIL, "wd_launch.c-> SetSignalHandlers(): sigaction() FAILED!\n");
}

static void HandleSIGUSR1(int sig)
{
	/* assert */
	assert(sig == SIGUSR1);
	
	/* reset g_counter */
	g_counter = 0;
}
