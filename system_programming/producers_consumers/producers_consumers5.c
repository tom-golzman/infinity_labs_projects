/**
	Written By: Tom Golzman
	Date: 06/07/2025
	Reviewed By: Sami
**/

/************************************includes*************************************/
#include <assert.h>		/* assert() */
#include <pthread.h>	/* pthread_t */
#include <stdlib.h>		/* calloc(), free() */
#include <stdio.h>		/* printf() */

#include "utils.h"		/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */
#include "reentrable_circular_queue.h"	/* rcq_t, RCQCreate(), RCQDestroy(), RCQEnqueue(), RCQDequeue() */

/**************************************define*************************************/
enum { CAPACITY = 5, NUM_PRODUCERS = 3, NUM_CONSUMERS = 3 };

static rcq_t* g_rcq = NULL;

/********************************Private Functions********************************/
static void* ProduceThreadFunc(void* arg);
static void* ConsumeThreadFunc(void* arg);
static int Produce(int val);
static void Consume(int message);

/************************************Functions************************************/
int main()
{
	int i = 0;
	int* val = NULL;
	
	/* create Producers & Consumers threads arrays */
	pthread_t producers[NUM_PRODUCERS];
	pthread_t consumers[NUM_CONSUMERS];

	/* create a global reentrable circular queue */
	g_rcq = RCQCreate(CAPACITY);
	ExitIfBad(NULL != g_rcq, FAIL, "ps5 main: WQCreate() Failed!");
	
	/* for each index in producers array */
	for (i = 0; i < NUM_PRODUCERS; ++i)
	{
		/* alloacte val */
		val = (int*)calloc(1, sizeof(int));
		/* handle failure */
		ExitIfBad(NULL != val, FAIL, "ps5 main: calloc() FAILED!\n");

		*val = i;

		/* create a thread & handle failure */
		ExitIfBad(0 == pthread_create(&producers[i], NULL, ProduceThreadFunc, val), FAIL, "ps5 main: pthread_create() FAILED!\n");
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
	int status = 0;
	int message = 0;
	int val = (assert(NULL != arg), *(int*)arg);
	
	/* free arg */
	free(arg);
	
	/* while (1) */
	while (1)
	{
		/* call Produce() */
		message = Produce(val);
		
		/* enqueue the message to the rcq */
		status = RCQEnqueue(g_rcq, message);
		RET_IF_BAD(SUCCESS == status, NULL, "ProduceThreadFunc: RCQEnqueue() FAILED!");
	}

	/* return NULL */
	return NULL;

	(void)arg;
}

static int Produce(int val)
{
	printf("produced: %d\n", val);

	return val;
}

static void* ConsumeThreadFunc(void* arg)
{
	int status = 0;
	int message = 0;
	
	/* while (1) */
	while (1)
	{
		/* dequeue message from the rcq */
		status = RCQDequeue(g_rcq, &message);
		RET_IF_BAD(SUCCESS == status, NULL, "ConsumeThreadFunc: RCQDequeue() FAILED!");
		
		/* call Consume() */
		Consume(message);
	}
	
	/* return NULL */
	return NULL;
	
	(void)arg;
}

static void Consume(int message)
{
	printf("consumed: %d\n", message);
}
