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
	
	/* create a semaphore */
	sem_init(&sem, 0, 0);

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
	int status = 0;

	/* for each number of messages */
	for (i = 0; i < NUM_MESSAGES; ++i)
	{
		/* produce a message to a local variable*/
		message = Produce(i);
		
		/* lock the mutex */
		status = pthread_mutex_lock(&mutex);
		RET_IF_BAD(0 == status, NULL, "ProduceThreadFunc: pthread_mutex_lock(&mutex) FAILED!");
		
		/* update global message & global version */
		g_message = message;		
		++g_version;
		
		/* wake all the consumers */
		status = pthread_cond_broadcast(&cond);
		RET_IF_BAD(0 == status, NULL, "ProduceThreadFunc: pthread_cond_broadcast(&cond) FAILED!");
		
		/* unlock the mutex */
		status = pthread_mutex_unlock(&mutex);
		RET_IF_BAD(0 == status, NULL, "ProduceThreadFunc: pthread_mutex_unlock(&mutex) FAILED!");
		
		/* for each number of consumers */
		for (j = 0; j < NUM_CONSUMERS; ++j)
		{
			/* wait for semaphore */
			sem_wait(&sem);
		}
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
	int status = 0;
	int id = (assert(NULL != arg), *(int*)arg);
	
	free(arg);
	
	/* while (1) */
	while (1)
	{
		/* lock the mutex */
		status = pthread_mutex_lock(&mutex);
		RET_IF_BAD(0 == status, NULL, "ConsumeThreadFunc: pthread_mutex_lock(&mutex) FAILED!");
		
		/* while the message is NOT new */
		while (local_version == g_version)
		{
			/* wait for the consition variable */
			status = pthread_cond_wait(&cond, &mutex);
			RET_IF_BAD(0 == status, NULL, "ConsumeThreadFunc: pthread_cond_wait(&cond, &mutex) FAILED!");			
		}
		
		/* copy the version to a local variable */
		local_version = g_version;
		
		/* unlock the mutex */
		status = pthread_mutex_unlock(&mutex);
		RET_IF_BAD(0 == status, NULL, "ConsumeThreadFunc: pthread_mutex_unlock(&mutex) FAILED!");
		
		/* copy the message to a local variable */
		message = g_message;

		/* consume the message */
		Consume(id, message);
		
		/* increment the semaphore by 1 */
		sem_post(&sem);
		RET_IF_BAD(0 == status, NULL, "ConsumeThreadFunc: sem_post(&sem) FAILED!");		
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
