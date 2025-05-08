#ifndef __ILRD_FSA_H__
#define __ILRD_FSA_H__

/************************************includes************************************/
#include <stddef.h> /* size_t */

/************************************typedef************************************/
typedef struct fsa fsa_t;

/************************************Functions************************************/
/*
	DESCRIPTION: Initialize a preallocated memory to a memory pool (allocator)
	PARAMETERS: 
			- void* pool - address of preallocated memory that will serve as memory pool.
			- size_t size - the size of the pool.
			- size_t block_size - size of block (chunk) to split the pool into.
	RETURNS: fsa_t* - pointer to fixed size allocator object
	COMPLEXITY: O(n)
	LIMITATIONS: Size must be greater than or equal (>=) to FSASuggestSize(0, block_size)
	NOTES: Size must be greater or equal to result of FSASuggestSize() to guarantee the amount of blocks created
*/
fsa_t* FSAInit(void* pool, size_t size, size_t block_size);

/*
	DESCRIPTION: Allocate a single block of memory from the pool.
	PARAMETERS: fsa_t* allocator - memory pool to allocate block from.
	RETURNS: void* - address of the block to be used by the user.
	COMPLEXITY: O(1)
	LIMITATIONS: Doing allocation when there are no free blocks left is undefined. check with FSACountFreeBlocks().
	NOTES: The allocated block is alligned to word size.
*/
void* FSAMalloc(fsa_t* allocator);

/*
	DESCRIPTION: Free block that was allocated from the specified allocator (memory pool), return it back to the pool.
	PARAMETERS: 
			- fsa_t* allocator - that was initialized with Init()
			- void* block_to_free - a pointer to the block to free
	RETURNS: void
	COMPLEXITY: O(1)
	LIMITATIONS: Attempting to free a block that was not allocated from the specified allocator has undefined behavior.
*/
void FSAFree(fsa_t* allocator, void* block_to_free);

/*
	DESCRIPTION: Get the amount of free blocks available for allocation.
	PARAMETERS: fsa_t* allocator - that was initialized with Init().
	RETURNS: size_t
	COMPLEXITY: O(n)
	LIMITATIONS: None
*/
size_t FSACountFreeBlocks(const fsa_t* allocator);

/*
	DESCRIPTION: Calculate the required size to allocate by the user for a memory pool that will serve its requirements of size and quantity.
	PARAMETERS:
			- size_t num_of_blocks - amount of blocks the pool should be able to serve.
			- size_t block_size - amount of bytes each block should have.
	RETURNS:
			- size_t - minimum size of bytes required to build a pool that can contain num_of_block block, each the size of block_size.
			- positive number on SUCCESS or 0 on error. see LIMITATIONS.
	COMPLEXITY: O(1)
	LIMITATIONS: There is an error when the requested size exceeds the limit of available space (beyond MAX of size_t).
*/
size_t FSASuggestSize(size_t num_of_blocks, size_t block_size);

#endif /*__ILRD_FSA_H__*/
