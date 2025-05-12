/**
	Written By: Tom Golzman
	Date: 08/05/2025
	Reviewed By: Sami
**/

/************************************includes************************************/
#include <assert.h> /* assert */

#include "vsa.h"

/*************************************define*************************************/
#define WORD_SIZE (sizeof(size_t))
#define MAGIC_NUMBER ((size_t)0xDEADBEEFDEADBEEF)
#define IS_USED_MASK ((size_t)1)
#define FREE_BLOCK ((size_t)0)
#define USED_BLOCK ((size_t)1)
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/*************************************typedef*************************************/
typedef struct block_header {
    size_t block_info; /* chunk size + bit status (0 - FREE / 1 - USED) */
#ifdef DEBUG
	size_t magic_number;
#endif
} block_header_t;

/**********************Private Functions Forward Decleration**********************/
static size_t AlignToWordSize(size_t size);
static int BlockIsUsed(block_header_t* header);
static int GetBlockSize(block_header_t* header);
static void MergeFreeBlocks(block_header_t* allocator);
static void InitBlockHeader(block_header_t* header, size_t block_size, size_t status);
static block_header_t* NextBlock(block_header_t* current_block);
#ifdef DEBUG
	static int BlockHeaderIsValid(block_header_t* header);
#endif

/************************************Functions************************************/
vsa_t* VSAInit(void* pool, size_t pool_size)
{
	block_header_t* first_block = NULL;
	block_header_t* end_block = NULL;
	size_t header_size = sizeof(block_header_t);
	size_t pool_size_aligned = AlignToWordSize(pool_size);
	size_t end_block_size = header_size;
	size_t first_block_size = pool_size_aligned - header_size - end_block_size;	
	size_t pool_start_offset = 0;
	
	assert(NULL != pool);

	pool_start_offset = AlignToWordSize((size_t)pool) - (size_t)pool;
	pool = (void*)((char*)pool + pool_start_offset);
	
	assert(pool_size > 2 * header_size + WORD_SIZE);
	
	pool_size_aligned = AlignToWordSize(pool_size - pool_start_offset);
	
	first_block = (block_header_t*)pool;
	InitBlockHeader(first_block, first_block_size, FREE_BLOCK);
	
	end_block = (block_header_t*)((char*)first_block + header_size + first_block_size);
	InitBlockHeader(end_block, 0, 0);
	
	return ((vsa_t*)first_block);
}

void* VSAMalloc(vsa_t* allocator, size_t size)
{
	size_t requested_size = AlignToWordSize(size);
	size_t header_size = sizeof(block_header_t);
	size_t alloc_size = 0;
	size_t current_size = 0;
	size_t remaining = 0;
	block_header_t* current_block = NULL;
	block_header_t* next_unused_block = NULL;	

	assert(NULL != allocator);
	
	MergeFreeBlocks((block_header_t*)allocator);
	
	current_block = (block_header_t*)allocator;
	
	while (0 != current_block->block_info)
	{
		assert(BlockHeaderIsValid(current_block));

		if (!BlockIsUsed(current_block))
		{
			current_size = GetBlockSize(current_block);
			
			if (current_size >= requested_size)
			{
				remaining = current_size - requested_size;
				alloc_size = requested_size;
				/* if we have enough bytes, split the block */
				if (remaining >= header_size + WORD_SIZE)
				{
					next_unused_block = (block_header_t*)((char*)current_block + header_size + requested_size);
					InitBlockHeader(next_unused_block, remaining - header_size, FREE_BLOCK);
				}
				else
				{
					alloc_size = current_size;
				}
				
				InitBlockHeader(current_block, alloc_size, USED_BLOCK);
			
				return ((char*)current_block + header_size);
			}
		}
		
		current_block = NextBlock(current_block);
	}
	
	return (NULL);
}

void VSAFree(void* block)
{
	size_t header_size = sizeof(block_header_t);
	block_header_t* block_header = NULL;

	assert(NULL != block);
	
	block_header = (block_header_t*)((char*)block - header_size);

	assert(BlockHeaderIsValid(block_header));
	
	block_header->block_info &= (~IS_USED_MASK);
}

size_t VSALargestChunkAvailable(vsa_t* allocator)
{
	size_t count = 0;
	size_t max = 0;
	size_t header_size = sizeof(block_header_t);	
	block_header_t* current_block = NULL;

	assert(NULL != allocator);
	
	MergeFreeBlocks((block_header_t*)allocator);
	
	current_block = (block_header_t*)allocator;
	
	while (0 != current_block->block_info)
	{
		if (!BlockIsUsed(current_block))
		{
			count += header_size + GetBlockSize(current_block);			
		}
		
		else
		{
			if (count >= header_size)
			{
				max = MAX(count - header_size, max);
			}
			
			count = 0;
		}
		
		current_block = NextBlock(current_block);
	}
	
	if (count >= header_size)
	{
		return (MAX(count - header_size, max));
	}
	
	return (max);
}

/********************************Private Functions********************************/
static size_t AlignToWordSize(size_t size)
{
	if (size > 0)
	{
		return (((size + WORD_SIZE - 1) / WORD_SIZE) * WORD_SIZE);
	}
	
	return (WORD_SIZE);
}

static int BlockIsUsed(block_header_t* header)
{
	return (header->block_info & IS_USED_MASK);
}

static int GetBlockSize(block_header_t* header)
{	
	return (header->block_info & ~IS_USED_MASK);
}

static block_header_t* NextBlock(block_header_t* current_block)
{
	size_t header_size = sizeof(block_header_t);
	size_t curr_block_size = GetBlockSize(current_block);
	
	return ((block_header_t*)((char*)current_block + header_size + curr_block_size));
}

static void MergeFreeBlocks(block_header_t* allocator)
{
    block_header_t* current = allocator;
    block_header_t* next = NULL;
    size_t header_size = sizeof(block_header_t);
	size_t new_size = 0;
	
    while (0 != current->block_info)
    {
        next = NextBlock(current);

        if (0 == next->block_info)
        {
            break;
        }

        if (!BlockIsUsed(current) && !BlockIsUsed(next))
        {
            new_size = GetBlockSize(current) + GetBlockSize(next) + header_size;
            InitBlockHeader(current, new_size, FREE_BLOCK);
        }
        else
        {
            current = next;
        }
    }
}

static void InitBlockHeader(block_header_t* header, size_t block_size, size_t status)
{
	header->block_info = (block_size & ~IS_USED_MASK) | status;
	
#ifdef DEBUG
	header->magic_number = MAGIC_NUMBER;
#endif
}

#ifdef DEBUG
	static int BlockHeaderIsValid(block_header_t* header)
	{
		(void)header;	
		
		return ((header->magic_number) == MAGIC_NUMBER);

	}
#endif
