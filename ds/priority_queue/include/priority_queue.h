#ifndef __ILRD_PRIORITY_QUEUE_H__
#define __ILRD_PRIORITY_QUEUE_H__

/************************************includes************************************/
#include <stddef.h> /* size_t */

/************************************typedef************************************/
typedef struct pq priority_queue_t;

/*
	DESCRIPTION: compare between 2 arguments with a reference to external param
	RETURNS: negative value when data1 is smaller (or earlier) than data2 0 when they are equal, or positive when data 1 is bigger (or later) than data2.
	ARGUMENTS:
		@param1 - const void* data1
		@param2 - const void* data2
		@param3 - void* param - can be used to reference an external data source.
	NOTES:
		data1 is the item from the queue to compare against. (during insert it will be iterated through all items until correct location was found)
		data2 is the new item inserted.
*/
typedef int (*pq_comparer_t)(const void* data1, const void* data2, void* param);

/* 
	RETURNS TRUE/FALSE 
*/
typedef int (*pq_is_match_t)(const void* data1, const void* data2);

/************************************Functions************************************/
/*
	DESCRIPTION: Creates a new priority queue
	RETURNS: priority_queue_t* - a pointer to a new empty priority queue or NULL on error.
	ARGUMENTS: 
		@arg1 - pq_comparer_t comparer - the list will be sorted according to this function.
		@arg2 - void* param - can be used to reference an external data source.
	LIMITATIONS: (none)
	NOTES: (none)
	COMPLEXITY: 
		Time: O(1)
*/
priority_queue_t* PQCreate(pq_comparer_t comparer, void* param);

/*
	DESCRIPTION: Destroys the priority queue and frees everything inside of it
	RETURNS: (nothing)
	ARGUMENTS:
		@arg1 - priority_queue_t* pq - our priority queue
	LIMITATIONS: (none)
	NOTES: 
	COMPLEXITY: 
		Time: O(n)
*/
void PQDestroy(priority_queue_t* pq);

/*
	DESCRIPTION: adds new element to the priority queue.
	RETURNS: 0 on SUCCESS and 1 on FAILURE.
	ARGUMENTS:
		@arg1 - priority_queue_t* pq.
		@arg2 - void* data.
	LIMITATIONS:
	COMPLEXITY: 
 	   Time: O(n)
*/
int PQEnqueue(priority_queue_t* pq, void* data);

/*
	DESCRIPTION: removes first element in the priority queue.
	RETURNS: (nothing)
	ARGUMENTS:
		@arg1 - priority_queue_t* pq.
	LIMITATIONS: when queue is empty, does nothing.
	COMPLEXITY: 
		Time: O(1)
*/
void PQDequeue(priority_queue_t* pq);

/*
	DESCRIPTION: returns first element in the priority queue.
	RETURNS: void* - first element in the  priority queue
	ARGUMENTS:
		@arg1 - const priority_queue_t* pq.
	LIMITATIONS: when queue is empty, returns NULL.
	COMPLEXITY: 
		Time: O(1)
*/
void* PQPeek(const priority_queue_t* pq);

/*
	DESCRIPTION: checks whether the priority queue is empty.
	RETURNS: 1 on TRUE and 0 on FALSE
	ARGUMENTS:
		@arg1 - const priority_queue_t* pq.
	LIMITATIONS:
	COMPLEXITY: 
		Time: O(1)
*/
int PQIsEmpty(const priority_queue_t* pq);

/*
	DESCRIPTION: returns size of the priority queue
	RETURNS: number of elements in the priority queue
	ARGUMENTS:
		@arg1 - const priority_queue_t* pq.
	LIMITATIONS:
	COMPLEXITY: 
		Time: O(n)
*/
size_t PQSize(const priority_queue_t* pq);

/*
	DESCRIPTION: Removes all elements in the priority queue
	RETURNS: (nothing)
	ARGUMENTS:
		@arg1 - priority_queue_t* pq - our priority queue
	LIMITATIONS: when queue is empty, does nothing.
	NOTES: 
	COMPLEXITY: 
		Time: O(n)
*/
void PQClear(priority_queue_t* pq);

/*
	DESCRIPTION: Removes a specific element from the priority queue
	RETURNS: the data we found, NULL if not found
	ARGUMENTS:
		@arg1 - priority_queue_t* pq - our priority queue
		@arg2 - void* data - data for the element we want to remove
	LIMITATIONS: if there isn't an element with the received data, does nothing.
	NOTES: 
	COMPLEXITY: 
		Time: O(n)
*/
void* PQErase(priority_queue_t* pq, pq_is_match_t is_match, void* data);

#endif /* __ILRD_PRIORITY_QUEUE_H__ */
