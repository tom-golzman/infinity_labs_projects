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
#include <stdio.h>		/* printf() */

#include "utils.h"		/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */
#include "dll.h"		/* dll_t, Create(), Destroy(), PushBack(), PopFront(), IsEmpty() */

/**************************************define*************************************/
enum { NUM_ROUNDS = 1, NUM_PRODUCERS = 5, NUM_CONSUMERS = 5 };

static dll_t* shared_list = NULL;
static pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;
static sem_t items_available;

/********************************Private Functions********************************/
static void* ProduceThreadFunc(void* arg);
static void* ConsumeThreadFunc(void* arg);
static int* Producer(int val);
static void Consumer(int* data);

/************************************Functions************************************/
int main()
{
	int i = 0;
	int* val = NULL;
	int status = -1;
	
	/* create producers & consumers threads arrays */
	pthread_t producers[NUM_PRODUCERS];
	pthread_t consumers[NUM_CONSUMERS];
	
	/* create a linked list */
	shared_list = DListCreate();
	/* handle failure */
	ExitIfBad(NULL != shared_list, FAIL, "DListCreate() FAILED!");
	
	/* create a semaphore */
	sem_init(&items_available, 0, 0);
	
	/* for each index in producers array */
	for (i = 0; i < NUM_PRODUCERS; ++i)
	{
		/* alloacte data */
		status = -1;
		val = (int*)calloc(1, sizeof(int));
		/* handle failure */
		ExitIfBad(NULL != val, FAIL, "calloc() FAILED!\n");
		*val = i;

		/* create a thread */
		status = pthread_create(&producers[i], NULL, ProduceThreadFunc, val);
		/* handle failure */
		ExitIfBad(0 == status, FAIL, "pthread_create() FAILED!\n");
	}
		
	/* for each index in consumers array */
	for (i = 0; i < NUM_CONSUMERS; ++i)
	{
		/* alloacte data */
		status = -1;

		/* create a thread */		
		status = pthread_create(&consumers[i], NULL, ConsumeThreadFunc, NULL);
		/* handle failure */
		ExitIfBad(0 == status, FAIL, "pthread_create() FAILED!\n");
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
	int val = 0;
	int* data = NULL;
	
	/* assert */
	assert(NULL != arg);
	
	val = *(int*)arg;
	free(arg);
	
	/* while 1 */
	while (1)
	{
		/* call Producer() */
		data = Producer(val);
		
		/* lock the mutex */
		pthread_mutex_lock(&list_mutex);
		
		/* push the value to the list */
		DListPushBack(shared_list, data);
		
		/* increment the semaphore by 1 with sem_post() */
		sem_post(&items_available);	
			
		/* unlock the mutex */
		pthread_mutex_unlock(&list_mutex);		
	}
	
	/* return NULL */
	return NULL;
}

static int* Producer(int val)
{
	int* data = NULL;

	/* allocate the data */	
	data = calloc(1, sizeof(int));
	/* handle failure */
	ExitIfBad(NULL != data, FAIL, "calloc() data FAILED!");

	printf("produced: %d\n", val);

	*data = val;
	
	return data;
}

static void* ConsumeThreadFunc(void* arg)
{
	int* data = NULL;
	
	/* while 1 */
	while (1)
	{
		/* wait for available items */
		sem_wait(&items_available);
			
		/* lock the mutex */
		pthread_mutex_lock(&list_mutex);
		
		/* pop the value from the list */
		data = (int*)DListGetData(DListBegin(shared_list));
		
		/* remove the value from the list */
		DListRemove(DListBegin(shared_list));
		
		/* unlock the mutex */
		pthread_mutex_unlock(&list_mutex);	
		
		/* call Consumer() */
		Consumer(data);
	}		
	
	/* return NULL */
	return NULL;
	
	(void)arg;
}

static void Consumer(int* data)
{
	printf("consumed: %d\n", *data);

	free(data);
}
