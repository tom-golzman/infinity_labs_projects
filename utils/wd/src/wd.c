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
int MakeMeImmortal(int who_am_i_, const char* path)
{
	/* if client */
		/* fork() */
		
		/* child */
			/* execv() for wd_client */
		
		/* parent */
			/* call InitScheduler() */
			
			/* create a thread */
		
	/* if wd */
		/* call InitScheduler() */

		/* create a thread */
	
	/* return NULL */
}

static void* ImmortalThreadFunc(void* arg)
{
	/* run scheduler */
	
	/* return NULL */
}

static sched_t* InitScheduler()
{
	/* create scheduler */
	
	/* create tasks */
	/* create task - send signal */
	/* create task - check the counter */
	/* add tasks to the scheduler */
	
	/* return scheduler */
}

CheckCounterTask()
{
	/* if counter is smaller or equal to N */
		/* return */

	/* if client */
		/* clear all tasks */	
		/* create task - run MakeMeImmortal() */
		/* add task to the scheduler */
	
	/* return */
}
