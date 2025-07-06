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
#include <stdlib.h>			/* calloc(), free() */

#include "utils.h"			/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

/**************************************define*************************************/
enum { NUM_MESSAGES = 5, NUM_CONSUMERS = 3 };

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static sem_t sem;
static volatile int g_message = 0;
static volatile int g_version = 0;
static volatile int g_consumers_left = 0;

/********************************Private Functions********************************/
static void* ProduceThreadFunc(void* arg);
static void* ConsumeThreadFunc(void* arg);
static int Produce(int message);
static void Consume(int id, int message);

/************************************Functions************************************/
int main()
{
	int i = 0;
	int* id = NULL;
	pthread_t producer;
	pthread_t consumers[NUM_CONSUMERS];

	/* create producer thread & consumers threads array */
	ExitIfBad(0 == pthread_create(&producer, NULL, ProduceThreadFunc, NULL), FAIL, "main: pthread_create(producer) FAILED!");	
		
	/* initialize consumers threads array */
	for (i = 0; i < NUM_CONSUMERS; ++i)
	{
		/* alloacte data */
		id = (int*)calloc(1, sizeof(int));
		/* handle failure */
		ExitIfBad(NULL != id, FAIL, "ps4 main: calloc(id) FAILED!\n");

		*id = i;
		
		/* create a thread & handle failure */
		ExitIfBad(0 == pthread_create(&consumers[i], NULL, ConsumeThreadFunc, id), FAIL, "main: pthread_create(consumer) FAILED!\n");
	}
	
	/* create a semaphore */
	sem_init(&sem, 0, 0);
	
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
	int i = 0, j = 0;
	int message = 0;

	/* for each number of messages */
	for (i = 0; i < NUM_MESSAGES; ++i)
	{
		/* produce a message to a local variable*/
		message = Produce(i);
		
		/* lock the mutex */
		pthread_mutex_lock(&mutex);
		
		/* reset consumers left */
		g_consumers_left = NUM_CONSUMERS;
		
		/* update global message & global version */
		g_message = message;		
		++g_version;
		
		/* unlock the mutex */
		pthread_mutex_unlock(&mutex);
		
		/* for each number of consumers */
		for (j = 0; j < NUM_CONSUMERS; ++j)
		{
			/* increment the semaphore  by 1 */
			sem_post(&sem);	
		}
		
		/* lock the mutex again to wait for consumers to finish */
		pthread_mutex_lock(&mutex);
		
		/* while there are consumers left */
		while (g_consumers_left > 0)
		{
			/* wait for them */
			pthread_cond_wait(&cond, &mutex);
		}
		
		/* unlock the mutex */
		pthread_mutex_unlock(&mutex);
	}
		
	/* return NULL */
	return NULL;
	
	(void)arg;
}

static int Produce(int message)
{
	DEBUG_ONLY(printf("produced: %d\n", message););	
	
	/* return the message */
	return message;
}

static void* ConsumeThreadFunc(void* arg)
{
	int message = 0;
	int local_version = -1;
	int should_signal = FALSE;
	int id = (assert(NULL != arg), *(int*)arg);
	
	free(arg);
	
	/* while (1) */
	while (1)
	{
		/* wait for semaphore (until a message is ready) */
		sem_wait(&sem);	
			
		/* lock the mutex */
		pthread_mutex_lock(&mutex);
		
		/* while the message is NOT new */
		while (local_version == g_version)
		{
			/* wait for the consition variable */
			pthread_cond_wait(&cond, &mutex);
		}
		
		/* copy the version to a local variable */
		local_version = g_version;
		
		/* copy the message to a local variable */
		message = g_message;
		
		/* decrement number of consumers left */
		--g_consumers_left;
		
		/* check if it is the last consumer */
		if (0 == g_consumers_left)
		{
			should_signal = TRUE;
		}
		
		/* unlock the mutex */
		pthread_mutex_unlock(&mutex);

		/* if it was the last consumer */
		if (FALSE != should_signal)
		{
			/* send signal to the producer */
			pthread_cond_signal(&cond);
		
			/* reset the flag */
			should_signal = FALSE;
		}
		
		/* consume the message */
		Consume(id, message);
	}
	
	/* return NULL */
	return NULL;
	
	(void)arg;
}

static void Consume(int id, int message)
{
	/* consume the message */
	printf("    consumer num %d: %d\n", id, message);	
}
