/**
	Written By: Tom Golzman
	Date: 08/04/2025
	Reviewed By: Sami
**/

/************************************includes************************************/
#include <stddef.h> /* offsetof */
#include <stdlib.h> /* malloc , size_t */
#include <assert.h> /* assert */
#include <stdio.h> /* printf for PrintCharBuffer() */

/************************************define************************************/
#define MIN(a,b) ((a) < (b) ? (a) : (b))

/************************************typedef************************************/
typedef struct circular_buffer{
	size_t size;
	size_t read;
	size_t capacity;
	char buffer[1];
} cb_t;

/************************************Functions************************************/
cb_t* CBCreate(size_t capacity)
{
	cb_t* c_buffer = (cb_t*)malloc(offsetof(cb_t, buffer) + capacity);
	
	assert(NULL != c_buffer);
	
	c_buffer->size = 0;
	c_buffer->capacity = capacity;
	c_buffer->read = 0;
	
	return (c_buffer);
}

void CBDestroy(cb_t* cb)
{
	assert(NULL != cb);
	
	free(cb);
	cb = NULL;
}

size_t CBWrite(cb_t* dest, const void* src, size_t n)
{
	size_t i = 0;
	size_t count = 0;
	size_t write_index = 0;
	size_t avalible = dest->capacity - dest->size;
	
	assert(NULL != src);
	assert(NULL != dest);
	
	n = MIN(avalible, n);
	
	for (i = 0; i < n; i++)
	{
		write_index = (dest->read + dest->size) % dest->capacity;
		dest->buffer[write_index] = ((char*)src)[i];
		dest->size++;
		count++;
	}
	
	return (count);
}

size_t CBRead(cb_t* src, void* dest, size_t n)
{
	size_t i = 0;
	size_t count = 0;
	size_t read_index = 0;

	assert(NULL != src);
	assert(NULL != dest);	
	
	n = MIN(src->size, n);

	for (i = 0; i < n; i++)
	{
		read_index = (src->read + i) % src->capacity;
		((char*)dest)[i] = src->buffer[read_index];
		count++;
	}
	
	src->read = (src->read + count) % src->capacity;
	src->size = src->size - count;
	
	return (count);
}

size_t CBGetSize(const cb_t* cb)
{
	assert(NULL != cb);

	return (cb->size);
}

size_t CBGetCapacity(const cb_t* cb)
{
	assert(NULL != cb);

	return (cb->capacity);
}

void PrintCharBuffer(cb_t* cb, size_t size)
{
	size_t i = 0;

	assert(NULL != cb);
	
	for (i = 0; i < size; i++)
	{
		printf("%c ", cb->buffer[i]);
	}
	printf("\n");
}
