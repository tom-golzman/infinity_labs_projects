/**
	Written By: Tom Golzman
	Date: 07/04/2025
	Reviewed By: Yuval Almog
**/

/************************************includes************************************/
#include <assert.h> /* assert */
#include "sll.h"

/************************************define************************************/
#define SUCCESS 0
#define FAIL 1

/************************************typedef************************************/
typedef list_t queue_t;
typedef int (*q_is_match_t)(const void* data1, const void* data2);

static queue_t* ListToQueue(list_t* list);
static list_t* QueueToList(queue_t* queue);

/************************************Functions************************************/
queue_t* QueueCreate(void)
{
	list_t* list = ListCreate();
	
	return (ListToQueue(list));
}

int QueueEnqueue(queue_t* queue, void* data)
{
	list_t* list = NULL;
	
	assert(NULL != queue);
	
	list = QueueToList(queue);
	if (ListInsert(list, ListEnd(queue), data) == ListEnd(list))
	{
		return (FAIL);
	}
	
	return (SUCCESS);
}

void QueueDequeue(queue_t* queue)
{
	list_t* list = NULL;
	
	assert(NULL != queue);
	
	list = QueueToList(queue);	
	ListRemove(ListBegin(list));
}

void* QueuePeek(const queue_t* queue)
{
	list_t* list = NULL;
	
	assert(NULL != queue);
	
	list = QueueToList((queue_t*)queue);
	return (ListGetData(ListBegin(list)));
}

int QueueIsEmpty(const queue_t* queue)
{
	list_t* list = NULL;
	
	assert(NULL != queue);
	
	list = QueueToList((queue_t*)queue);
	return (ListIsEmpty(list));
}

size_t QueueSize(const queue_t* queue)
{
	list_t* list = NULL;
	
	assert(NULL != queue);
	
	list = QueueToList((queue_t*)queue);
	return (ListSize(list));
}

void QueueDestroy(queue_t* queue)
{
	list_t* list = NULL;
	
	assert(NULL != queue);
	
	list = QueueToList(queue);
	ListDestroy(list);
	queue = NULL;
}

void* QueueFind(const queue_t* queue, void* data, q_is_match_t is_match)
{
	list_t* list = NULL;
	iter_t iter;
	
	assert(NULL != queue);
	
	list = QueueToList((queue_t*)queue);	
	iter = ListFind(ListBegin(list), ListEnd(list), is_match, data);
	
	if (IterIsEqual(ListEnd(list), data))
	{
		return (NULL);
	}
	
	return (ListGetData(iter));
}

static queue_t* ListToQueue(list_t* list)
{
	return ((queue_t*)list);
}

static list_t* QueueToList(queue_t* queue)
{
	return ((list_t*)queue);
}
