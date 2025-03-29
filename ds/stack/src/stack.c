/**
	Written By: Tom Golzman
	Date: 26/02/2025
	Reviewed By: Sami 
**/

/*----------------------includes----------------------*/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free*/
#include <string.h> /* memcpy */

/*----------------------define----------------------*/
#define TRUE 1
#define FALSE 0

/*---------------------typedef---------------------*/
typedef struct {
    void* base_ptr;
    void* stack_ptr;
    size_t capacity;
    size_t elem_size;
} stack_internal_t;

typedef stack_internal_t stack_t;

/*-------------------------functions-------------------------*/
stack_t* Create(size_t capacity, size_t elem_size)
{
	stack_internal_t* stack = NULL;
	void* base = NULL;
	
	stack = (stack_internal_t*)malloc(sizeof(stack_internal_t));
	if (NULL == stack)
	{
		return (NULL);
	}
	
	base = (void*)malloc(capacity * elem_size);
	if (NULL == base)
	{
		free (stack);
		
		return (NULL);
	}
	
	stack->base_ptr = base;
	stack->stack_ptr = base;
	stack->capacity = capacity;
	stack->elem_size = elem_size;
	
	return (stack);
}

void Push(stack_t* s, void* data)
{
	assert(NULL != s);
	
	memcpy(s->stack_ptr, data, s->elem_size);
	s->stack_ptr = (char*)s->stack_ptr + s->elem_size; 
}

void* Pop(stack_t* s)
{
	assert(NULL != s);
	
	s->stack_ptr = (char*)s->stack_ptr - s->elem_size;
	
	return (s->stack_ptr);
}

void* Peek(const stack_t* s)
{
	void* data;

	assert(NULL != s);
	
	data = (char*)s->stack_ptr - s->elem_size;
	
	return (data);
}

int IsEmpty(const stack_t* s)
{
	assert(NULL != s);
	
	if (s->stack_ptr == s->base_ptr)
	{
		return (TRUE);
	}
	
	return (FALSE);
}

size_t Count(const stack_t* s)
{
	size_t diff = 0;
	
	assert(NULL != s);
	
	diff = ((char*)(s->stack_ptr) - (char*)(s->base_ptr)) / s->elem_size;
	
	return (diff);
}

int IsFull(const stack_t* s)
{
	assert(NULL != s);

	if (Count(s) == s->capacity)
	{
		return (TRUE);
	}
	
	return (FALSE);
}

size_t Capacity(const stack_t* s)
{
	assert(NULL != s);
	
	return (s->capacity);
}

void Destroy(stack_t* s)
{
	assert(NULL != s);
		
	free(s->base_ptr);	
	s->base_ptr = NULL;
	
	free(s);
	s = NULL;
}
