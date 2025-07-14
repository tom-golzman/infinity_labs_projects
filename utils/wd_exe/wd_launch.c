/**
	Written By: Tom Golzman
	Date: 09/07/2025
	Reviewed By: 
**/

/************************************ includes ************************************/
#include <signal.h>	/* sigaction, sig_atomic_t */
#include <unistd.h>	/* getppid, execv, kill */
#include <assert.h>	/* assert */
#include <time.h>	/* time */

#include "utils.h"	/* ExitIfBad, Log */

#include "scheduler.h"
#include "wd.h"

/************************************ defines ************************************/
enum { MAX_MISSES = 5, INTERVAL = 1, MAX_REVIVES = 3 };

static volatile sig_atomic_t g_counter = 1;

/************************************ main ************************************/
int main(int argc, const char* argv[])
{
	int status = 0;
	
	/* create wd_t struct */
	wd_t wd = {0};
		
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
	ExitIfBad(NULL != wd.scheduler, FAIL, "wd_launch main: SchedCreate() FAILED!");
		
	/* init wd scheduler */
	InitSchedulerWD(&wd);
	
	/* run scheduler */
	SchedRun(wd.scheduler);
}

/************************************ Functions ************************************/
int InitSchedulerWD(wd_t* wd)
{
	ilrd_uid_t task_uid = INVALID_UID;
	time_t now = -1;
	
	/* assert */
	assert(NULL != wd);
	
	now = time(NULL);
	ExitIfBad(-1 != now, FAIL, "InitSchedulerWD(): time() FAILED!");
	
	/* add task - send signal */
	task = SchedAddTask(wd->scheduler, now + wd->interval, SendSignalWD, (void*)wd, NULL, NULL, wd->interval);
	ExitIfBad(INVALID_UID != task_uid, FAIL, "InitSchedulerWD(): AddTask(SendSignal) FAILED!");
	
	/* add task - check counter */
	task = SchedAddTask(wd->scheduler, now + wd->interval, CheckCounterWD, (void*)wd, NULL, NULL, wd->interval);
	ExitIfBad(INVALID_UID != task_uid, FAIL, "InitSchedulerWD(): AddTask(CheckCounter) FAILED!");
	
	/* return SUCCESS */
	return SUCCESS;
}

int SendSignalWD(void* arg)
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);
	int status = FAIL;
	
	/* send signal to client */
	status = kill(wd->other_proccess_pid, SIGUSR1);
	ExitIfBad(0 == status, FAIL, "SendSignalWD(): kill() FAILED!");
	
	/* return TO_RESCHEDULE */
	return TO_RESCHEDULE
}

int CheckCounterWD(void* arg) /* wd_t* */
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
			Log("CheckCounterWD(): Client didn't respond - exiting");
			
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

int ReviveClient(void* arg) /* wd_t* */
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);

	/* kill client process */
	kill(wd->other_proccess_pid, SIGKILL);
	
	/* execv() */
	execv(wd->wd_exec_path, wd->argv);
	
	/* if execv returns - failed */
	/* log */
	Log("ReviveClient(): execv() FAILED!");

	/* return FAIL */
	return FAIL;
}

/************************************ Signal Handler ************************************/
int SetSignalHandlers()
{
	struct sigaction sa;
	int status = 0;
	
	/* set handlers with sigaction */
	sa.sa_handler = HandleSIGUSR1;
	
	status = sigaction(SIGUSR1, &sa, NULL);
	
	ExitIfBad(0 != status, FAIL, "SetSignalHandlers(): sigaction() FAILED!");
	
	/* return SUCCESS */
	return SUCCESS;
}

void HandleSIGUSR1(int sig)
{
	/* assert */
	assert(sig == SIGUSR1);
	
	/* reset g_counter */
	__atomic_store_n(&counter, 0, __ATOMIC_SEQ_CST);
}
