/**
	Written By: Tom Golzman
	Date: 02/07/2025
	Reviewed By: 
**/

/************************************includes*************************************/
#include <assert.h>			/* assert() */
#include <pthread.h>		/* pthread_t */
#include <stdlib.h>			/* calloc(), free() */
#include <semaphore.h>		/* sem_t */
#include <stdio.h>			/* printf */

#include "utils.h"			/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */
#include "waitable_queue.h"	/* wq_t, WQCreate(), WQDestroy(), WQEnqueue(), WQDequeue() */

/**************************************define*************************************/
enum { CAPACITY = 5, NUM_PRODUCERS = 3, NUM_CONSUMERS = 3 };

static wq_t* g_queue;

/********************************Private Functions********************************/
static void* ProduceThreadFunc(void* arg);
static void* ConsumeThreadFunc(void* arg);
static int Produce(int val);
static void Consume(int* data);

/************************************Functions************************************/
int main()
{
	int i = 0;
	int* val = NULL;
	
	/* create Producers & Consumers threads arrays */
	pthread_t producers[NUM_PRODUCERS];
	pthread_t consumers[NUM_CONSUMERS];

	/* create a global waitable queue & handle failure */
	g_queue = WQCreate(CAPACITY);
	ExitIfBad(NULL != g_queue, FAIL, "ps4 main: WQCreate() Failed!");
	
	/* for each index in producers array */
	for (i = 0; i < NUM_PRODUCERS; ++i)
	{
		/* alloacte data */
		val = (int*)calloc(1, sizeof(int));
		/* handle failure */
		ExitIfBad(NULL != val, FAIL, "ps4 main: calloc() FAILED!\n");

		*val = i;

		/* create a thread & handle failure */
		ExitIfBad(0 == pthread_create(&producers[i], NULL, ProduceThreadFunc, val), FAIL, "ps4 main: pthread_create() FAILED!\n");
	}
		
	/* for each index in consumers array */
	for (i = 0; i < NUM_CONSUMERS; ++i)
	{
		/* create a thread & handle failure */
		ExitIfBad(0 == pthread_create(&consumers[i], NULL, ConsumeThreadFunc, NULL), FAIL, "ps4 main: pthread_create() FAILED!\n");
	}
	
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
	int value = 0;
	int val = (assert(NULL != arg), *(int*)arg);
	
	free(arg);
	
	/* while 1 */
	while(1)
	{
		/* call Produce() */
		value = Produce(val);
		
		/* enqueue value */
		RET_IF_BAD_CLEAN(SUCCESS == WQEnqueue(g_queue, value), NULL, "ProduceThreadFunc: WQEnqueue() FAILED!", WQDestroy(g_queue));
	}
	
	/* return NULL */
	return NULL;
}

static int Produce(int val)
{
	printf("produced: %d\n", val);

	return val;
}

static void* ConsumeThreadFunc(void* arg)
{
	int value = 0;
	
	/* while 1 */
	while (1)
	{
		/* dequeue value */
		RET_IF_BAD_CLEAN(SUCCESS == WQDequeue(g_queue, &value), NULL, "ConsumeThreadFunc: WQDequeue() FAILED!", WQDestroy(g_queue));
		
		/* call Consume() */
		Consume(&value);
	}
	
	/* return NULL */
	return NULL;
	
	(void)arg;	
}

static void Consume(int* value)
{
	printf("consumed: %d\n", *value);
}
