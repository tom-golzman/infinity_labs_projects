/**
	Written By: Tom Golzman
	Date: 02/07/2025
	Reviewed By: 
**/

/************************************includes*************************************/
#include <assert.h>		/* assert() */
#include <pthread.h>	/* pthread_t */
#include <stdlib.h>		/* calloc(), free() */
#include <semaphore.h>	/* sem_t */

#include "utils.h"		/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

#ifndef NDEBUG
	#include <stdio.h>
#endif

/**************************************define*************************************/
enum { NUM_ROUNDS = 3, NUM_PRODUCERS = 1, NUM_CONSUMERS = 3 };

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static sem_t sem;

static int message = 0;

/********************************Private Functions********************************/
static void* Producer(void* arg);
static void* Consumer(void* arg);

/************************************Functions************************************/
int main()
{
	int i = 0;
	int* id = NULL;
	int status = -1;

	/* create producer thread & consumers threads array */
	pthread_t producer;
	pthread_t consumers[NUM_CONSUMERS];
	
	/* create a semaphore */
	sem_init(&sem, 0, 0);
	
	/* initialize producer thread */
	status = pthread_create(&producer, NULL, Producer, NULL);
	ExitIfBad(0 == status, FAIL, "pthread_create() FAILED!\n");
	
	/* initialize consumers arrays with ids and threads */
	for (i = 0; i < NUM_CONSUMERS; ++i)
	{
		status = -1;
		id = (int*)calloc(1, sizeof(int));
		ExitIfBad(NULL != id, FAIL, "calloc() FAILED!\n");
		*id = i;
		
		status = pthread_create(&consumers[i], NULL, Consumer, id);
		ExitIfBad(0 == status, FAIL, "pthread_create() FAILED!\n");
	}
	
	/* join producer thread */
	pthread_join(producer, NULL);
	
	/* join pconsumers threads */
	for (i = 0; i < NUM_CONSUMERS; ++i)
	{
		pthread_join(consumers[i], NULL);
	}
	
	/* destroy the semaphore */
	sem_destroy(&sem);
	
	return SUCCESS;
}

static void* Producer(void* arg)
{
	int i = 0;
	int j = 0;
	int status = FAIL;
	int consumers_left = 0;	
	
	/* for each number of rounds */
	for (i = 1; i <= NUM_ROUNDS; ++i)
	{
		/* lock the mutex */
		pthread_mutex_lock(&mutex);
		
		/* update the message */
		message = i + 10;
		
		/* wake up all the consumers */
		for (j = 0; j < NUM_CONSUMERS; ++j)
		{
			/* increment the semaphore by 1 */
			sem_post(&sem);
		}
		
		/* get consumers left */
		status = sem_getvalue(&sem, &consumers_left);
		ExitIfBad(SUCCESS == status, FAIL, "sem_getvalue()!");
		
		DEBUG_ONLY(printf("consumers left: %d\n", consumers_left););		
		
		/* wait for all the comsumers to finish read the message */
		while (consumers_left > 0)
		{
			pthread_cond_wait(&cond, &mutex);
		}
		
		DEBUG_ONLY(printf("producer: message number %d\n", message););
		
		/* unlock the mutex */
		pthread_mutex_unlock(&mutex);
	}
	
	return NULL;
	
	(void)arg;
}

static void* Consumer(void* arg)
{
	int id = 0;

	
	assert(NULL != arg);
	
	/* save the arg (id) and free it */
	id = *(int*)arg;
	free(arg);
	
	while (1)
	{
		/* wait until the message is ready (semaphore is 1) */
		sem_wait(&sem);

		/* lock the mutex */
		pthread_mutex_lock(&mutex);
		
		DEBUG_ONLY(printf("consumer num %d: message number %d\n",id, message););
		
		/* wake up the producer */
		pthread_cond_signal(&cond);
		
		/* unlock the mutex */
		pthread_mutex_unlock(&mutex);
	}
	
	return NULL;
}
