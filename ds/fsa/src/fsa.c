/**
	Written By: Tom Golzman
	Date: 06/05/2025
	Reviewed By: Sami
**/

/************************************includes************************************/
#include <assert.h> /* assert */

#include "fsa.h"

/************************************define************************************/
#define WORD_SIZE (sizeof(size_t))
#define SIZE_T_MAX (~(size_t)0)

/************************************typedef************************************/
struct fsa
{
	size_t free_list_head_offset;
};

/************************************Private Functions Forward Decleration************************************/
static size_t AlignToWordSize(size_t size);

/************************************Functions************************************/
fsa_t* FSAInit(void* pool, size_t size, size_t block_size)
{
	size_t i = 0;
	size_t aligned_block_size = 0;
	size_t meta_data_size = AlignToWordSize(sizeof(fsa_t));
	size_t num_of_blocks = 0;
	size_t next_offset = 0;
	char* block = NULL;
	fsa_t* fsa = NULL;
	
	assert(NULL != pool);
	assert(block_size >= FSASuggestSize(0, block_size));
	
	aligned_block_size = AlignToWordSize(block_size);
	num_of_blocks = (size - meta_data_size) / aligned_block_size;
	
	fsa = (fsa_t*)pool;
	fsa->free_list_head_offset = meta_data_size;
	
	for (i = 0; i < num_of_blocks; i++)
	{
		block = (char*)pool + meta_data_size + (i * aligned_block_size);
		if (i + 1 < num_of_blocks)
		{
			next_offset = meta_data_size + ((i + 1) * aligned_block_size);
		}
		else
		{
			next_offset = 0;
		}
		
		*((size_t*)block) = next_offset;
	}
	
	return (fsa);
}

void* FSAMalloc(fsa_t* allocator)
{
	size_t offset = 0;
	char* pool_start = NULL;
	char* block = NULL;

	assert(NULL != allocator);

	offset = allocator->free_list_head_offset;
	if (0 == offset)
	{
		return (NULL);
	}
	
	pool_start = (char*)allocator;
	block = pool_start + offset;
	
	allocator->free_list_head_offset = *((size_t*)block);
	
	return (block);
}

void FSAFree(fsa_t* allocator, void* block_to_free)
{
	size_t offset = 0;
	char* pool_start = NULL;
	char* block = NULL;

	assert(NULL != allocator);
	assert(NULL != block_to_free);
	
	pool_start = (char*)allocator;
	block = (char*)block_to_free;
	offset = block - pool_start;
	
	*((size_t*)block) = allocator->free_list_head_offset;
	allocator->free_list_head_offset = offset;
}

size_t FSACountFreeBlocks(const fsa_t* allocator)
{
	size_t counter = 0;
	size_t offset = 0;
	char* pool_start = NULL;
	char* block = NULL;
	
	assert(NULL != allocator);
	
	offset = allocator->free_list_head_offset;
	pool_start = (char*)allocator;
	
	while (offset != 0)
	{
		block = pool_start + offset;
		offset = *((size_t*)block);
		counter++;
	}
	
	return (counter);
}

size_t FSASuggestSize(size_t num_of_blocks, size_t block_size)
{
	size_t meta_data_size = AlignToWordSize(sizeof(fsa_t));
	size_t aligned_block_size = AlignToWordSize(block_size);
	
	if (num_of_blocks > ((SIZE_T_MAX - meta_data_size) / aligned_block_size))
	{
		return (0);
	}
		
	return ((num_of_blocks * aligned_block_size) + meta_data_size);
}

/************************************Private Functions************************************/
static size_t AlignToWordSize(size_t size)
{
	if (size > 0)
	{
		return (((size + WORD_SIZE - 1) / WORD_SIZE) * WORD_SIZE);
	}
	
	return (WORD_SIZE);
}
