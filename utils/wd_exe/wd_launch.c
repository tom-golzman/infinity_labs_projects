/**
	Written By: Tom Golzman
	Date: 09/07/2025
	Reviewed By: Amir Granot
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
enum
{
	MAX_MISSES_IDX = 1,
	INTERVAL_IDX = 2,
	PPID_IDX = 3,
	CLIENT_PATH_IDX = 4,
	MIN_ARGC = 5
};

typedef struct watchdog
{
	char** argv;
	int max_misses;
	unsigned long interval;
	sched_t* scheduler;
	const char* client_exec_path;
	pid_t other_process_pid; /* client pid */
} wd_t;

static volatile sig_atomic_t g_counter = 1;

/******************************* private functions *****************************/
static void InitStruct(wd_t* wd, char* argv[]);
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
	wd_t wd = { 0 };
	
	/* assert */
	assert(argc >= MIN_ARGC);
	
	/* set signal handlers */
	SetSignalHandlers();
	
	/* initialize struct fields */
	InitStruct(&wd, argv);
		
	/* init wd scheduler */
	InitSchedulerWD(&wd);
	
	/* run scheduler */
	return SchedRun(wd.scheduler);
}

/************************************ Functions ************************************/
static void InitStruct(wd_t* wd, char* argv[])
{
	int max_misses = atoi(argv[MAX_MISSES_IDX]);
	int interval = atoi(argv[INTERVAL_IDX]);
	pid_t other_process_pid = getppid();
	
	if (1 == other_process_pid)
	{
		execv(argv[CLIENT_PATH_IDX], argv);
		
		/* if execv returns - failed */
		DEBUG_ONLY(Log("wd_launch.c-> InitStruct(): execv() FAILED!\n"););
		
		abort();
	}
	
	ExitIfBad(max_misses > 0, FAIL, "wd_launch.c-> InitStruct(): atoi(max_misses) FAILED!\n");
	ExitIfBad(interval > 0, FAIL, "wd_launch.c-> InitStruct(): atoi(interval) FAILED!\n");
		
	/* assert */
	assert(NULL != wd);
	
	wd->scheduler = SchedCreate();
	ExitIfBad(NULL != wd->scheduler, FAIL, "wd_launch.c-> InitStruct(): SchedCreate() FAILED!\n");
		
	/* initialize struct fields */
	wd->argv = &argv[4];
	wd->max_misses = max_misses;
	wd->interval = (unsigned long)interval;
	wd->client_exec_path = argv[CLIENT_PATH_IDX];
	wd->other_process_pid = other_process_pid;
}

static void InitSchedulerWD(wd_t* wd)
{
	ilrd_uid_t task_uid = invalid_uid;
	time_t now = -1;
	
	/* assert */
	assert(NULL != wd);
	
	now = time(NULL);
	
	/* add task - send signal */
	task_uid = SchedAddTask(wd->scheduler, now, SendSignalTaskWD, wd, NULL, NULL, wd->interval);
	ExitIfBad(!UIDIsSame(task_uid, invalid_uid), FAIL, "wd_launch.c-> InitSchedulerWD(): AddTask(SendSignal) FAILED!\n");
	
	/* add task - check counter */
	task_uid = SchedAddTask(wd->scheduler, now, CheckCounterTaskWD, wd, NULL, NULL, wd->interval);
	ExitIfBad(!UIDIsSame(task_uid, invalid_uid), FAIL, "wd_launch.c-> InitSchedulerWD(): AddTask(CheckCounter) FAILED!\n");
}

static int SendSignalTaskWD(void* arg)
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);
	int status = -1;
	
	/* send signal to client */
	status = kill(wd->other_process_pid, SIGUSR1);
	LogIfBad(0 == status, "wd_launch.c-> SendSignalTaskWD(): kill() FAILED!\n");

	DEBUG_ONLY(Log("WD sent signal to %d (pid)\n", wd->other_process_pid););
	
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
		DEBUG_ONLY(Log("wd_launch.c-> CheckCounterTaskWD(): Client didn't respond - exiting!\n"););
		
		/* stop scheduler */
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
	LogIfBad(0 == status, "wd_launch.c-> ReviveClient(): kill() FAILED!\n");
	
	/* execv() */
	execv(wd->client_exec_path, (wd->argv));
	
	/* if execv returns - failed */
	DEBUG_ONLY(Log("wd_launch.c-> ReviveClient(): execv() FAILED!\n"););
	
	abort();
}

/************************************ Signal Handler ************************************/
static void SetSignalHandlers()
{
	struct sigaction sa = { 0 };
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
