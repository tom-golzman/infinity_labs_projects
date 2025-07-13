/**
	Written By: Tom Golzman
	Date: 09/07/2025
	Reviewed By: 
**/

/************************************includes************************************/
#include <assert.h> /* assert() */

#include "utils.h"	/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */
#include "wd.h"

/************************************define************************************/
enum { CLIENT = 0, WD = 1 , MAX_REVIVES = 3};

typedef struct watchdog
{
	int sig1_counter[2];
	
	int sig1_received[2];
	
	int dnr_received[2];
	
	int count_revives[2];

	pid_t pid_client;
	pid_t pid_wd;
	
	/* int current_role ??? or known by argv[1] */
} wd_t;

/************************************Functions************************************/
int MakeMeImmortal(int who_am_i_, const char* path_)
{
	/* mask SIGUSR1 + 2 */
	
	/* create thread */
	
	/* wait for an indication all is good or all is lost */
	
	/* return status */
}

ThreadFunc()
{
	/* set signal handlers */
	
	/* unmask SIGUSR1 + 2 */
	
	/* create wd process */
	
	/* wait for first signal */
	
	/* create client scheduler */

	/* init client scheduler */
		
	/* run client scheduler */
}

CreateWDProcess()
{
	/* fork */
		/* if parent */
			/* return */
		
		/* if child */
			/* execv() */
}

int SetSignalHandlers()
{
	/* set handlers with sigaction */
	
	/* return status */
}

/************************************Client Functions************************************/
int InitSchedulerClient(sched_t* scheduler)
{
	/* add task - send signal */
	/* add task - increment counter */
	/* add task - check counter */
	/* add task - check SIGUSR2 flag */
	
	/* return status */
}

int SendSignalClient(void* arg) /* struct with pid and signal ? */
{
	/* assert */
	
	/* send signal to WD*/
	
	/* return TO_RESCHEDULE */
}

int IncrementCounterClient(void* arg)
{
	/* assert ? */
	
	/* if signal flag is TRUE */
		
		/* increment counter */
		
		/* reset flag */
	
	/* return TO_RESCHEDULE */
}

int CheckCounterClient(void* arg)
{
	/* assert ? */
	
	/* if counter is above N */

		/* increase revive counter in the struct */
		
		/* call revive client */
		
		/* handle failure: stop scheduler and return NOT_RESCHEDULE */
	
	/* return TO_RESCHEDULE */
}

int CheckDNRClient(void* arg)
{
	/* send SIGUSR2 to WD ? */
}

int ReviveClient(void* arg) /* wd_t* */
{
	/* assert */
	
	/* kill wd process */
	
	/* create wd process */
	
	/* update pid_client in the struct */

	/* reset sig1_counter in the struct */

	/* reset received flag in the struct */
	
	/* wait for first signal */
	
	/* return status */
}

/************************************Watchdog Functions************************************/
int InitSchedulerWD(sched_t* scheduler)
{
	/* add task - send signal */
	/* add task - increment counter */
	/* add task - check counter */
	/* add task - check SIGUSR2 flag */
	
	/* return status */
}

int SendSignalWD(void* arg) /* struct with pid and signal ? */
{
	/* assert */
	
	/* send signal to Client */
	
	/* return TO_RESCHEDULE */
}

int IncrementCounterWD(void* arg)
{
	/* assert ? */
	
	/* if signal flag is TRUE */
		
		/* increment counter */
		
		/* reset flag */
	
	/* return TO_RESCHEDULE */
}

int CheckCounterWD(void* arg)
{
	/* assert ? */
	
	/* if counter is above N */
		
		/* increase revive counter in the struct */
		
		/* call revive wd */
		
		/* handle failure: stop scheduler and return NOT_RESCHEDULE */
	
	/* return TO_RESCHEDULE */
}

int CheckDNRWD(void* arg) /* wd_t* */
{
	/* assert */
	
	/* if dnr flag is TRUE */
		
		/* stop scheduler */
		
		/* return NOT_RESCHEDULE */
	
	/* return TO_RESCHEDULE */
}

int ReviveWD(void* arg) /* wd_t* */
{
	/* assert */
	
	/* kill wd process */
	
	/* create wd process */
	
	/* update pid_wd in the struct */
	
	/* reset sig1_counter in the struct */

	/* reset received flag in the struct */
		
	/* wait for first signal */
	
	/* return status */
}

/************************************Signal Handlers************************************/
void HandleSIGUSR1(int sig)
{
	/* update sig1_recieved flag */
}

void HandleDNR(int sig)
{
	/* update dnr_recieved flag */
}
