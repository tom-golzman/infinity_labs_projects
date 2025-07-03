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

/********************************Private Functions********************************/
static fsq_t* FSQCreate(size_t capacity);
static void FSQDestroy(fsq_t* fsq);
static void FSQEnqueue(fsq_t* fsq, int* data);
static int FSQDequeue(fsq_t* fsq);

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
	
	return SUCCESS;
}
static fsq_t* FSQCreate(size_t capacity)
{
	fsq_t* fsq = NULL;
	int* array = NULL;
	
	assert(capacity > 0);
	
	fsq = (fsq_t*)calloc(1, sizeof(fsq_t));
	ExitIfBad(NULL != fsq_t, FAIL, "fsq calloc() FAILED!");
	
	array = (int*)calloc(capacity, sizeof(int));
	if (NULL == array)
	{
		FSQDestroy(fsq);
		
		return NULL;
	}
	
	fsq->capacity = capacity;
	fsq->read_idx = 0;
	fsq->write_idx = 0;
	
	pthread_mutex_init(&fsq->enqueue_mutex, NULL);
	pthread_mutex_init(&fsq->dequeue_mutex, NULL);
	
	sem_init(&fsq->sem_free_space, 0, capacity);
	sem_init(&fsq->sem_free_space, 0, 0);

	return fsq;
}

static void FSQDestroy(fsq_t* fsq)
{
	if (NULL == fsq)
	{
		return;
	}
	
	free(fsq->array);
	
	pthread_mutex_destroy(&fsq->enqueue_mutex);
	pthread_mutex_destroy(&fsq->dequeue_mutex);
	
	sem_destroy(&fsq->sem_free_space);
	sem_destroy(&fsq->sem_used_space);

	DEBUG_ONLY(
		fsq->array = BAD_MEM(int*);
		fsq->capacity = 0;
		fsq->read_idx = 0;
		fsq->write_idx = 0;
	);

	free(fsq);
}

static void FSQEnqueue(fsq_t* fsq, int* data)
{
	assert(NULL != fsq);

	sem_wait(&fsq->sem_free_space);
}

static int FSQDequeue(fsq_t* fsq);
