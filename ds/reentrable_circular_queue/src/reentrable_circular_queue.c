/**
	Written By: Tom Golzman
	Date: 07/07/2025
	Reviewed By: 
**/

/************************************includes************************************/
#include <assert.h>		/* assert() */
#include <stdlib.h>		/* calloc(), free() */
#include <pthread.h>	/* pthread_t */
#include <semaphore.h>	/* sem_t */

#include "utils.h"		/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */
#include "reentrable_circular_queue.h"

/************************************define************************************/
struct reentrable_circular_queue
{
	sem_t free_space_counter;
	sem_t used_space_counter;
	pthread_mutex_t read_lock;
	pthread_mutex_t write_lock;
	volatile const int* read_head;
	volatile int* write_head;
	int queue[1];
};

/************************************Functions************************************/
rcq_t* RCQCreate(size_t capacity)
{
	int status = 0;
	rcq_t* rcq = NULL;
	
	assert(capacity > 0);
	
	/* allocate memory for the struct */
	rcq = (rcq_t*)calloc(1, offsetof(rcq_t, queue) + sizeof(int) * capacity);
	RET_IF_BAD(NULL != rcq, NULL, "RCQCreate: calloc() FAILED!");
	
	/* intialize mutex in the struct */
	status = pthread_mutex_init(&(rcq->read_lock), NULL);
	RET_IF_BAD(0 == status, NULL, "RCQCreate: pthread_mutex_init(read_lock) FAILED!");
	
	status = pthread_mutex_init(&(rcq->write_lock), NULL);
	RET_IF_BAD(0 == status, NULL, "RCQCreate: pthread_mutex_init(write_lock) FAILED!");
	
	/* initialize semaphores in the struct */
	status = sem_init(&(rcq->free_space_counter), 0, capacity);
	RET_IF_BAD(0 == status, NULL, "RCQCreate: sem_init(free_space_counter) FAILED!");
	
	status = sem_init(&(rcq->used_space_counter), 0, 0);
	RET_IF_BAD(0 == status, NULL, "RCQCreate: sem_init(used_space_counter) FAILED!");

	/* assign read & write pointers in the struct */
	rcq->read_head = rcq->queue;
	rcq->write_head = rcq->queue;
	
	/* return the new rcq */
	return rcq;
}

void RCQDestroy(rcq_t* rcq)
{
	if (NULL == rcq)
	{
		return;
	}
	
	/* destroy mutex in the rcq */
	RET_IF_BAD(0 == pthread_mutex_destroy(&(rcq->read_lock)), NOTHING, "RCQDestroy: pthread_mutex_destroy(read_lock) FAILED!");
	RET_IF_BAD(0 == pthread_mutex_destroy(&(rcq->write_lock)), NOTHING, "RCQDestroy: pthread_mutex_destroy(write_lock) FAILED!");
	
	/* destroy semaphores in the rcq */
	RET_IF_BAD(0 == sem_destroy(&(rcq->free_space_counter)), NOTHING, "RCQDestroy: sem_destroy(free_space_counter) FAILED!");
	RET_IF_BAD(0 == sem_destroy(&(rcq->used_space_counter)), NOTHING, "RCQDestroy: sem_destroy(used_space_counter) FAILED!");
	
	/* for debug - put BAD_MEM in the pointers */
	DEBUG_ONLY
	(
		rcq->read_head = BAD_MEM(int*);
		rcq->write_head = BAD_MEM(int*);
	);
	
	/* free the rcq */
	free(rcq);
}

int RCQEnqueue(rcq_t* rcq, int data)
{
	int status = 0;
	volatile int* curr_write_head = NULL;
	
	/* assert */
	assert(NULL != rcq);
	
	/* wait for free_space semaphore */
	status = sem_wait(&(rcq->free_space_counter));
	RET_IF_BAD(0 == status, FAIL, "RCQEnqueue: sem_wait(free_space) FAILED!");
	
	/* lock write mutex */
	status = pthread_mutex_lock(&(rcq->write_lock));
	RET_IF_BAD(0 == status, FAIL, "RCQEnqueue: pthread_mutex_lock(write_lock) FAILED!");
	
	/* copy write head to a local pointer */
	curr_write_head = rcq->write_head;
	
	/* increase write head by 1 */
	++(rcq->write_head);
	
	/* if the write head reached the end of the queue */
	if ((rcq_t*)rcq->write_head == rcq + 1)
	{
		/* write head points to the start of the queue */
		rcq->write_head = rcq->queue;
	}
	
	/* unlock write mutex */
	status = pthread_mutex_unlock(&(rcq->write_lock));
	RET_IF_BAD(0 == status, FAIL, "RCQEnqueue: pthread_mutex_unlock(write_lock) FAILED!");
	
	/* insert data to the queue */
	*curr_write_head = data;
	
	/* increment the used semaphore by 1 */
	status = sem_post(&(rcq->used_space_counter));
	RET_IF_BAD(0 == status, FAIL, "RCQEnqueue: sem_post(used_space) FAILED!");
	
	/* return SUCCESS */
	return SUCCESS;
}

int RCQDequeue(rcq_t* rcq, int* output_data)
{
	int status = 0;
	volatile const int* curr_read_head = NULL;
		
	/* assert */
	assert(NULL != rcq);

	/* wait for used_space semaphore */
	status = sem_wait(&(rcq->used_space_counter));
	RET_IF_BAD(0 == status, FAIL, "RCQDequeue: sem_wait(used_space) FAILED!");

	/* lock read mutex */
	status = pthread_mutex_lock(&(rcq->read_lock));
	RET_IF_BAD(0 == status, FAIL, "RCQDequeue: pthread_mutex_lock(read_lock) FAILED!");
	
	/* copy read head to a local pointer */
	curr_read_head = rcq->read_head;
	
	/* increase read head by 1 */
	++(rcq->read_head);
	
	/* if the read head reached the end of the queue */
	if ((rcq_t*)rcq->read_head == rcq + 1)
	{
		/* read head points to the start of the queue */
		rcq->read_head = rcq->queue;
	}
	
	/* unlock read mutex */
	status = pthread_mutex_unlock(&(rcq->read_lock));
	RET_IF_BAD(0 == status, FAIL, "RCQDequeue: pthread_mutex_unlock(read_lock) FAILED!");
	
	/* assign the data from the queue to output data */
	*output_data = *curr_read_head;
	
	/* increment the free semaphore by 1 */
	status = sem_post(&(rcq->free_space_counter));
	RET_IF_BAD(0 == status, FAIL, "RCQDequeue: sem_post(free_space) FAILED!");
	
	/* return SUCCESS */
	return SUCCESS;
}
