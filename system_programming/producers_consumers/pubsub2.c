/**
	Written By: Tom Golzman
	Date: 01/07/2025
	Reviewed By: 
**/

/************************************includes*************************************/
#include <assert.h>		/* assert() */
#include <pthread.h>	/* pthread_t */
#include <stdlib.h>		/* calloc(), free() */

#include "utils.h"		/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */
#include "dll.h"		/* dll_t, Create(), Destroy(), PushBack(), PopFront(), IsEmpty() */

#ifndef NDEBUG
	#include <stdio.h>
#endif

/**************************************define*************************************/
enum { NUM_ROUNDS = 10, NUM_PRODUCERS = 10, NUM_CONSUMERS = 10 };

static dll_t* shared_list = NULL;
static pthread_mutex_t list_mutex;

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
	pthread_mutex_init(&list_mutex, NULL);
	
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
	
	DListDestroy(shared_list);
	pthread_mutex_destroy(&list_mutex);
	
	return SUCCESS;
}

static void* Producer(void* arg)
{
	int i = 0;
	int id = 0;
	int* data = NULL;
	
	assert(NULL != arg);
	
	id = *(int*)arg;
	free(arg);
	
	data = calloc(1, sizeof(int));
	*data = id + i + 10;
	
	pthread_mutex_lock(&list_mutex);
	
	DListPushBack(shared_list, data);
	
	DEBUG_ONLY(printf("producer num %d: %d\n", id, *data););

	pthread_mutex_unlock(&list_mutex);

	return NULL;
}

static void* Consumer(void* arg)
{
	int id = 0;
	int data = 0;
	int* temp_data = NULL;
	
	assert(NULL != arg);
	
	id = *(int*)arg;
	free(arg);
	
	pthread_mutex_lock(&list_mutex);
	
	if (!DListIsEmpty(shared_list))
	{
		temp_data = (int*)DListGetData(DListBegin(shared_list));
		data = *temp_data;
		free(temp_data);

		DListPopFront(shared_list);
		
		DEBUG_ONLY(printf("consumer num %d: %d\n", id, data););
	}

	pthread_mutex_unlock(&list_mutex);
	
	return NULL;
}
