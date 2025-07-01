/**
	Written By: Tom Golzman
	Date: 01/07/2025
	Reviewed By: 
**/

/************************************includes*************************************/
#include <assert.h> /* assert() */
#include <pthread.h> /* pthread_t */


#include "utils.h"	/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

#ifndef NDEBUG
#include <stdio.h>
#endif
/**************************************define*************************************/
enum { NUM_ROUNDS = 3 };

volatile int lock = 0; /* 0 = opened , 1 = locked */
volatile int shared_val = 0;
volatile int is_val_available = FALSE;

/********************************Private Functions********************************/
static void* Producer(void* arg);
static void* Consumer(void* arg);
static void SpinLock();
static void SpinUnlock();

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
	
	for (i = 0; i < NUM_ROUNDS; ++i)
	{
		while (1)
		{
			SpinLock();
			if (!is_val_available)
			{
				shared_val = i;
				is_val_available = TRUE;
				DEBUG_ONLY(printf("producer: %d\n", i););
				SpinUnlock();
				break;
			}
			SpinUnlock();
		}
	}
	
	return NULL;

	(void)arg;
}

static void* Consumer(void* arg)
{
	int i = 0;
	int val = 0;
	
	for (i = 0; i < NUM_ROUNDS; ++i)
	{
		while (1)
		{
			SpinLock();
			if (is_val_available)
			{
				val = shared_val;
				is_val_available = FALSE;
				DEBUG_ONLY(printf("consumer: %d\n", val););
				SpinUnlock();
				break;
			}		
			
			SpinUnlock();
		}
	}
	
	return NULL;
	
	(void)arg;
}

static void SpinLock()
{
	while (__sync_lock_test_and_set(&lock, 1))
	{
		/* busy wait */
	}
}

static void SpinUnlock()
{
	__sync_lock_release(&lock);
}
