#ifndef __ILRD_REENTRABLE_CIRCULAR_QUEUE_H__
#define __ILRD_REENTRABLE_CIRCULAR_QUEUE_H__

/************************************includes************************************/
#include <stddef.h> /* size_t */

/************************************typedef************************************/
typedef struct reentrable_circular_queue rcq_t;

/************************************Functions************************************/
/**
	@note: undefined behavior when capacity greater than SEM_VALUE_MAX
*/
rcq_t* RCQCreate(size_t capacity);

/**
	@param: queue accepts NULL
	@note: undefined behavior when destroying while waiting on enqueue or desqueue
*/
void RCQDestroy(rcq_t*);

/**
	@returns: status
	@note: pause execution until item can be enqueued (there is space for it)
	@note: can be called concurrently with RCQDequeue
*/
int RCQEnqueue(rcq_t*, int data);

/**
	@returns: status
	@note: output data of dequeued item is via out_data param.
	@note: pause execution until item can be dequeued (there are items to fetch)
	@note: can be called concurrently with RCQEnqueue
*/
int RCQDequeue(rcq_t*, int* output_data);

#endif /*__ILRD_REENTRABLE_CIRCULAR_QUEUE_H__*/
