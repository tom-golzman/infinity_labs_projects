/************************************includes************************************/
#include <stdio.h> /* printf */

#include "test_utils.h" /* colors, titles, status, boolean */
#include "bit_trie.h"

/************************************define************************************/

/************************************Functions Forward Decleration************************************/
void TestCreateAndDestroy();
void TestGet();
void TestFree();
void TestCountFree();

/************************************main************************************/
int main(void)
{
	TestCreateAndDestroy();
	TestGet();
	TestFree();
	
	printf("\n");
	
	return (0);
}

/************************************Functions************************************/
void TestCreateAndDestroy()
{
	bit_trie_t* trie = NULL;

	printf(BOLD_TITLE "\nTest: BitTrieCreate() + BTrieDestroy()\n" RESET);

	trie = BitTrieCreate(5);
	if (NULL != trie)
	{
		printf(GREEN "TEST 1 PASSED\n" RESET);
	}
	else
	{
		printf(RED "TEST 1 FAILED: " RESET "Allocation failed\n");
	}

	BTrieDestroy(trie);
}

void TestGet()
{
	bit_trie_t* trie = BitTrieCreate(4);
	unsigned long addr1 = 0, addr2 = 0, get_result = 0;
	
	size_t i = 0, total = (1UL << 4); /* 16 addresses total */
	size_t used = 2; /* already allocated above */


	printf(BOLD_TITLE "\nTest: BTrieGet()\n" RESET);

	/* try to get a specific address */
	addr1 = BTrieGet(trie, 5); /* 0101 */
	if (addr1 == 5)
	{
		printf(GREEN "TEST 1 PASSED\n" RESET);
	}
	else
	{
		printf(RED "TEST 1 FAILED: " RESET "Expected 5, got %lu\n", addr1);
	}

	/* try to get same address again → should return next available */
	addr2 = BTrieGet(trie, 5);
	if (addr2 != 5 && addr2 != 0)
	{
	printf(GREEN "TEST 2 PASSED\n" RESET);
	}
	else
	{
	printf(RED "TEST 2 FAILED: " RESET "Expected different from 5, got %lu\n", addr2);
	}

	/* edge: use all remaining addresses */
	for (i = 0; i < total - used; ++i)
	{
		BTrieGet(trie, 0);
	}

	/* now the trie should be full */
	get_result = BTrieGet(trie, 0);
	if (0 == get_result)
	{
		printf(GREEN "TEST 3 PASSED\n" RESET);
	}
	else
	{
		printf(RED "TEST 3 FAILED: " RESET "Expected 0 when trie is full, but get result=%lu\n", get_result);
	}

	BTrieDestroy(trie);
}

void TestFree()
{
	bit_trie_t* trie = BitTrieCreate(3);
	unsigned long a1, a2;

	printf(BOLD_TITLE "\nTest: BTrieFree()\n" RESET);

	a1 = BTrieGet(trie, 2); /* 010 */
	a2 = BTrieGet(trie, 3); /* 011 */
	(void)a2;
	
	BTrieFree(trie, a1); /* free 2 */

	a1 = BTrieGet(trie, 2); /* try again */
	if (a1 == 2)
	{
		printf(GREEN "TEST 1 PASSED\n" RESET);
	}
	else
	{
		printf(RED "TEST 1 FAILED: " RESET "Expected 2 again, got %lu\n", a1);
	}

	/* free unallocated address (no crash expected) */
	BTrieFree(trie, 7);
	printf(GREEN "TEST 2 PASSED: " RESET "Free unallocated address handled\n");

	BTrieDestroy(trie);
}
