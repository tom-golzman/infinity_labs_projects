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

/************************************typedef************************************/

/************************************Functions************************************/
int MakeMeImmortal(int who_am_i_, const char* path_)
{
	/* mask SIGUSR1 + 2 */
	
	/* set signal handlers */
	
	/* create thread */
	
	/* wait for an indication all is good or all is lost */
	
	/* return status */
}

ThreadFunc()
{
	/* unmask SIGUSR1 + 2 */
	
	/* create wd process */
	
	/* wait for first signal */
	
	/* create client scheduler */
	
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

CreateClientScheduler()
{
	/* create scheduler */
	
	/* init scheduler */
	
	/* return scheduler */
}

InitClientScheduler()
{
	/* add task - send signal */
	/* add task - increase counter */
	/* add task - check counter */
	
	/* return status */
}

Revive()
{
	/* kill wd process */
	
	/* create process */
	/* wait for first signal */
	
	/* return status */
}
