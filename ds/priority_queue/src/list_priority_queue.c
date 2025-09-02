/**
	Written By: Tom Golzman
	Date: 16/04/2025
**/

#include <stdlib.h>
#include <assert.h>

#include "oll.h"
#include "priority_queue.h"
#include "utils.h"

struct pq
{
    oll_t* olist;
};

priority_queue_t* PQCreate(pq_comparer_t comparer, void* param)
{
	priority_queue_t* pq = (priority_queue_t*)malloc(sizeof(priority_queue_t));
	if (NULL == pq)
	{
		return (NULL);
	}

	assert(NULL != comparer);
	
	pq->olist = OListCreate(comparer, param);
	if (NULL == pq->olist)
	{
		free(pq);
		pq = NULL;
		
		return (NULL);
	}
	
	return (pq);
}

void PQDestroy(priority_queue_t* pq)
{
	if (NULL == pq)
	{
		return;
	}

	OListDestroy(pq->olist);
	pq->olist = NULL;
	
	free(pq);
	pq = NULL;
}

int PQEnqueue(priority_queue_t* pq, void* data)
{
	oll_iter_t iter;
	
	assert(NULL != pq);
	
	iter = OListInsert(pq->olist, data);
	
	if (OListIsSameIter(iter, OListEnd(pq->olist)))
	{
		return (FAIL);
	}
	
	return (SUCCESS);
}

void PQDequeue(priority_queue_t* pq)
{
	assert(NULL != pq);
	assert(!PQIsEmpty(pq));
	
	OListPopFront(pq->olist);	
}

void* PQPeek(const priority_queue_t* pq)
{
	oll_iter_t iter;

	assert(NULL != pq);

	iter = OListBegin(pq->olist);
	
	if (OListIsSameIter(iter, OListEnd(pq->olist)))
	{
		return (NULL);
	}
	
	return (OListGetData(iter));	
}

int PQIsEmpty(const priority_queue_t* pq)
{
	assert(NULL != pq);
	
	return (OListIsEmpty(pq->olist));
}

size_t PQSize(const priority_queue_t* pq)
{
	assert(NULL != pq);
	
	return (OListSize(pq->olist));
}

void PQClear(priority_queue_t* pq)
{
	
	assert(NULL != pq);
	
	while (!PQIsEmpty(pq))
	{
		PQDequeue(pq);
	}
} 

void* PQErase(priority_queue_t* pq, pq_is_match_t is_match, void* data)
{
	void* ret_data = NULL;
	oll_iter_t from;
	oll_iter_t to;
	oll_iter_t found;
	
	assert(NULL != pq);
	assert(NULL != is_match);
	
	from = OListBegin(pq->olist);
	to = OListEnd(pq->olist);
	found = OListFindIf(from, to, is_match, data);
	
	if (!OListIsSameIter(found, to))
	{
		ret_data = OListGetData(found);
		OListRemove(found);
	}
	
	return (ret_data);
} 