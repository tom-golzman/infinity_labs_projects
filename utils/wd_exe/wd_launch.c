/**
	Written By: Tom Golzman
	Date: 09/07/2025
	Reviewed By: 
**/
#define _POSIX_C_SOURCE 200112L
/*********************************** includes ***********************************/
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
	const char** argv;
	int max_misses;
	unsigned long interval;
	int revive_counter;
	sched_t* scheduler;
	const char* wd_exec_path;
	pid_t other_process_pid;
} wd_t;

static volatile sig_atomic_t g_counter = 1;

/******************************* private functions *****************************/
static int InitSchedulerWD(wd_t* wd);
static int SendSignalWD(void* arg);
static int CheckCounterWD(void* arg);
static int ReviveClient(void* arg);
static int SetSignalHandlers();
static void HandleSIGUSR1(int sig);

/************************************ main ************************************/
int main(int argc, const char* argv[])
{
	/* create wd_t struct */
	wd_t wd;
		
	/* set signal handlers */
	SetSignalHandlers();
	
	/* initialize struct fields */
	wd.argc = argc;
	wd.argv = argv;
	wd.max_misses = MAX_MISSES;
	wd.interval = INTERVAL;
	wd.revive_counter = 0;
	wd.wd_exec_path = argv[0];
	wd.other_process_pid = getppid();
	
	wd.scheduler = SchedCreate();
	ExitIfBad(NULL != wd.scheduler, FAIL, "wd_launch main: SchedCreate() FAILED!\n");
		
	/* init wd scheduler */
	InitSchedulerWD(&wd);
	
	/* run scheduler */
	SchedRun(wd.scheduler);
	
	return SUCCESS;
}

/************************************ Functions ************************************/
static int InitSchedulerWD(wd_t* wd)
{
	ilrd_uid_t task_uid = invalid_uid;
	time_t now = -1;
	
	/* assert */
	assert(NULL != wd);
	
	now = time(NULL);
	ExitIfBad(-1 != now, FAIL, "InitSchedulerWD(): time() FAILED!\n");
	
	/* add task - send signal */
	task_uid = SchedAddTask(wd->scheduler, now + wd->interval, SendSignalWD, (void*)wd, NULL, NULL, wd->interval);
	ExitIfBad(!UIDIsSame(task_uid, invalid_uid), FAIL, "InitSchedulerWD(): AddTask(SendSignal) FAILED!\n");
	
	/* add task - check counter */
	task_uid = SchedAddTask(wd->scheduler, now + wd->interval, CheckCounterWD, (void*)wd, NULL, NULL, wd->interval);
	ExitIfBad(!UIDIsSame(task_uid, invalid_uid), FAIL, "InitSchedulerWD(): AddTask(CheckCounter) FAILED!\n");
	
	/* return SUCCESS */
	return SUCCESS;
}

static int SendSignalWD(void* arg)
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);
	int status = FAIL;
	
	/* send signal to client */
	status = kill(wd->other_process_pid, SIGUSR1);
	ExitIfBad(0 == status, FAIL, "SendSignalWD(): kill() FAILED!\n");
	
	/* return TO_RESCHEDULE */
	return TO_RESCHEDULE;
}

static int CheckCounterWD(void* arg) /* wd_t* */
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);
	
	/* increment g_counter */
	__atomic_add_fetch(&g_counter, 1, __ATOMIC_SEQ_CST);
	
	/* if counter is above N */
	if (g_counter > wd->max_misses)
	{
		/* if revive counter is equal to MAX_REVIVES */
		if (wd->revive_counter >= MAX_REVIVES)
		{
			/* write to log */
			Log("CheckCounterWD(): Client didn't respond - exiting\n");
			
			/* stop scheduler and cleanup  */
			SchedStop(wd->scheduler);
			
			/* return NOT_RESCHEDULE */
			return NOT_RESCHEDULE;
		}
					
		/* increase revive counter in the struct */
		++wd->revive_counter;
		
		/* call revive client */
		return ReviveClient(wd);
	}
	
	/* return TO_RESCHEDULE */
	return TO_RESCHEDULE;
}

static int ReviveClient(void* arg) /* wd_t* */
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);

	/* kill client process */
	kill(wd->other_process_pid, SIGKILL);
	
	/* execv() */
	execv(wd->wd_exec_path, (char* const*)(wd->argv));
	
	/* if execv returns - failed */
	/* log */
	Log("ReviveClient(): execv() FAILED!\n");

	/* return FAIL */
	return FAIL;
}

/************************************ Signal Handler ************************************/
static int SetSignalHandlers()
{
	struct sigaction sa;
	int status = 0;
	
	memset(&sa, 0, sizeof(sa));

	/* set handlers with sigaction */
	sa.sa_handler = HandleSIGUSR1;
	
	status = sigaction(SIGUSR1, &sa, NULL);
	
	ExitIfBad(0 == status, FAIL, "SetSignalHandlers(): sigaction() FAILED!\n");
	
	/* return SUCCESS */
	return SUCCESS;
}

static void HandleSIGUSR1(int sig)
{
	/* assert */
	assert(sig == SIGUSR1);
	
	/* reset g_counter */
	__atomic_store_n(&g_counter, 0, __ATOMIC_SEQ_CST);
}
