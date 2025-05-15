/************************************includes************************************/
#include <stdio.h> /* printf */

#include "bst.h"

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define BOLD_TITLE "\033[1;4m"
#define TITLE "\033[4m"

#define TRUE (1)
#define FALSE (0)
#define SUCCESS (0)
#define FAIL (1)

/************************************Functions Forward Decleration************************************/
void TestCreate();
void TestSize();
void TestIsEmpty();
void TestInsert();
void TestRemove();
void TestFind();
void TestBeginAndSameIter();
void TestEnd();
void TestForeach();

int Compare(const void* a, const void* b, void* param);
int Add(void* data, void* param);

/************************************typedef************************************/
typedef struct {
	int value;
	int key;
} elem_t;

/************************************main************************************/
int main(void)
{
	TestCreate();
	TestSize();
	TestIsEmpty();
	TestInsert();
	TestRemove();
	TestBeginAndSameIter();
	TestEnd();
	TestFind();
	TestForeach();
	
	printf("\n");
	
	return (0);
}

/************************************Functions************************************/
void TestCreate()
{
	bst_t* bst = BSTCreate(Compare, NULL);
	
	printf(TITLE "\nTest Create()\n" RESET);
	 	
	if (NULL != bst)
	{
		printf(GREEN "Test Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test Failed!\n" RESET);
	}
	
	BSTDestroy(bst);
}

void TestSize()
{
	bst_t* bst = BSTCreate(Compare, NULL);
	
	printf(TITLE "\nTest Size()\n" RESET);
	 	
	if (NULL != bst && 0 == BSTSize(bst))
	{
		printf(GREEN "Test Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test Failed! size = %lu\n" RESET, BSTSize(bst));
	}
	
	BSTDestroy(bst);
}

void TestIsEmpty()
{
	bst_t* bst = BSTCreate(Compare, NULL);
	
	printf(TITLE "\nTest IsEmpty()\n" RESET);
	 	
	if (NULL != bst && TRUE == BSTIsEmpty(bst))
	{
		printf(GREEN "Test Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test Failed! IsEmpty = %d\n" RESET, BSTIsEmpty(bst));
	}
	
	BSTDestroy(bst);
}

void TestInsert()
{
	bst_t* bst = BSTCreate(Compare, NULL);
	elem_t elem1 = {10, 1};
	elem_t elem2 = {20, 2};
	size_t size = 0;
	
	printf(TITLE "\nTest Insert()\n" RESET);
	
	BSTInsert(bst, &elem1);
	size = BSTSize(bst);
	if (NULL != bst && FALSE == BSTIsEmpty(bst) && size == 1)
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed! size = %lu\n" RESET, size);
	}
	
	BSTInsert(bst, &elem2);
	size = BSTSize(bst);
	if (NULL != bst && FALSE == BSTIsEmpty(bst) && size == 2)
	{
		printf(GREEN "Test 2 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 2 Failed! size = %lu\n" RESET, size);
	}

	BSTDestroy(bst);
}

void TestRemove()
{
	bst_t* bst = BSTCreate(Compare, NULL);
	elem_t elem1 = {10, 1};
	elem_t elem2 = {20, 2};
	bst_iter_t iter1, iter2;
	size_t size = 0;
	
	printf(TITLE "\nTest Remove()\n" RESET);
	
	iter1 = BSTInsert(bst, &elem1);
	iter2 = BSTInsert(bst, &elem2);

	BSTRemove(iter1);

	size = BSTSize(bst);
	
	if (NULL != bst && FALSE == BSTIsEmpty(bst) && size == 1)
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed! size = %lu\n" RESET, size);
	}
	
	BSTRemove(iter2);
	
	size = BSTSize(bst);
	
	if (NULL != bst && TRUE == BSTIsEmpty(bst) && size == 0)
	{
		printf(GREEN "Test 2 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 2 Failed! size = %lu\n" RESET, size);
	}

	BSTDestroy(bst);	
}

void TestBeginAndSameIter()
{
	bst_t* bst = BSTCreate(Compare, NULL);
	elem_t elem1 = {10, 1};
	bst_iter_t iter1, iter2;
	
	printf(TITLE "\nTest Begin() and IsSameIter()\n" RESET);
	
	iter1 = BSTInsert(bst, &elem1);
	iter2 = BSTBegin(bst);

	if (BSTIsSameIter(iter1, iter2))
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
	}
	
	BSTDestroy(bst);		
}

void TestEnd()
{
	bst_t* bst = BSTCreate(Compare, NULL);
	elem_t elem1 = {10, 1};
	elem_t elem2 = {20, 2};
	bst_iter_t iter1;
	elem_t* data;
	
	printf(TITLE "\nTest End()\n" RESET);
	
	if (BSTIsSameIter(BSTEnd(bst), BSTBegin(bst)))
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
	}
	
	BSTInsert(bst, &elem1);
	BSTInsert(bst, &elem2);

	iter1 = BSTPrev(BSTEnd(bst));
	data = (elem_t*)BSTGetData(iter1);
	
	if (data->key == 2)
	{
		printf(GREEN "Test 2 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 2 Failed!\n" RESET);
	}
	
	if (BSTIsSameIter(BSTNext(iter1), BSTEnd(bst)))
	{
		printf(GREEN "Test 3 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 3 Failed!\n" RESET);
	}

	BSTDestroy(bst);	
}

void TestFind()
{
	bst_t* bst = BSTCreate(Compare, NULL);
	elem_t elem1 = {10, 1};
	elem_t elem2 = {20, 2};
	elem_t elem3 = {30, 3};
	elem_t elem4 = {40, 4};
	bst_iter_t iter1, iter2;
	elem_t* data;
	
	printf(TITLE "\nTest Find()\n" RESET);
	
	BSTInsert(bst, &elem1);
	BSTInsert(bst, &elem2);
	iter1 = BSTInsert(bst, &elem3);
	BSTInsert(bst, &elem4);

	iter2 = BSTFind(bst, BSTBegin(bst), BSTEnd(bst), &elem3);
	data = (elem_t*)BSTGetData(iter2);
	
	if (data->key == 3)
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
	}
	
	BSTRemove(iter1);
	
	iter2 = BSTFind(bst, BSTBegin(bst), BSTEnd(bst), &elem3);
	
	if (BSTIsSameIter(BSTEnd(bst), iter2))
	{
		printf(GREEN "Test 2 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 2 Failed!\n" RESET);
	}

	BSTDestroy(bst);	
}

void TestForeach()
{
	bst_t* bst = BSTCreate(Compare, NULL);
	elem_t elem1 = {10, 1};
	elem_t elem2 = {20, 2};
	elem_t elem3 = {30, 3};
	bst_iter_t iter1, iter2, iter3;
	elem_t* data1;
	elem_t* data2;
	elem_t* data3;
	int foreach_status = 0;
	int param = 5;
	
	printf(TITLE "\nTest ForEach()\n" RESET);
	
	iter1 = BSTInsert(bst, &elem1);
	iter2 = BSTInsert(bst, &elem2);
	iter3 = BSTInsert(bst, &elem3);

	foreach_status = BSTForEach(BSTBegin(bst), BSTEnd(bst), Add, &param);
	
	data1 = (elem_t*)BSTGetData(iter1);
	data2 = (elem_t*)BSTGetData(iter2);
	data3 = (elem_t*)BSTGetData(iter3);
	
	if (SUCCESS == foreach_status && data1->value == 15 && data2->value == 25 && data3->value == 35)
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
	}

	BSTDestroy(bst);
}

int Compare(const void* a, const void* b, void* param)
{
    (void)param;
    return (((elem_t*)a)->key - ((elem_t*)b)->key);
}

int Add(void* data, void* param)
{
	((elem_t*)data)->value += *(int*)param;
	
	return (SUCCESS);
}
