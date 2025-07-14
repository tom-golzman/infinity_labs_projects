/**
	Written By: Tom Golzman
	Date: 09/07/2025
	Reviewed By: 
**/

/************************************ includes ************************************/
#include <assert.h> /* assert() */

#include "utils.h"	/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

#include "scheduler.h"
#include "wd.h"

/************************************ defines************************************/
enum { CLIENT = 0, WD = 1, MAX_REVIVES = 3};

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
static int g_is_dnr_received = FALSE;

/************************************ Functions ************************************/
int MakeMeImmortal(int argc, char* argv[])
{
	/* mask signals */
	
	/* create thread */
	
	/* wait for an indication all is good or all is lost */
	
	/* return SUCCESS */
	return SUCCESS;
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
	/* add task - check counter client */
	/* add task - check DNR */
	
	/* return status */
}

/******************************** functions ********************************/
int InitSchedulerClient(wd_t* wd)
{
	/* phase 1: init */
		/* add task - check counter */
		
	/* phase 2: mainstream */
	/* add task - init mainstream tasks (send signal, check counter, check SIGUSR2 flag */
	
	/* return status */
}

int CreateWDProcess(wd_t* wd)
{
	/* fork & handle failure */
	
		/* if parent */
			/* return SUCCESS */
		
		/* if child */
			/* initialize wd_pid in the struct */
			/* execv() & handle failure */
}

void DNR()
{
	/* change global flag of DNR to TRUE */
}

/******************************** tasks ********************************/
int CheckFirstSignalClient(void* arg) /* wd_t* */
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

int SendSignalClient(void* arg)
{
	/* get WD_PID from env */
	
	/* send signal to WD*/
	
	/* return TO_RESCHEDULE */
}

int CheckCounterClient(void* arg) /* wd_t* */
{
	/* assert */
	
	/* increment g_counter */
	
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

int CheckDNR(void* arg)
{
	/* if received dnr */
		
		/* stop scheduler and cleanup */
		
		/* log */
		
		/* return NOT_RESCHEDULE */
		
	/* return TO_RESCHEDULE */
}

int ReviveWD(void* arg) /* wd_t* */
{
	/* assert */
	
	/* kill wd process */
		
	/* create new wd process */
	
	/* reset g_counter */
	
	/* add task - wait first signal */

	/* reset revive counter */
	
	/* return SUCCESS */
}


int WaitFirstSignal(wd_t* wd)
{
	/* assert */
	
	/* if g_counter is zero */
		
		/* remove this task */
		
		/* reset revive counter */
		
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
