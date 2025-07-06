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
	int i = 0;
	pthread_t producer;
	pthread_t consumers[NUM_CONSUMERS];

	/* create producer thread & consumers threads array */
	ExitIfBad(0 == pthread_create(&producer, NULL, ProduceThreadFunc, NULL), FAIL, "main: pthread_create(producer) FAILED!");	
		
	/* initialize consumers threads array */
	for (i = 0; i < NUM_CONSUMERS; ++i)
	{
		/* create a thread & handle failure */
		ExitIfBad(0 == pthread_create(&consumers[i], NULL, ConsumeThreadFunc, NULL), FAIL, "main: pthread_create(consumer) FAILED!\n");
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

	/* for each number of messages */
	for (i = 0; i < NUM_MESSAGES; ++i)
	{
		/* reset consumers left */
		g_consumers_left = NUM_CONSUMERS;
	
		/* produce a message */
		g_message = Produce(i);
		
		/* lock the mutex */
		pthread_mutex_lock(&mutex);
		
		/* for each number of consumers */
		for (j = 0; j < NUM_CONSUMERS; ++j)
		{
			/* increment the semaphore  by 1 */
			sem_post(&sem);	
		}
		
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
	
	/* while (1) */
	while (1)
	{
		/* wait for semaphore (until a message is ready) */
		sem_wait(&sem);	
			
		/* lock the mutex */
		pthread_mutex_lock(&mutex);
		
		/* copy the message to a local variable */
		message = g_message;
		
		/* decrement number of consumers left */
		--g_consumers_left;
		
		/* send signal to the producer */
		pthread_cond_signal(&cond);
		
		/* unlock the mutex */
		pthread_mutex_unlock(&mutex);
		
		/* consume the message */
		Consume(message);
	}
	
	/* return NULL */
	return NULL;
	
	(void)arg;
}

static void Consume(int message)
{
	/* consume the message */
	printf("consumed: %d\n", message);	
}
