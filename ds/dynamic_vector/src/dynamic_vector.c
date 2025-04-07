/**
	Written By: Tom Golzman
	Date: 06/04/2025
	Reviewed By: Hagai Levy
**/

/************************************includes************************************/
#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */

/************************************define************************************/
#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define FAIL 1
#define GROWTH_FACTOR 2
#define DEFAULT_CAPACITY 10

/************************************typedef************************************/
typedef struct dvector{
    void* arr;
    size_t elem_size;
    size_t capacity;
    size_t size;
} dvector_t;

/************************************Functions************************************/
dvector_t* VectorCreate(size_t capacity, size_t elem_size)
{
	dvector_t* vector = NULL;
	void* arr = NULL;
	
	assert(elem_size != 0);
	
	vector = (dvector_t*)malloc(sizeof(dvector_t));
	if (NULL == vector)
	{
		return (NULL);
	}
	
	if (0 == capacity)
	{
		capacity = DEFAULT_CAPACITY;
	}
	
	arr = (void*)malloc(capacity * elem_size);
	if (NULL == arr)
	{
		free(vector);
		vector = NULL;
		
		return (NULL);
	}
	
	vector->arr = arr;
	vector->capacity = capacity;
	vector->elem_size = elem_size;
	vector->size = 0;
	
	return (vector);
}

void VectorDestroy(dvector_t* vector)
{
	assert(NULL != vector);
	
	free(vector->arr);
	vector->arr = NULL;
	
	free(vector);
	vector = NULL;
}

void* VectorGetAccessToElem(const dvector_t* vector, size_t index)
{
	char* ret_elem = NULL;

	assert(NULL != vector);
	assert(index < vector->size);
	
	ret_elem = (char*)vector->arr + (index * vector->elem_size);
	
	return ((void*)ret_elem);
}

int VectorPushBack(dvector_t* vector, void* data)
{
	void* realloc_result = NULL;

	assert(NULL != vector);
	
	if (vector->size == vector->capacity)
	{
		realloc_result = realloc(vector->arr, vector->capacity * vector->elem_size * GROWTH_FACTOR);
		if (NULL == realloc_result)
		{
			return (FAIL);
		}
		vector->arr = realloc_result;
		vector->capacity = vector->capacity * GROWTH_FACTOR;
	}
	
	memcpy(((char*)vector->arr + (vector->size * vector->elem_size)), data, vector->elem_size);
	vector->size += 1;
	
	return (SUCCESS);
}

void VectorPopBack(dvector_t* vector)
{
	assert(NULL != vector);
	
	if (vector->size > 0)
	{
		vector->size--;
	}
}

void VectorSetElem(dvector_t* vector, size_t index, void* data)
{
	assert(NULL != vector);
	assert(index < vector->size);
	
	memcpy(((char*)vector->arr + (index * vector->elem_size)), data, vector->elem_size);
}

int VectorResize(dvector_t* vector, size_t new_capacity)
{
	void* realloc_result = NULL;
		
	assert(NULL != vector);
	assert(new_capacity >= vector->size);
	
	realloc_result = realloc(vector->arr, new_capacity * vector->elem_size);
	if (NULL == realloc_result)
	{
		return (FAIL);
	}
	
	vector->arr = realloc_result;
	vector->capacity = new_capacity;

	return (SUCCESS);
}

size_t VectorCapacity(const dvector_t* vector)
{
	assert(NULL != vector);
	
	return (vector->capacity);
}

size_t VectorSize(const dvector_t* vector)
{
	assert(NULL != vector);
	
	return (vector->size);
}

size_t VectorGetElemSize(const dvector_t* vector)
{
	assert(NULL != vector);
	
	return (vector->elem_size);
}

int VectorShrinkToFit(dvector_t* vector)
{
	void* realloc_result = NULL;
	size_t new_capacity = 0;
		
	assert(NULL != vector);
	
	if (vector->size == 0)
	{
		realloc_result = realloc(vector->arr, DEFAULT_CAPACITY * vector->elem_size);
		if (NULL == realloc_result)
		{
			return (FAIL);
		}
		new_capacity = DEFAULT_CAPACITY;
	}
	
	else
	{
		realloc_result = realloc(vector->arr, vector->size * GROWTH_FACTOR * vector->elem_size);
		if (NULL == realloc_result)
		{
			return (FAIL);
		}
		new_capacity = vector->size * GROWTH_FACTOR;
	}
	
	vector->arr = realloc_result;
	vector->capacity = new_capacity; 

	return (SUCCESS);
}
