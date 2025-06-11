/**
	Written By: Tom Golzman
	Date: 08/05/2025
	Reviewed By:
**/

/************************************includes************************************/
#include <stdlib.h> 		/* malloc */
#include <assert.h>			/* assert */
#include <string.h>			/* memcpy */

#include "utils.h"			/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM() */
#include "dynamic_vector.h" /* dvector_t */
#include "priority_queue.h" /* priority_queue_t */

/************************************define************************************/
#define DEFAULT_VECTOR_SIZE	(0)
#define FIRST_ELEM_INDEX	(0)

/************************************typedef************************************/
struct pq
{
    dvector_t* vector;
    pq_comparer_t comparer;
    void* comparer_param;
};

/************************************Private Functions************************************/
static size_t GetParent(priority_queue_t* pq, size_t index);
static size_t GetLeftChild(priority_queue_t* pq, size_t index);
static size_t GetRightChild(priority_queue_t* pq, size_t index);
static size_t Min(priority_queue_t* pq, size_t index1, size_t index2);
static void HeapifyUp(priority_queue_t* pq, size_t curr_elem_index);
static void HeapifyDown(priority_queue_t* pq, size_t curr_elem_index);
static void Swap(priority_queue_t* pq, size_t index1, size_t index2);
static size_t FindElemIndexInVector(const priority_queue_t* pq, pq_is_match_t is_match, void* data);
static size_t GetLastElemIndex(const priority_queue_t* pq);

/************************************Public Functions************************************/
priority_queue_t* PQCreate(pq_comparer_t comparer, void* param)
{
	priority_queue_t* pq = NULL;
	
	/* assert */
	assert(NULL != comparer);
	
	/* allocate memory of pq */
	pq = (priority_queue_t*)calloc(1, sizeof(priority_queue_t));
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
	size_t last_elem_index = 0;
	int push_status = FAIL;
	
	/* assert */
	assert(NULL != pq);
	assert(NULL != data);
	
	/* PushBack to the vector */
	push_status = VectorPushBack(pq->vector, &data);
	/* handle failure */
	if (SUCCESS != push_status)
	{
		return (push_status);
	}
	
	last_elem_index = GetLastElemIndex(pq);
	
	/* HeapifyUp() */
	HeapifyUp(pq, last_elem_index);
	
	return (SUCCESS);
}

void PQDequeue(priority_queue_t* pq)
{
	size_t last_elem_index = 0;
	
	/* assert */
	assert(NULL != pq);
	assert(VectorSize(pq->vector) > 0);
	
	if (PQSize(pq) == 1)
	{
		VectorPopBack(pq->vector);
		
		return;	
	}
	
	last_elem_index = PQSize(pq) - 1;
	
	/* swap the last element with the first element in the vector */	
	Swap(pq, FIRST_ELEM_INDEX, last_elem_index);
	
	/* PopBack to the vector */
	VectorPopBack(pq->vector);
	
	/* HeapifyDown() */
	HeapifyDown(pq, FIRST_ELEM_INDEX);	
}

void* PQPeek(const priority_queue_t* pq)
{
	/* assert */
	assert(NULL != pq);
	
	if (PQIsEmpty(pq))
	{
		return (NULL);
	}
	
	/* return the element in index 1 */
	return (*(void**)VectorGetAccessToElem(pq->vector, FIRST_ELEM_INDEX));
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
	
	/* return if the size is 0 */
	return (0 == VectorSize(pq->vector));
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
	size_t found_elem_index = 0;
	void* erased_data = NULL;
	
	/* assert */
	assert(NULL != pq);
	assert(NULL != is_match);
	assert(NULL != data);
	
	if (0 == PQSize(pq))
	{
		return (NULL);
	}
	
	/* find the index of the data in the vector */
	found_elem_index = FindElemIndexInVector(pq, is_match, data);
	/* if data not found */
	if (PQSize(pq) == found_elem_index)
	{
		return (NULL);
	}
	
	erased_data = *(void**)VectorGetAccessToElem(pq->vector, found_elem_index);
	
	if (found_elem_index != GetLastElemIndex(pq))
	{
		/* swap with the last element and PQDequeue() */
		Swap(pq, GetLastElemIndex(pq), found_elem_index);
	}
	
	VectorPopBack(pq->vector);

	if (PQSize(pq) > 0)
	{
		HeapifyUp(pq, found_elem_index);
		HeapifyDown(pq, found_elem_index);
	}
	
	return (erased_data);
}

/************************************Private Functions************************************/
static size_t GetParent(priority_queue_t* pq, size_t index)
{
	size_t parent = (index - 1) / 2;

	assert(NULL != pq->vector);
	
	if (index == FIRST_ELEM_INDEX)
	{
		return (PQSize(pq));
	}
	
	return (parent);
}

static size_t GetLeftChild(priority_queue_t* pq, size_t index)
{
	size_t child = (index * 2) + 1;
	
	assert(NULL != pq->vector);
	
	if (child >= PQSize(pq))
	{
		return (PQSize(pq));
	}
	
	return (child);
}

static size_t GetRightChild(priority_queue_t* pq, size_t index)
{
	size_t child = (index * 2) + 2;
	
	assert(NULL != pq->vector);
	
	if (child >= PQSize(pq))
	{
		return (PQSize(pq));
	}
	
	return (child);
}

static size_t Min(priority_queue_t* pq, size_t index1, size_t index2)
{
	assert(NULL != pq);

	if (index1 >= PQSize(pq))
	{
		return (index2);
	}
	
	if (index2 >= PQSize(pq))
	{
		return (index1);
	}
	
	if (pq->comparer(*(void**)VectorGetAccessToElem(pq->vector, index1), *(void**)VectorGetAccessToElem(pq->vector, index2), pq->comparer_param) > 0)
	{
		return (index2);
	}

	return (index1);
}

static void HeapifyUp(priority_queue_t* pq, size_t curr_elem_index)
{
	size_t parent_index = 0;
	void* curr = NULL;
	void* parent = NULL;

	assert(NULL != pq);

	parent_index = GetParent(pq, curr_elem_index);
	if (PQSize(pq) == parent_index)
	{
		return;
	}
		
	curr = *(void**)VectorGetAccessToElem(pq->vector, curr_elem_index);
	parent = *(void**)VectorGetAccessToElem(pq->vector, parent_index);

	while (pq->comparer(curr, parent, pq->comparer_param) < 0)
	{
		Swap(pq, curr_elem_index, parent_index);
		
		curr_elem_index = parent_index;
		
		parent_index = GetParent(pq, curr_elem_index);
		
		if (PQSize(pq) == parent_index)
		{
			return;
		}

		curr = *(void**)VectorGetAccessToElem(pq->vector, curr_elem_index);
		parent = *(void**)VectorGetAccessToElem(pq->vector, parent_index);
	}
}

static void HeapifyDown(priority_queue_t* pq, size_t curr_elem_index)
{
	void* curr_elem = NULL;
	void* min_child = NULL;
	size_t left_child_index = 0;
	size_t right_child_index = 0;
	size_t min_child_index = 0;
	
	assert(NULL != pq);

	curr_elem = *(void**)VectorGetAccessToElem(pq->vector, curr_elem_index);
	
	left_child_index = GetLeftChild(pq, curr_elem_index);
	if (PQSize(pq) == left_child_index)
	{
		return;
	}
	
	right_child_index = GetRightChild(pq, curr_elem_index);
	min_child_index = Min(pq, left_child_index, right_child_index);
	min_child = *(void**)VectorGetAccessToElem(pq->vector, min_child_index);
	
	/* while curr_elem is samller than the biggest child */
	while (pq->comparer(curr_elem, min_child, pq->comparer_param) > 0)
	{
		/* swap with the largest child */
		Swap(pq, curr_elem_index, min_child_index);
		curr_elem_index = min_child_index;
		
		left_child_index = GetLeftChild(pq, curr_elem_index);
		if (PQSize(pq) == left_child_index)
		{
			return;
		}
		
		right_child_index = GetRightChild(pq, curr_elem_index);
		min_child_index = Min(pq, left_child_index, right_child_index);		
	}
}

static void Swap(priority_queue_t* pq, size_t index1, size_t index2)
{
	void** elem1 = (void**)VectorGetAccessToElem(pq->vector, index1);
	void** elem2 = (void**)VectorGetAccessToElem(pq->vector, index2);
	
	void* temp = *elem1;
	*elem1 = *elem2;
	*elem2 = temp;
}

static size_t FindElemIndexInVector(const priority_queue_t* pq, pq_is_match_t is_match, void* data)
{
	size_t index = FIRST_ELEM_INDEX;
	void* curr_elem = NULL;
	
	/* assert */
	assert(NULL != pq);
	assert(NULL != is_match);
	assert(NULL != data);

	curr_elem = *(void**)VectorGetAccessToElem(pq->vector, FIRST_ELEM_INDEX);
	
	/* iterate on the vector until find the data or until the end */	
	while (index < PQSize(pq) - 1 && !is_match(curr_elem, data))
	{
		++index;
		curr_elem = *(void**)VectorGetAccessToElem(pq->vector, index);		
	}
	
	/* if data not found */
	if (!is_match(curr_elem, data))
	{
		return (PQSize(pq));
	}
	
	return (index);
}

static size_t GetLastElemIndex(const priority_queue_t* pq)
{
	assert(NULL != pq);

	return (VectorSize(pq->vector) - 1);
}

/************************************Print Functions************************************/
#ifndef NDEBUG

#include <stdio.h> /* printf */
#define INDENT_STEP (5)

static void PrintTreeRec(priority_queue_t* pq, size_t index, int indent)
{
	size_t size = VectorSize(pq->vector);
	size_t left = 2 * index + 1;
	size_t right = 2 * index + 2;

	if (index >= size)
	{
		return;
	}

	PrintTreeRec(pq, right, indent + INDENT_STEP);

	printf("%*s%d\n", indent, "", **(int**)VectorGetAccessToElem(pq->vector, index));

	PrintTreeRec(pq, left, indent + INDENT_STEP);
}

void PQPrintTree(priority_queue_t* pq)
{
	assert(NULL != pq);
	printf( "\nPriority Queue Tree (rotated):\n" );
	PrintTreeRec(pq, 0, 0);
}

#endif
