#ifndef __ILRD_VSA_H__
#define __ILRD_VSA_H__

/************************************includes************************************/
#include <stddef.h> /* size_t */

/************************************typedef************************************/
typedef struct block_header vsa_t;

/************************************Functions************************************/
/*
	DESCRIPTION: Initialize a preallocated memory (pool memory) into an allocator.
	PARAMETERS: 
			- void* pool - user allocated pool
			- size_t size - pool size in bytes
	RETURNS: vsa_t* - pointer to an allocator object. Allocator's size and address are aligned to word size.
	COMPLEXITY: O(1)
	LIMITATIONS: undefined behavior when the user send a pool that smaller than of the implementations headers.
*/
vsa_t* VSAInit(void* pool, size_t pool_size);

/*
	DESCRIPTION: Allocate requested size from the specified pool.
	PARAMETERS: 
			- vsa_t* allocator - pool to allocate memory from.
			- size_t size - requested size in bytes
	RETURNS: void* - address of the block to be used by the user, or NULL when no free blocks left.
	COMPLEXITY: O(n)
	LIMITATIONS: Undefined behavior if allocator is NULL.
	NOTE: Returns the first block that fits and not the block with the most optimal size.
*/
void* VSAMalloc(vsa_t* allocator, size_t size);

/*
	DESCRIPTION: Free block that was allocated from the specified allocator (memory pool), return it back to the pool.
	PARAMETERS: void* block - block we want to free
	RETURNS: (none)
	COMPLEXITY: O(1)
	LIMITATIONS: Attempting to free a block that was not allocated from the specified allocator has undefined behavior + undefined behaviour if block is NULL.
*/
void VSAFree(void* block);

/*
	DESCRIPTION: Returns the largest continuous block that can be given to the user
	PARAMETERS: vsa_t* allocator - pointer to the allocator object
	RETURNS: size_t - size of largest chunk that's avaliable.
	COMPLEXITY: O(n)
	LIMITATIONS: Undefined behaviour if allocator is NULL.
*/
size_t VSALargestChunkAvailable(vsa_t* allocator);

#endif /*__ILRD_VSA_H__*/
