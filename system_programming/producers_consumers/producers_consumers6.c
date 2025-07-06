/**
	Written By: Tom Golzman
	Date: 02/07/2025
	Reviewed By: 
**/

/************************************includes*************************************/
#include <assert.h>			/* assert() */
#include <pthread.h>		/* pthread_t */
#include <semaphore.h>		/* sem_t */
#include <stdio.h>			/* printf */

#include "utils.h"			/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

/**************************************define*************************************/
enum { NUM_MESSAGES = 5, NUM_CONSUMERS = 3 };

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static sem_t sem;
static int g_message = 0;
static int g_consumers_left = 0;

/********************************Private Functions********************************/
static void* ProduceThreadFunc(void* arg);
static void* ConsumeThreadFunc(void* arg);
static int Produce(int message);
static void Consume(int message);

/************************************Functions************************************/
int main()
{
	/* create producer thread & consumers threads array */
		
	/* initialize consumers threads array */
	
	/* create a semaphore */
	
	/* while (1) */
		/* do nothing */
	
	/* return SUCCESS */
}

static void* ProduceThreadFunc(void* arg)
{
	/* for each number of messages */
		/* reset consumers left */
	
		/* produce a message */
		
		/* lock the mutex */
		
		/* for each number of consumers */
			/* increment the semaphore  by 1 */
		
		/* while there are consumers left */
			/* wait for them */
		
		/* unlock the mutex */
		
	/* return NULL */
}

static int Produce(int message)
{
	/* return the message */
}

static void* ConsumeThreadFunc(void* arg)
{
	/* while (1) */
		/* wait for semaphore (until a message is ready) */
			
		/* lock the mutex */
		
		/* copy the message to a local variable */
		
		/* decrement number of consumers left */
		
		/* send signal to the producer */
		
		/* unlock the mutex */
		
		/* consume the message */
	
	/* return NULL */
}

static void Consume(int message)
{
	/* consume the message */
}
