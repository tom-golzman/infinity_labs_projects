/************************************includes************************************/
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <assert.h>

#include "vsa.h"

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define BOLD_TITLE "\033[1;4m"
#define TITLE "\033[4m"

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
	
	return (0);
}

/************************************Functions************************************/
void TestInitAndMalloc()
{
	size_t pool_size = 1000;
	void* pool = (void*)malloc(pool_size);
	void* block1 = NULL;
	void* block2 = NULL;
	vsa_t* allocator = VSAInit(pool, pool_size);

    printf(TITLE "\nTest Init() and Malloc():\n" RESET);
    
    block1 = VSAMalloc(allocator, 100);
    block2 = VSAMalloc(allocator, 200);
    
    if (NULL != block1 && NULL != block2 && block1 != block2)
    {
    	printf(GREEN "Test 1 Passed!\n" RESET);
    }
    else
    {
    	printf(RED "Test 1 Failed!\n" RESET);
    }
    
    free(pool);
}

void TestFreeAndReuse()
{
	size_t pool_size = 1000;
	void* pool = (void*)malloc(pool_size);
	void* block1 = NULL;
	void* block2 = NULL;
	vsa_t* allocator = VSAInit(pool, pool_size);

    printf(TITLE "\nTest Free() and reuse same block:\n" RESET);
    
    block1 = VSAMalloc(allocator, 100);
    VSAFree(block1);
    block2 = VSAMalloc(allocator, 100);
    
    if (NULL != block2 && block1 == block2)
    {
    	printf(GREEN "Test 1 Passed!\n" RESET);
    }
    else
    {
    	printf(RED "Test 1 Failed!\n" RESET);
    }
    
    free(pool);
}

void TestLargestChunk()
{
	size_t pool_size = 1000;
	size_t expected = 0;
	size_t result = 0;
	void* pool = NULL;
	vsa_t* allocator = NULL;
#ifndef NDEBUG
	size_t header_size = sizeof(size_t)*2;
#else
	size_t header_size = sizeof(size_t);
#endif
	
    printf(TITLE "\nTest LargestChunkAvaliable():\n" RESET);	

	expected = pool_size - header_size*2;
	pool = (void*)malloc(pool_size);
	allocator = VSAInit(pool, pool_size);
	
    result = VSALargestChunkAvailable(allocator);
    
    if (result == expected)
    {
    	printf(GREEN "Test 1 Passed!\n" RESET);
    }
    else
    {
        printf(RED "Test Failed! Expected %lu, Result %lu\n" RESET, (unsigned long)expected, (unsigned long)result);
    }
    
    free(pool);    
}

void TestLargestChunkAfterMalloc()
{
	size_t pool_size = 128;
	size_t allocated_data_size = 16;
	size_t expected = 0;
	size_t result = 0;
	void* pool = NULL;
	vsa_t* allocator = NULL;
#ifndef NDEBUG
	size_t header_size = sizeof(size_t)*2;
#else
	size_t header_size = sizeof(size_t);
#endif
	
    printf(TITLE "\nTest LargestChunkAvaliable() after Malloc():\n" RESET);	

	pool = (void*)malloc(pool_size);
	allocator = VSAInit(pool, pool_size); 
	
	if (pool_size - header_size * 3 >= allocated_data_size + WORD_SIZE)
	{
		expected = pool_size - (2 * header_size) - allocated_data_size - header_size;
	}
	else
	{
		expected = 0;
	}
		
	VSAMalloc(allocator, 16);
	result = VSALargestChunkAvailable(allocator);
    
    if (result == expected)
    {
    	printf(GREEN "Test 1 Passed!\n" RESET);
    }
    else
    {
        printf(RED "Test Failed! Expected %lu, Result %lu\n" RESET, (unsigned long)expected, (unsigned long)result);
    }
    
    free(pool);
}

void TestLargestChunkAfterMallocAndFree()
{
    size_t pool_size = 128;
    size_t block_size = 16;
    size_t expected = 0;
    size_t result = 0;
    void* pool = malloc(pool_size);
    vsa_t* vsa = VSAInit(pool, pool_size);
    void* b2 = NULL;
#ifndef NDEBUG
    size_t header_size = sizeof(size_t) * 2;
#else
	size_t F0, F1, F2, F3;
    size_t header_size = sizeof(size_t);
#endif

    printf("\n"  "Test LargestChunkAvaliable() after Malloc() and Free():\n" );

    VSAMalloc(vsa, block_size);
    b2 = VSAMalloc(vsa, block_size);
    VSAMalloc(vsa, block_size);

    VSAFree(b2);

#ifndef NDEBUG
	expected = block_size;
#else
	F0 = pool_size - 2 * header_size;
	F1 = ((F0 - block_size - header_size + WORD_SIZE - 1) / WORD_SIZE) * WORD_SIZE;
	F2 = ((F1 - block_size - header_size + WORD_SIZE - 1) / WORD_SIZE) * WORD_SIZE;
	F3 = ((F2 - block_size - header_size + WORD_SIZE - 1) / WORD_SIZE) * WORD_SIZE;
	expected = F3;
#endif

    result = VSALargestChunkAvailable(vsa);

    if (result == expected)
    {
        printf(GREEN "Test Passed! Largest = %lu\n" RESET, (unsigned long)result);
    }
    else
    {
        printf(RED   "Test Failed! Expected %lu, Result %lu\n" RESET,
               (unsigned long)expected, (unsigned long)result);
    }

    free(pool);
}

void TestAlignment()
{
	size_t pool_size = 1000;
	void* pool = NULL;
	void* block = NULL;
	vsa_t* allocator = NULL;
	
    printf(TITLE "\nTest alignment:\n" RESET);
    
    pool = (void*)malloc(pool_size);
    allocator = VSAInit(pool, pool_size);
    block = VSAMalloc(allocator, 1);
    
    if (0 == ((size_t)block % WORD_SIZE))
    {
    	printf(GREEN "Test 1 Passed!\n" RESET);
    }
    else
    {
    	printf(RED "Test 1 Failed!\n" RESET);
    }
    
    free(pool);
}
