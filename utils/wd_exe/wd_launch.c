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
enum { MAX_MISSES = 5, INTERVAL = 1, MAX_REVIVES = 3 };

typedef struct watchdog
{
	int argc;
	char** argv;
	int max_misses;
	unsigned long interval;
	int revive_counter;
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
	assert(argc >= 5);
	
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
	int max_misses = atoi(argv[1]);
	unsigned long interval = (unsigned long)atoi(argv[2]);
	
	ExitIfBad(0 != max_misses, FAIL, "wd_launch.c-> InitStruct(): atoi(argv[1]) FAILED!\n");
	ExitIfBad(0 != interval, FAIL, "wd_launch.c-> InitStruct(): atoi(argv[2]) FAILED!\n");
	
	/* assert */
	assert(NULL != wd);
	assert(argc >= 5);
		
	/* initialize struct fields */
	wd->argc = argc - 5;
	wd->argv = &argv[5];
	wd->max_misses = max_misses;
	wd->interval = interval;
	wd->revive_counter = 0;
	wd->client_exec_path = argv[4];
	
	wd->other_process_pid = getppid();
	ExitIfBad(wd->other_process_pid != 1, FAIL, "wd_launch.c-> InitStruct(): ppid = 1\n");
	
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
	int status = FAIL;
	
	/* send signal to client */
	status = kill(wd->other_process_pid, SIGUSR1);
	LogIfBad(0 == status, "wd_launch.c-> SendSignalTaskWD(): kill() FAILED!\n");
	
	/* return TO_RESCHEDULE */
	return TO_RESCHEDULE;
}

static int CheckCounterTaskWD(void* arg) /* wd_t* */
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);
	
	/* if g_counter is zero */
	if (0 == g_counter)
	{
		/* reset revive counter */
		wd->revive_counter = 0;
		
		/* return TO_RESCHEDULE */
		return TO_RESCHEDULE;
	}
	
	/* increse g_counter */
	++g_counter;
	
	/* if g_counter is above max_misses */
	if (g_counter > wd->max_misses)
	{
		/* if revive counter is equal to MAX_REVIVES */
		if (wd->revive_counter >= MAX_REVIVES)
		{
			/* write to log */
			Log("wd_launch.c-> CheckCounterTaskWD(): Client didn't respond - exiting\n");
			
			/* stop scheduler and cleanup  */
			SchedStop(wd->scheduler);
			
			/* return NOT_RESCHEDULE */
			return NOT_RESCHEDULE;
		}
					
		/* increase revive counter in the struct */
		++wd->revive_counter;
		
		/* call revive client */
		ReviveClient(wd);
	}
	
	/* return TO_RESCHEDULE */
	return TO_RESCHEDULE;
}

static void ReviveClient(void* arg) /* wd_t* */
{
	int status = -1;
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);

	/* kill client process */
	status = kill(wd->other_process_pid, SIGTERM);
	ExitIfBad(0 == status, FAIL, "wd_launch.c-> ReviveClient(): kill() FAILED!\n");
	/* TODO: check errno and send SIGSTOP if failed */
	
	/* execv() */
	execv(wd->client_exec_path, (wd->argv));
	
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
