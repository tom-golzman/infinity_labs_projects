/**
	Written By: Tom Golzman
	Date: 08/05/2025
	Reviewed By: Amir Granot
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
static size_t GetParent(size_t index);
static size_t GetLeftChild(size_t index);
static size_t GetRightChild(size_t index);
static size_t Min(const priority_queue_t* pq, size_t index1, size_t index2);
static void HeapifyUp(priority_queue_t* pq, size_t curr_elem_index);
static void HeapifyDown(priority_queue_t* pq, size_t curr_elem_index);
static void Swap(priority_queue_t* pq, size_t index1, size_t index2);
static size_t FindElemIndexInVector(const priority_queue_t* pq, pq_is_match_t is_match, void* data);
static size_t GetLastElemIndex(const priority_queue_t* pq);
static void** GetBase(const priority_queue_t* pq);
static void UpdateChildren(size_t curr, size_t* left_child, size_t* right_child);

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
	
	last_elem_index = GetLastElemIndex(pq);
	
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
static size_t GetParent(size_t index)
{
	assert(index > 0);

	return ((index - 1) / 2);
}

static size_t GetLeftChild(size_t index)
{
	return ((index * 2) + 1);
}

static size_t GetRightChild(size_t index)
{
	return ((index * 2) + 2);
}

static size_t Min(const priority_queue_t* pq, size_t index1, size_t index2)
{
	void** base = NULL;
	
	assert(NULL != pq);

	base = GetBase(pq);
	
	if (index1 >= PQSize(pq))
	{
		return (index2);
	}
	
	if (index2 >= PQSize(pq))
	{
		return (index1);
	}
	
	if (pq->comparer(base[index1], base[index2], pq->comparer_param) > 0)
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
	void** base = NULL;

	assert(NULL != pq);

	if (curr_elem_index == FIRST_ELEM_INDEX)
	{
		return;
	}
		
	parent_index = GetParent(curr_elem_index);
	base = GetBase(pq);
	
	curr = base[curr_elem_index];
	parent = base[parent_index];

	while (pq->comparer(curr, parent, pq->comparer_param) < 0)
	{
		Swap(pq, curr_elem_index, parent_index);
		
		curr_elem_index = parent_index;		
		if (curr_elem_index == FIRST_ELEM_INDEX)
		{
			return;
		}

		parent_index = GetParent(curr_elem_index);

		curr = base[curr_elem_index];
		parent = base[parent_index];
	}
}

static void HeapifyDown(priority_queue_t* pq, size_t curr_elem_index)
{
	void** base = NULL;
	size_t left_child_index = 0;
	size_t right_child_index = 0;
	size_t min_child_index = 0;
	
	assert(NULL != pq);
	
	base = GetBase(pq);
	
	UpdateChildren(curr_elem_index, &left_child_index, &right_child_index);
	if (left_child_index >= PQSize(pq) || right_child_index >= PQSize(pq))
	{
		return;
	}
	
	min_child_index = Min(pq, left_child_index, right_child_index);
	
	/* while curr_elem is samller than the biggest child */
	while (pq->comparer(base[curr_elem_index], base[min_child_index], pq->comparer_param) > 0)
	{
		/* swap with the largest child */
		Swap(pq, curr_elem_index, min_child_index);
		curr_elem_index = min_child_index;
		
		/* update children of the new curr */
		UpdateChildren(curr_elem_index, &left_child_index, &right_child_index);
		if (left_child_index >= PQSize(pq) || right_child_index >= PQSize(pq))
		{
			return;
		}
		
		min_child_index = Min(pq, left_child_index, right_child_index);		
	}
}

static void UpdateChildren(size_t curr, size_t* left_child, size_t* right_child)
{
	assert(NULL != left_child);
	assert(NULL != right_child);
	
	*left_child = GetLeftChild(curr);
	*right_child = GetRightChild(curr);
}

static void Swap(priority_queue_t* pq, size_t index1, size_t index2)
{
	void** base = NULL;
	void* temp = NULL;
	
	assert(NULL != pq);

	base = GetBase(pq);

	temp = base[index1];
	base[index1] = base[index2];
	base[index2] = temp;	
}

static size_t FindElemIndexInVector(const priority_queue_t* pq, pq_is_match_t is_match, void* data)
{
	size_t index = FIRST_ELEM_INDEX;
	void* curr_elem = NULL;
	void** base = NULL;
		
	/* assert */
	assert(NULL != pq);
	assert(NULL != is_match);
	assert(NULL != data);
	
	base = GetBase(pq);
	
	curr_elem = base[FIRST_ELEM_INDEX];
	
	/* iterate on the vector until find the data or until the end */	
	while (index < PQSize(pq) - 1 && !is_match(curr_elem, data))
	{
		++index;
		curr_elem = base[index];		
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

static void** GetBase(const priority_queue_t* pq)
{
	assert(NULL != pq);
	
	return ((void**)VectorGetAccessToElem(pq->vector, FIRST_ELEM_INDEX));
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
