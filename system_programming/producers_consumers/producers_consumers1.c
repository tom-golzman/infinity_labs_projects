/**
	Written By: Tom Golzman
	Date: 01/07/2025
	Reviewed By: 
**/

/************************************includes*************************************/
#include <assert.h>		/* assert() */
#include <pthread.h>	/* pthread_t */
#include <stdio.h>		/* printf() */

#include "utils.h"		/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

/**************************************define*************************************/
enum { CONSUMED = 0, PRODUCED = 1 };

volatile int shared_val = 0;
volatile int is_val_available = 0;

/********************************Private Functions********************************/
static void* ProduceThreadFunc(void* arg);
static void* ConsumeThreadFunc(void* arg);
static int Producer();
static void Consumer(int val);

/************************************Functions************************************/
int main()
{
	int status = 0;
	
	/* create threads for producer and consumer */
	pthread_t producer;
	pthread_t consumer;
	
	status = pthread_create(&producer, NULL, ProduceThreadFunc, NULL);
	ExitIfBad(SUCCESS == status, FAIL, "pthread_create() producer FAILED!");
	
	status = pthread_create(&consumer, NULL, ConsumeThreadFunc, NULL);
	ExitIfBad(SUCCESS == status, FAIL, "pthread_create() consumer FAILED!");

	/* while (1) */
	while (1)
	{
		/* do nothing */
	}

	/* return SUCCESS */
	return SUCCESS;
}

static void* ProduceThreadFunc(void* arg)
{
	int val = 0;
		
	while (1)
	{
		/* call Producer() */
		val = Producer();

		/* atomic function - while the value wasn't read */
		while (CONSUMED != __atomic_load_n(&is_val_available, __ATOMIC_SEQ_CST))
		{
			/* busy wait */
		}
			
		/* write the shared value */
		shared_val = val;
		
		/* atomic function - update that the value was written */
		__atomic_store_n(&is_val_available, PRODUCED, __ATOMIC_SEQ_CST);
	}
	
	return NULL;
	
	(void)arg;
}

static int Producer()
{
	static int val = 10;
	
	printf("produced: %d\n", val);
	
	++val;
	
	/* return the value */
	return val;
}

static void* ConsumeThreadFunc(void* arg)
{
	int val = 0;
	
	while (1)
	{
		/* atomic function - while the value wasn't written */
		while (PRODUCED != __atomic_load_n(&is_val_available, __ATOMIC_SEQ_CST))	
		{
			/* busy wait */
		}
			
		/* copy the shared value */
		val = shared_val;
		
		/* atomic function - update that the value was read */
		__atomic_store_n(&is_val_available, CONSUMED, __ATOMIC_SEQ_CST);
		
		/* call Consumer() */
		Consumer(val);
	}
	
	return NULL;
	
	(void)arg;
}

static void Consumer(int val)
{
	/* print the value */
	printf("consumer: %d\n", val);
}
