/************************************includes************************************/
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <assert.h>

#include "vsa.h"
#include "test_utils.h"

/************************************define************************************/
#define WORD_SIZE (sizeof(size_t))

/************************************Functions Forward Decleration************************************/
void TestInitAndMalloc();
void TestFreeAndReuse();
void TestLargestChunk();
void TestLargestChunkAfterMalloc();
void TestLargestChunkAfterMallocAndFree();
void TestAlignment();

/************************************main************************************/
int main(void)
{
#ifndef NDEBUG
	printf(BOLD_TITLE "\nDEBUG VERSION\n" RESET);
#else
	printf(BOLD_TITLE "\nRELEASE VERSION\n" RESET);
#endif
	
	TestInitAndMalloc();
	TestFreeAndReuse();
	TestLargestChunk();	
	TestLargestChunkAfterMalloc();
	TestLargestChunkAfterMallocAndFree();
	TestAlignment();

	printf("\n");
	
	return 0;
}

/************************************Functions************************************/
void TestInitAndMalloc()
{
	size_t pool_size = 1000;
	void* pool = NULL;
	vsa_t* allocator = NULL;
	void* block1 = NULL;
	void* block2 = NULL;

	pool = malloc(pool_size);
	allocator = VSAInit(pool, pool_size);
	block1 = VSAMalloc(allocator, 100);
	block2 = VSAMalloc(allocator, 200);

	TITLE("Test Init() and Malloc()");
	RUN_TEST("Allocates two blocks successfully and they differ", 
		NULL != block1 && NULL != block2 && block1 != block2);

	free(pool);
}

void TestFreeAndReuse()
{
	size_t pool_size = 1000;
	void* pool = NULL;
	vsa_t* allocator = NULL;
	void* block1 = NULL;
	void* block2 = NULL;

	pool = malloc(pool_size);
	allocator = VSAInit(pool, pool_size);
	block1 = VSAMalloc(allocator, 100);
	VSAFree(block1);
	block2 = VSAMalloc(allocator, 100);

	TITLE("Test Free() and reuse same block");
	RUN_TEST("Block reused after free", block1 == block2);

	free(pool);
}

void TestLargestChunk()
{
	size_t pool_size = 1000;
	size_t header_size = sizeof(size_t);
	void* pool = NULL;
	vsa_t* allocator = NULL;
	size_t expected = 0;
	size_t result = 0;

#ifndef NDEBUG
	header_size *= 2;
#endif

	expected = pool_size - 2 * header_size;
	pool = malloc(pool_size);
	allocator = VSAInit(pool, pool_size);
	result = VSALargestChunkAvailable(allocator);

	TITLE("Test LargestChunkAvaliable()");
	RUN_TEST("Initial largest chunk equals total - 2 headers", result == expected);

	free(pool);
}

void TestLargestChunkAfterMalloc()
{
	size_t pool_size = 128;
	size_t header_size = sizeof(size_t);
	size_t allocated_data_size = 16;
	size_t expected = 0;
	size_t result = 0;
	void* pool = NULL;
	vsa_t* allocator = NULL;

#ifndef NDEBUG
	header_size *= 2;
#endif

	pool = malloc(pool_size);
	allocator = VSAInit(pool, pool_size);

	if (pool_size - header_size * 3 >= allocated_data_size + WORD_SIZE)
	{
		expected = pool_size - (2 * header_size) - allocated_data_size - header_size;
	}
	else
	{
		expected = 0;
	}

	VSAMalloc(allocator, allocated_data_size);
	result = VSALargestChunkAvailable(allocator);

	TITLE("Test LargestChunkAvaliable() after Malloc()");
	RUN_TEST("Expected value after malloc", result == expected);

	free(pool);
}

void TestLargestChunkAfterMallocAndFree()
{
	size_t pool_size = 128;
	size_t block_size = 16;
	void* pool = NULL;
	vsa_t* vsa = NULL;
	void* b1 = NULL;
	void* b2 = NULL;
	void* b3 = NULL;
	size_t result = 0;
	size_t expected = 0;

#ifndef NDEBUG
	expected = block_size;
#else
	size_t header_size = sizeof(size_t);
	size_t F0 = 0, F1 = 0, F2 = 0, F3 = 0;
#endif

	pool = malloc(pool_size);
	vsa = VSAInit(pool, pool_size);

	b1 = VSAMalloc(vsa, block_size);
	b2 = VSAMalloc(vsa, block_size);
	b3 = VSAMalloc(vsa, block_size);

	VSAFree(b2);

#ifndef NDEBUG
	/* already set above */
#else
	F0 = pool_size - 2 * header_size;
	F1 = ((F0 - block_size - header_size + WORD_SIZE - 1) / WORD_SIZE) * WORD_SIZE;
	F2 = ((F1 - block_size - header_size + WORD_SIZE - 1) / WORD_SIZE) * WORD_SIZE;
	F3 = ((F2 - block_size - header_size + WORD_SIZE - 1) / WORD_SIZE) * WORD_SIZE;
	expected = F3;
#endif

	result = VSALargestChunkAvailable(vsa);

	TITLE("Test LargestChunkAvaliable() after Malloc() and Free()");
	RUN_TEST("Free block in the middle is reused correctly", result == expected);

	free(pool);
}

void TestAlignment()
{
	size_t pool_size = 1000;
	void* pool = NULL;
	vsa_t* allocator = NULL;
	void* block = NULL;

	pool = malloc(pool_size);
	allocator = VSAInit(pool, pool_size);
	block = VSAMalloc(allocator, 1);

	TITLE("Test alignment");
	RUN_TEST("Returned block is aligned to word size", 0 == ((size_t)block % WORD_SIZE));

	free(pool);
}