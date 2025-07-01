/**
	Written By: Tom Golzman
	Date: 01/07/2025
	Reviewed By: 
**/

/************************************includes*************************************/
#include <assert.h>		/* assert() */
#include <pthread.h>	/* pthread_t */

#include "utils.h"		/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

#ifndef NDEBUG
	#include <stdio.h>
#endif

/**************************************define*************************************/
enum { NUM_ROUNDS = 3 };

volatile int shared_val = 0;
volatile int is_val_available = 0;

/********************************Private Functions********************************/
static void* Producer(void* arg);
static void* Consumer(void* arg);

/************************************Functions************************************/
int main()
{
	pthread_t producer;
	pthread_t consumer;
	
	pthread_create(&producer, NULL, Producer, NULL);
	pthread_create(&consumer, NULL, Consumer, NULL);
	
	pthread_join(producer, NULL);
	pthread_join(consumer, NULL);
	
	return SUCCESS;
}

static void* Producer(void* arg)
{
	int i = 0;
	
	/* for each number of rounds */
	for (i = 0; i < NUM_ROUNDS; ++i)
	{
		while (1)
		{
			/* check if the value isn't available (was read) */
			if (0 == __sync_fetch_and_add(&is_val_available, 0))
			{
				/* update the shared value */
				shared_val = i;
				
				DEBUG_ONLY(printf("producer: %d\n", shared_val););

				/* change the value to be unavailable */
				__sync_fetch_and_add(&is_val_available, 1);
				
				break;
			}
		}
	}
	
	return NULL;

	(void)arg;
}

static void* Consumer(void* arg)
{
	int i = 0;
	
	/* for each number of rounds */
	for (i = 0; i < NUM_ROUNDS; ++i)
	{
		while (1)
		{
			/* check if the value is available (was written) */
			if (1 == __sync_fetch_and_add(&is_val_available, 0))
			{
				DEBUG_ONLY(printf("consumer: %d\n", shared_val););

				/* change the value to be unavailable */
				__sync_fetch_and_add(&is_val_available, -1);
				
				break;
			}		
		}
	}
	
	return NULL;
	
	(void)arg;
}
