/**
	Written By: Tom Golzman
	Date: 03/07/2025
	Reviewed By: 
**/

/************************************includes************************************/
#include <assert.h>				/* assert() */
#include <stdlib.h>				/* calloc() */
#include <semaphore.h>			/* sem_t */
#include <pthread.h>			/* pthread_t */

#include "utils.h"				/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

#include "circular_buffer.h"	/* cb_t */
#include "waitable_queue.h"

/************************************define************************************/
struct waitable_queue
{
	cb_t* cb;
	sem_t sem_free_space;
	sem_t sem_used_space;
	pthread_mutex_t mutex;
};

/************************************Functions************************************/
wq_t* WQCreate(size_t capacity)
{
	wq_t* queue = NULL;
	
	/* assert - capacity bigger than 0*/
	assert(capacity > 0);
	
	/* allocate space for the struct */
	queue = (wq_t*)calloc(1, sizeof(wq_t));
	/* handle failure */
	RET_IF_BAD(NULL != queue, NULL, "malloc(wq_t) FAILED!");

	/* create circular buffer & handle failure */
	RET_IF_BAD_CLEAN(NULL != CBCreate(capacity * sizeof(int)), NULL, "CBCreate() FAILED!", WQDestroy(queue));
	
	/* create free space semaphore & handle failure */
	RET_IF_BAD_CLEAN(0 == sem_init(&queue->sem_free_space, 0, capacity), NULL, "sem_init(sem_free_space) FAILED!", WQDestroy(queue));

	/* create used space semaphore & handle failure */
	RET_IF_BAD_CLEAN(0 == sem_init(&queue->sem_used_space, 0, 0), NULL, "sem_init(sem_used_space) FAILED!", WQDestroy(queue));
	
	/* create mutex & handle failure */
	RET_IF_BAD_CLEAN(0 == pthread_mutex_init(&queue->mutex, NULL), NULL, "pthread_mutex_init(mutex) FAILED!", WQDestroy(queue));
	
	/* return waitable queue */
	return queue;
}

int WQDestroy(wq_t* queue)
{
	/* if the pointer is NULL */
	if (NULL == queue)
	{
		/* return SUCCESS */
		return SUCCESS;
	}
	
	/* destroy circular buffer */
	CBDestroy(queue->cb);
	
	/* destroy free space semaphore & handle failure*/
	RET_IF_BAD(0 == sem_destroy(&queue->sem_free_space), FAIL, "sem_destroy(sem_free_sapce) FAILED!");
	
	/* destroy used space semaphore & handle failure*/
	RET_IF_BAD(0 == sem_destroy(&queue->sem_used_space), FAIL, "sem_destroy(sem_used_sapce) FAILED!");
	
	/* destroy mutex & handle failure */
	RET_IF_BAD(0 == pthread_mutex_destroy(&queue->mutex), FAIL, "mutex_destroy(mutex) FAILED!");
	
	/* BAD_MEM */
	DEBUG_ONLY(
		queue->cb = BAD_MEM(cb_t*);
	);
	
	/* reutrn SUCCESS */
	return SUCCESS;
}

int WQEnqueue(wq_t* queue, int* value)
{
	size_t write_result = 0;
	
	/* assert - queue */
	assert(NULL != queue);

	/* wait for semaphore free_space & handle failure */
	RET_IF_BAD_CLEAN(0 == sem_wait(&queue->sem_free_space), FAIL, "sem_wait(sem_free_space) FAILED!", WQDestroy(queue));
	
	/* lock the mutex & handle failure */
	RET_IF_BAD_CLEAN(0 == pthread_mutex_lock(&queue->mutex), FAIL, "pthread_mutex_lock(mutex) FAILED!", WQDestroy(queue));
	
	/* assert - enough sapce in the circular buffer */
	assert(CBGetSize(queue->cb) < CBGetCapacity(queue->cb));
	
	/* write value to the circular buffer */
	write_result = CBWrite(queue->cb, value, sizeof(int));
	
	/* unlock the mutex */
	RET_IF_BAD_CLEAN(0 == pthread_mutex_unlock(&queue->mutex), FAIL, "pthread_mutex_unlock(mutex) FAILED!", WQDestroy(queue));
	
	/* assert - we wrote sizeof(int) bytes */
	assert(write_result == sizeof(int));
	
	/* increase the semaphore used space */
	RET_IF_BAD_CLEAN(0 == sem_post(&queue->sem_used_space), FAIL, "sem_post(sem_used_space) FAILED!", WQDestroy(queue));
	
	/* reutrn SUCCESS */
	return SUCCESS;
}

int WQDequeue(wq_t* queue, int* output)
{
	int* value = NULL;
	size_t read_result = 0;
	
	/* assert - queue */
	assert(NULL != queue);
	
	/* wait for semaphore used_space */
	RET_IF_BAD_CLEAN(0 == sem_wait(&queue->sem_used_space), FAIL, "sem_wait(sem_used_space) FAILED!", WQDestroy(queue));
	
	/* lock the mutex & handle failure */
	RET_IF_BAD_CLEAN(0 == pthread_mutex_lock(&queue->mutex), FAIL, "pthread_mutex_lock(mutex) FAILED!", WQDestroy(queue));
		
	/* assert - circular buffer isn't empty */
	assert(CBGetSize(queue->cb) > 0);
		
	/* read value from the circular buffer */
	read_result = CBRead(queue->cb, value, sizeof(int));
		
	/* unlock the mutex */
	RET_IF_BAD_CLEAN(0 == pthread_mutex_unlock(&queue->mutex), FAIL, "pthread_mutex_unlock(mutex) FAILED!", WQDestroy(queue));
		
	/* assert - we read sizeof(int) bytes */
	assert(read_result == sizeof(int));
	
	/* increase the semaphore free space */
	RET_IF_BAD_CLEAN(0 == sem_post(&queue->sem_free_space), FAIL, "sem_post(sem_free_space) FAILED!", WQDestroy(queue));	
		
	/* insert value to the output param */
	*output = *value;
	
	/* reutrn SUCCESS */
	return SUCCESS;
}
