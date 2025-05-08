/************************************includes************************************/
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "fsa.h"

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define TITLE "\033[1;4m"

#define SIZE_OF_STRUCT (sizeof(size_t))
#define WORD_SIZE (sizeof(size_t))

/************************************Functions Forward Decleration************************************/
void TestSuggestSize();
void TestInitAndCount();
void TestMalloc();

static size_t AlignToWordSize(size_t size);

/************************************main************************************/
int main(void)
{
	TestSuggestSize();
	TestInitAndCount();
	TestMalloc();
	
	printf("\n");
	
	return (0);
}

/************************************Functions************************************/
void TestSuggestSize()
{
	size_t num_of_blocks1 = 5;
	size_t block_size1 = 16;
	size_t expected1 = SIZE_OF_STRUCT + (num_of_blocks1 * AlignToWordSize(block_size1)); 
	size_t result1 = 0;

	size_t num_of_blocks2 = 0;
	size_t block_size2 = 10;
	size_t expected2 = SIZE_OF_STRUCT	; 
	size_t result2 = 0;

	printf(TITLE "Test SuggestSize():\n" RESET);
	
	result1 = FSASuggestSize(num_of_blocks1, block_size1);
	result2 = FSASuggestSize(num_of_blocks2, block_size2);
	
	if (result1 == expected1)
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
		printf("Expected: %lu, Result: %lu\n", (unsigned long)expected1, (unsigned long)result1);
	}
	
	if (result2 == expected2)
	{
		printf(GREEN "Test 2 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 2 Failed!\n" RESET);
		printf("Expected: %lu, Result: %lu\n", (unsigned long)expected2, (unsigned long)result2);
	}
}

void TestInitAndCount()
{
	size_t num_of_blocks = 5;
	size_t block_size = 16;
	size_t pool_size = FSASuggestSize(num_of_blocks, block_size);
	void* pool = (void*)malloc(pool_size);
	size_t expected = num_of_blocks;
	size_t result = 0;
	
	fsa_t* fsa = FSAInit(pool, pool_size, block_size);
    result = FSACountFreeBlocks(fsa);

    printf(TITLE "Test FSAInit + FSACountFreeBlocks():\n" RESET);

    if (result == expected)
    {
        printf(GREEN "Test Passed! (%lu blocks free)\n" RESET, (unsigned long)result);
    }
    else
    {
        printf(RED "Test Failed! got %lu, expected %lu\n" RESET, (unsigned long)result, (unsigned long)expected);
	}
	
    free(pool);
}

void TestMalloc()
{
    size_t num_of_blocks = 5;
    size_t block_size = 16;
    size_t pool_size = FSASuggestSize(num_of_blocks, block_size);
    void* pool = malloc(pool_size);
    fsa_t* fsa = FSAInit(pool, pool_size, block_size);
    void* blocks[5] = {0};
    size_t i = 0;
    int passed = 1;

    printf(TITLE "Test Malloc():\n" RESET);

    for (i = 0; i < num_of_blocks; ++i)
    {
        blocks[i] = FSAMalloc(fsa);
        if (blocks[i] == NULL)
        {
            passed = 0;
            printf(RED "Alloc Failed on block %lu\n" RESET, (unsigned long)i);
        }
    }

    if (FSAMalloc(fsa) == NULL)
    {
        printf(GREEN "Extra alloc correctly returned NULL\n" RESET);
    }
    else
    {
        passed = 0;
        printf(RED "Extra alloc should return NULL but didn't\n" RESET);
    }

    for (i = 0; i < num_of_blocks; ++i)
    {
        FSAFree(fsa, blocks[i]);
    }

    if (FSACountFreeBlocks(fsa) == num_of_blocks)
        printf(GREEN "Free count correct after releasing all blocks\n" RESET);
    else
    {
        passed = 0;
        printf(RED "Free count incorrect after releasing\n" RESET);
    }

    if (passed)
    {
        printf(GREEN "Test Passed!\n" RESET);
    }
    else
    {
        printf(RED "Test Failed!\n" RESET);
	}
	
    free(pool);
}

static size_t AlignToWordSize(size_t size)
{
	if (size > 0)
	{
		return (((size + WORD_SIZE - 1) / WORD_SIZE) * WORD_SIZE);
	}
	
	return (WORD_SIZE);
}

