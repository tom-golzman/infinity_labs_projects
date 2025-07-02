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
#include "dll.h"		/* dll_t, Create(), Destroy(), PushBack(), PopFront(), IsEmpty() */

#ifndef NDEBUG
	#include <stdio.h>
#endif

/**************************************define*************************************/
enum { NUM_ROUNDS = 2, NUM_PRODUCERS = 3, NUM_CONSUMERS = 3, LIST_SIZE = 5 };

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static dll_t* shared_list = NULL;
static sem_t sem_free_space;
static sem_t sem_used_space;

/********************************Private Functions********************************/
static void* Producer(void* arg);
static void* Consumer(void* arg);

/************************************Functions************************************/
int main()
{
	int i = 0;
	int* id = NULL;
	int status = -1;

	/* create producers & consumers threads arrays */
	pthread_t producers[NUM_PRODUCERS];
	pthread_t consumers[NUM_CONSUMERS];
	
	/* create a linked list */
	shared_list = DListCreate();
	ExitIfBad(NULL != shared_list, FAIL, "DListCreate() FAILED!");
	
	/* create a list mutex */
	pthread_mutex_init(&mutex, NULL);
	
	/* init the semaphores */
	sem_init(&sem_free_space, 0, LIST_SIZE);
	sem_init(&sem_used_space, 0, 0);
	
	/* initialize producers arrays with ids and threads */
	for (i = 0; i < NUM_PRODUCERS; ++i)
	{
		status = -1;
		id = (int*)calloc(1, sizeof(int));
		ExitIfBad(NULL != id, FAIL, "calloc() FAILED!\n");
		*id = i;
		
		status = pthread_create(&producers[i], NULL, Producer, id);
		ExitIfBad(0 == status, FAIL, "pthread_create() FAILED!\n");
	}
	
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
	
	/* join producers threads */
	for (i = 0; i < NUM_PRODUCERS; ++i)
	{
		pthread_join(producers[i], NULL);
	}
	
	/* join pconsumers threads */
	for (i = 0; i < NUM_CONSUMERS; ++i)
	{
		pthread_join(consumers[i], NULL);
	}
		
	/* destroy the list */
	DListDestroy(shared_list);
	
	/* destroy the mutex */
	pthread_mutex_destroy(&mutex);
		
	/* destroy the semaphores */
	sem_destroy(&sem_free_space);
	sem_destroy(&sem_used_space);
	
	return SUCCESS;
}

static void* Producer(void* arg)
{
	int i = 0;
	int id = 0;
	int* data = NULL;
	
	assert(NULL != arg);
	
	/* save the arg (id) and free it */
	id = *(int*)arg;
	free(arg);
	
	for (i = 0; i < NUM_ROUNDS; ++i)
	{
		/* allocate the data */
		data = calloc(1, sizeof(int));
		*data = id + i + 10;
		
		/* wait for free space */
		sem_wait(&sem_free_space);
		
		/* lock the mutex */
		pthread_mutex_lock(&mutex);
		
		/* push the data to the list */
		DListPushBack(shared_list, data);

		DEBUG_ONLY(printf("producer num %d: %d\n", id, *data););

		/* unlock the mutex */
		pthread_mutex_unlock(&mutex);

		/* increment the used space semaphore by 1 */
		sem_post(&sem_used_space);	
	}
	
	return NULL;
}

static void* Consumer(void* arg)
{
	int i = 0;
	int id = 0;
	int data = 0;
	int* temp_data = NULL;
	
	assert(NULL != arg);
	
	/* save the arg (id) and free it */
	id = *(int*)arg;
	free(arg);
	
	for (i = 0; i < NUM_ROUNDS; ++i)
	{
		/* wait until there is data to read in the used space semphore */
		sem_wait(&sem_used_space);

		/* lock the mutex */
		pthread_mutex_lock(&mutex);
		
		/* check if the list is not empty */
		if (!DListIsEmpty(shared_list))
		{
			/* pop the data from the list and free it */
			temp_data = (int*)DListGetData(DListBegin(shared_list));
			data = *temp_data;
			free(temp_data);

			DListPopFront(shared_list);
			
			DEBUG_ONLY(printf("consumer num %d: %d\n", id, data););

			/* unlock the mutex */
			pthread_mutex_unlock(&mutex);
			
			/* increment the free space semaphore by 1 */
			sem_post(&sem_free_space);
		}
	}
	
	return NULL;
}
