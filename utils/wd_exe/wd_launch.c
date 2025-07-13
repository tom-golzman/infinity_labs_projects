/**
	Written By: Tom Golzman
	Date: 09/07/2025
	Reviewed By: 
**/

/************************************ includes ************************************/
#include "wd.h"

/************************************ defines ************************************/
static volatile sig_atomic_t g_counter = 1;

/************************************ main ************************************/
int main(int argc, const char* argv[])
{
	/* set signal handlers */
	
	/* create wd_t struct */
	
	/* initialize struct fields */
	
	/* init wd scheduler */
	
	/* run scheduler */
}

/************************************ Functions ************************************/
int InitSchedulerWD(sched_t* scheduler)
{
	/* add task - send signal */
	/* add task - check counter */
	
	/* return status */
}

int SendSignalWD(void* arg)
{
	/* send signal to client */
	
	/* return TO_RESCHEDULE */
}

int CheckCounterWD(void* arg)
{
	/* assert */
	
	/* if counter is above N */

		/* if revive counter is equal to MAX_REVIVES */

			/* write to log */
			
			/* stop scheduler and cleanup  */
			
			/* return NOT_RESCHEDULE */
		
					
		/* increase revive counter in the struct */
		
		/* call revive client */
	
	/* return TO_RESCHEDULE */
}

int ReviveClient(void* arg) /* wd_t* */
{
	/* assert */
	
	/* execv() */
	/* if failed */
		/* log */
		/* return FAIL */
		
	/* return SUCCESS */
}

/************************************ Signal Handler ************************************/
void HandleSIGUSR1(int sig)
{
	/* reset g_counter */
}
