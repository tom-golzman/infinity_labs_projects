/**
	Written By: Tom Golzman
	Date: 02/07/2025
	Reviewed By: 
**/

/************************************includes*************************************/
#include <assert.h>			/* assert() */

#include "utils.h"			/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

/**************************************define*************************************/
enum { NUM_PRODUCERS = 1, NUM_CONSUMERS = 3 };

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static sem_t sem;

/********************************Private Functions********************************/
static void* ProduceThreadFunc(void* arg);
static void* ConsumeThreadFunc(void* arg);
static int Produce(int val);
static void Consume(int* data);

/************************************Functions************************************/
int main()
{
	/* create producer thread*/ 
	
	/* create consumers threads array */
	
	/* create a semaphore */
	
	/* initialize producer thread */
	
	/* for each index in consumers array */
		/* allocate space for id */

		/* initialize id with i */
		
		/* create a thread with the id */

	/* while (1) */
		/* do nothing */
	
	/* return SUCCESS */
}

static void* ProduceThreadFunc(void* arg)
{
	/* while (1) */
		/* produce a message */
		
		/* lock the mutex */
		
		/* for each consumer */
			/* increment the semaphore  by 1 */
			
		/* check how many consumers left to read the message */
			
		/* while there are left consumers */
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

		/* send signal to the producer */
		
		/* unlock the mutex */

	/* consume the message */
	
	/* return NULL */
}

static void Consume(int message)
{
	/* consume the message */
}
