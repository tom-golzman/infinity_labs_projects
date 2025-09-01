#include <stdio.h>

#include "test_utils.h"
#include "bit_trie.h"

void TestCreateAndDestroy();
void TestGet();
void TestFree();
void TestCountFree();

int main(void)
{
	TestCreateAndDestroy();
	TestGet();
	TestFree();
	TestCountFree();

	printf("\n");

	return (0);
}

void TestCreateAndDestroy()
{
	bit_trie_t* trie = NULL;

	TITLE("Test: BitTrieCreate() + BTrieDestroy()");

	trie = BitTrieCreate(5);
	RUN_TEST("Create returns non-NULL", NULL != trie);

	BTrieDestroy(trie);
}

void TestGet()
{
	bit_trie_t* trie = BitTrieCreate(4);
	unsigned long addr1 = 0, addr2 = 0, get_result = 0;

	size_t i = 0, total = (1UL << 4); /* 16 addresses total */
	size_t used = 2; /* already allocated above */

	TITLE("Test: BTrieGet()");

	addr1 = BTrieGet(trie, 5); /* 0101 */
	RUN_TEST("Get specific address (5)", addr1 == 5);

	addr2 = BTrieGet(trie, 5);
	RUN_TEST("Get same address again returns different", addr2 != 5 && addr2 != 0);

	for (i = 0; i < total - used; ++i)
	{
		BTrieGet(trie, 0);
	}

	get_result = BTrieGet(trie, 0);
	RUN_TEST("Trie is full returns 0", get_result == 0);

	BTrieDestroy(trie);
}

void TestFree()
{
	bit_trie_t* trie = BitTrieCreate(3);
	unsigned long a1, a2;

	TITLE("Test: BTrieFree()");

	a1 = BTrieGet(trie, 2); /* 010 */
	a2 = BTrieGet(trie, 3); /* 011 */
	(void)a2;

	BTrieFree(trie, a1); /* free 2 */

	a1 = BTrieGet(trie, 2); /* try again */
	RUN_TEST("Free and reuse address (2)", a1 == 2);

	BTrieDestroy(trie);
}

void TestCountFree()
{
	bit_trie_t* trie = BitTrieCreate(3);
	size_t count = 0;
	unsigned long i, a1, a2;

	TITLE("Test: BTrieCountFree()");

	count = BTrieCountFree(trie);
	RUN_TEST("Initial free count is 8", count == 8);

	a1 = BTrieGet(trie, 1); /* 001 */
	a2 = BTrieGet(trie, 7); /* 111 */
	(void)a1;
	(void)a2;

	count = BTrieCountFree(trie);
	RUN_TEST("After 2 allocations, free count is 6", count == 6);

	BTrieFree(trie, 1); /* free 001 */

	count = BTrieCountFree(trie);
	RUN_TEST("After freeing 1, free count is 7", count == 7);

	for (i = 0; i < 8; ++i)
	{
		BTrieGet(trie, i);
	}

	count = BTrieCountFree(trie);
	RUN_TEST("After filling all, free count is 0", count == 0);

	BTrieDestroy(trie);
}