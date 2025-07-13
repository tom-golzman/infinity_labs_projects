/**
	Written By: Tom Golzman
	Date: 09/07/2025
	Reviewed By: 
**/

/************************************ includes ************************************/
#include <assert.h> /* assert() */

#include "utils.h"	/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */
#include "wd.h"

/************************************ defines************************************/
enum { CLIENT = 0, WD = 1, MAX_REVIVES = 3};

typedef struct watchdog
{
	int argc;
	char* argv[];
	int revive_counter;
	sched_t* scheduler;
	char* wd_exec_path;
	pid_t other_process_pid;
} wd_t;

static volatile sig_atomic_t g_counter = 1;
static int g_is_dnr_received = FALSE;

/************************************ Functions ************************************/
int MakeMeImmortal(int argc, char* argv[])
{
	/* mask signals */
	
	/* create thread */
	
	/* wait for an indication all is good or all is lost */
	
	/* return status */
}

void* ThreadFunc(void* arg)
{
	/* set signal handlers */
	
	/* unmask signals */
	
	/* create wd process */
	/* create client scheduler */
	/* init client scheduler */
	
	/* run scheduler */
}

int SetSignalHandlers()
{
	/* set handlers with sigaction */
	
	/* return status */
}

/******************************** phase 1 (init) functions ********************************/
int InitTasks(wd_t* wd)
{
	/* add task - increment counter */
	/* add task - check counter */
}

/******************************** phase 2 (mainstream) functions ********************************/
int MainstreamTasks(wd_t* wd)
{
	/* add task - send signal */
	/* add task - increment counter */
	/* add task - check counter client */
	/* add task - check DNR */
	
	/* return status */
}

/******************************** functions ********************************/
int InitSchedulerClient(sched_t* scheduler)
{
	/* phase 1: init */
		/* add task - increment counter */
		/* add task - check counter */
		
	/* phase 2: mainstream */
	/* add task - init mainstream tasks (send signal, increment counter, check counter, check SIGUSR2 flag */
	
	/* return status */
}

int CreateWDProcess(wd_t* wd)
{
	/* fork & handle failure */
	
		/* if parent */
			/* return SUCCESS */
		
		/* if child */
			/* update WD_PID in env */
			
			/* init argv[0] with wd_exec_path */
			/* init argv[1] with "wd" */
			
			/* execv() & handle failure */
}

void DNR()
{
	/* change global glaf of DNR to TRUE */
}

/******************************** tasks ********************************/
int CheckFirstSignalTask(void* arg) /* wd_t* */
{
	/* assert */
	
	/* if g_counter is zero */
		
		/* clear the scheduler */
		
		/* call MainstreamTasks() */
		
		/* return SUCCESS */
	
	/* print to log */
	
	/* stop scheduler and cleanup */
	
	/* return FAIL */
}

int SendSignalClientTask(void* arg)
{
	/* get WD_PID from env */
	
	/* send signal to WD*/
	
	/* return TO_RESCHEDULE */
}

int IncrementCounterClientTask(void* arg)
{
	/* increment g_counter */
	
	/* return TO_RESCHEDULE */
}

int CheckCounterClientTask(void* arg) /* wd_t* */
{
	/* assert */
	
	/* if counter is above N */

		/* if revive counter is equal or bigger to MAX_REVIVES */

			/* log */
			
			/* stop scheduler and cleanup */
			
			/* return NOT_RESCHEDULE */
		
					
		/* increase revive counter in the struct */
		
		/* call revive wd */
		
		/* handle failure: stop scheduler, cleanup and return NOT_RESCHEDULE */
	
		/* reset g_counter */
		
	/* return TO_RESCHEDULE */
}

int CheckDNRTask(void* arg)
{
	/* if received dnr */
		
		/* stop scheduler and cleanup */
		
		/* log */
		
		/* return NOT_RESCHEDULE */
		
	/* return TO_RESCHEDULE */
}

int ReviveWDTask(void* arg) /* wd_t* */
{
	/* assert */
	
	/* kill wd process: */
		/* get WD_PID from env */
		/* call kill() */
		
	/* create new wd process */
	
	/* reset g_counter */
	
	/* add task - wait first signal */

	/* reset revive counter */
	
	/* return SUCCESS */
}


int WaitFirstSignalTask(wd_t* wd)
{
	/* assert */
	
	/* if g_counter is zero */
		
		/* remove this task */
		
		/* call reset revive counter */
		
		/* return SUCCESS */
	
	/* print to log */
	
	/* stop scheduler and cleanup */
	
	/* return FAIL */
}

/************************************ Signal Handler ************************************/
void HandleSIGUSR1(int sig)
{
	/* reset g_counter */
}
