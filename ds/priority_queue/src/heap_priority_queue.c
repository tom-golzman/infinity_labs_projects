/**
	Written By: Tom Golzman
	Date: 08/05/2025
	Reviewed By:
**/

/************************************includes************************************/
#include <stdlib.h> 		/* malloc */
#include <assert.h>			/* assert */

#include "utils.h"			/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM() */
#include "dynamic_vector.h" /* dvector_t */
#include "priority_queue.h" /* priority_queue_t */

/************************************define************************************/
#define DEFAULT_VECTOR_SIZE (0)

/************************************typedef************************************/
struct pq
{
    dvector_t* vector;
    pq_comparer_t comparer;
    void* comparer_param;
};

/************************************Public Functions************************************/
priority_queue_t* PQCreate(pq_comparer_t comparer, void* param)
{
	priority_queue_t* pq = NULL;
	priority_queue_t* dummy = NULL;
	
	/* assert */
	assert(NULL != comparer);
	
	/* allocate memory of pq */
	pq = (priority_queue_t*)calloc(sizeof(priority_queue_t));
	if (NULL == pq)
	{
		return (NULL);
	}
	
	/* create dynamic vector */
	pq->vector = VectorCreate(DEFAULT_VECTOR_SIZE, sizeof(void*));
	if (NULL == pq->vector)
	{
		free(pq);
		pq = NULL;
		
		return (NULL);
	}
	
	pq->comparer = comparer;
	pq->comparer_param = param;
	
	dummy = pq;
	PQEnqueue(pq, dummy);
	
	return (pq);
}

void PQDestroy(priority_queue_t* pq)
{
	/* handle NULL */
	if (NULL == pq)
	{
		return;
	}
	
	/* destroy the dynamic vector */
	VectorDestroy(pq->vector);
	
	/* free the pq */
	DEBUG_ONLY(
		pq->vector = BAD_MEM64(dvector_t*);
		pq->comparer = BAD_MEM64(pq_comparer_t);
		pq->comparer_param = BAD_MEM64(void*);
	);
	
	free(pq);
}

int PQEnqueue(priority_queue_t* pq, void* data)
{
	void* new_elem = NULL;
	int push_status = FAIL;
	
	/* assert */
	assert(NULL != pq);
	assert(NULL != data);
	
	/* PushBack to the vector */
	push_status = VectorPushBack(pq->vector, data);
	/* handle failure */
	if (SUCCESS != push_status)
	{
		return (push_status);
	}
	
	new_elem = VectorGetAccessToElem(pq->vector, VectorSize(pq->vector));
		
	/* HeapifyUp() */
	HeapifyUp(pq, new_elem);
}

void PQDequeue(priority_queue_t* pq)
{
	void* last_elem = NULL;
	void* parent = NULL;
	
	/* assert */
	assert(NULL != pq);
	
	/* copy the last element of the vector in the beginning */
	last_elem = VectorGetAccessToElem(pq->vector, VectorSize(pq->vector));
	parent = GetParent(last_elem);
	
	/* PopBack to the vector */
	VectorPopBack(pq->vector);

	/* while now the biggest of top3 */
	while (last_elem == Max(pq, last_elem, parent))
	{
		/* swap with the largest child */
		Swap(last_elem, parent);
		parent = GetParent(last_elem);
	}
}

void* PQPeek(const priority_queue_t* pq)
{
	/* assert */
	assert(NULL != pq);
	
	/* return the element in index 1 */
	return (VectorGetAccessToElem(pq->vector, 1));
}

size_t PQSize(const priority_queue_t* pq)
{
	/* assert */
	assert(NULL != pq);
	
	/* return VectorSize */
	return (VectorSize(pq->vector));
}

int PQIsEmpty(const priority_queue_t* pq)
{
	/* assert */
	assert(NULL != pq);
	
	/* return if the size is one (only dummy) */
	return (1 == VectorSize(pq->vector));
}

void PQClear(priority_queue_t* pq)
{
	/* assert */
	assert(NULL != pq);
	
	while (!PQIsEmpty(pq))
	{
		PQDequeue(pq);
	}
}

void* PQErase(priority_queue_t* pq, pq_is_match_t is_match, void* data)
{
	/* assert */
	/* find the index of the data in the vector */
	/* save the data and PQDequeue() */
}

/************************************Private Functions************************************/
static void* GetParent(dvector_t* vector, size_t index)
{
	size_t parent = index / 2;

	assert(NULL != vector);
	assert(index > 0);
	
	if (index == 1)
	{
		return (0);
	}
	
	return (VectorGetAccessToElem(vector, parent));
}

static void* GetLeftChild(dvector_t* vector, size_t index)
{
	size_t child = index * 2;
	
	assert(NULL != vector);
	assert(index > 0);
	
	if (child > VectorSize(vector))
	{
		return (0);
	}
	
	return (VectorGetAccessToElem(vector, child));
}

static void* GetRightChild(dvector_t* vector, size_t index)
{
	size_t child = (index * 2) + 1;
	
	assert(NULL != vector);
	assert(index > 0);
	
	if (child > VectorSize(vector))
	{
		return (0);
	}
	
	return (VectorGetAccessToElem(vector, child));
}

static void* Max(priority_queue_t* pq, void* elem1, void* elem2)
{
	void* max = elem1;

	assert(NULL != pq);
	assert(NULL != elem1);
	assert(NULL != elem2);
	
	if (pq->comparer(max, elem2, pq->comparer_param) < 0)
	{
		max = elem2;
	}

	return (max);
}

static void HeapifyUp(priority_queue_t* pq, void* new_elem)
{
	void* parent = NULL;

	assert(NULL != pq);
	assert(NULL != new_elem);

	parent = GetParent(new_elem);
	
	/* while the new element is bigger than the parent */
	while (new_elem == Max(pq, new_elem, parent))
	{
		/* swap with the parent */
		Swap(new_elem, parent);
		
	}
}

static void Swap(void* elem1, void* elem2)
{
	size_t elem_size = sizeof(void*); 
	char temp_buffer[elem_size];
	
	memcpy(temp_buffer, elem1, elem_size);
	memcpy(elem1, elem2, elem_size);
	memcpy(elem2, temp_buffer, elem_size);
}
