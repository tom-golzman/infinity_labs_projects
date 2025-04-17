/************************************includes************************************/
#include <stdio.h> /* printf */

#include "oll.h"

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define TITLE "\033[1;4m"

#define SUCCESS 0
#define FAIL 1

/************************************Functions Forward Decleration************************************/
void TestOListCreate();
void TestOListInsert();
void TestOListRemove();
void TestOListMerge();
void TestOListSize();
void TestOListIsEmpty();
void TestOListBegin();
void TestOListEnd();
void TestOListNext();
void TestOListPrev();
void TestOListForEach();
void TestOListFind();
void TestOListFindIf();
void TestOListGetData();
void TestOListIsSameIter();
void TestOListPopFront();
void TestOListPopBack();

int CompareInts(const void* a, const void* b, void* param);
int IsMatch(const void* a, const void* b);
int MultiplyByParam(void* data, void* param);

/************************************main************************************/
int main(void)
{
	TestOListCreate();
    TestOListInsert();
    TestOListRemove();
    TestOListMerge();
    TestOListSize();
    TestOListIsEmpty();
    TestOListBegin();
    TestOListEnd();
    TestOListNext();
    TestOListPrev();
    TestOListForEach();
    TestOListFind();
    TestOListFindIf();
    TestOListGetData();
    TestOListIsSameIter();
    TestOListPopFront();
    TestOListPopBack();
    
    printf("\n");
    
    return 0;
}

/************************************Functions************************************/
void TestOListCreate()
{
    oll_t* list = OListCreate((olist_comparer_t)CompareInts, NULL);
    
    printf(TITLE "\nTest: OListCreate()\n" RESET);

    if (list != NULL && OListIsEmpty(list))
    {
        printf(GREEN "PASSED: " RESET "List created and is empty\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "List not created properly\n");
    }
    
    OListDestroy(list);
}

void TestOListInsert(void)
{

	int a = 5, b = 2, c = 8;
	oll_t* list = OListCreate(CompareInts, NULL);
	
	printf(TITLE "\nTest: OListInsert()\n" RESET);
	
	OListInsert(list, &a);
	OListInsert(list, &b);
	OListInsert(list, &c);
	
	if (*(int*)OListGetData(OListBegin(list)) == 2)
	{
		printf(GREEN "PASSED: " RESET "Insert maintains sort order\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "Insert sort incorrect\n");
	}
	
	OListDestroy(list);
}

void TestOListRemove(void)
{
	int a = 10;
	oll_iter_t it;
	oll_t* list = OListCreate(CompareInts, NULL);
	
	printf(TITLE "\nTest: OListRemove()\n" RESET);
	
	it = OListInsert(list, &a);
	it = OListRemove(it);
	
	if (OListIsEmpty(list))
	{
		printf(GREEN "PASSED: " RESET "Remove removed element\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "Remove did not work\n");
	}
	
	OListDestroy(list);
}

void TestOListMerge(void)
{
	int a = 1, b = 3, c = 2;
	oll_t* l1 = OListCreate(CompareInts, NULL);
	oll_t* l2 = OListCreate(CompareInts, NULL);
	
	printf(TITLE "\nTest: OListMerge()\n" RESET);
	
	OListInsert(l1, &a);
	OListInsert(l1, &b);
	OListInsert(l2, &c);
	
	OListMerge(l1, l2);
	
	if (OListSize(l1) == 3 && OListIsEmpty(l2))
	{
		printf(GREEN "PASSED: " RESET "Merge successful\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "Merge failed\n");
	}
	
	OListDestroy(l1);
	OListDestroy(l2);
}

void TestOListSize(void)
{
	int a = 1;
	oll_t* list = OListCreate(CompareInts, NULL);
	
	printf(TITLE "\nTest: OListSize()\n" RESET);
	
	if (OListSize(list) == 0)
	{
		printf(GREEN "PASSED: " RESET "Empty size is 0\n");
	}
	
	OListInsert(list, &a);
	if (OListSize(list) == 1)
	{
		printf(GREEN "PASSED: " RESET "Size updated\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "Size incorrect\n");
	}
	
	OListDestroy(list);
}

void TestOListIsEmpty(void)
{
	int a = 1;
	oll_t* list = OListCreate(CompareInts, NULL);
	
	printf(TITLE "\nTest: OListIsEmpty()\n" RESET);
	
	if (OListIsEmpty(list))
	{
		printf(GREEN "PASSED: " RESET "Correct empty check\n");
	}
	
	OListInsert(list, &a);
	if (!OListIsEmpty(list))
	{
		printf(GREEN "PASSED: " RESET "Correct non-empty check\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "Empty check failed\n");
	}
	
	OListDestroy(list);
}

void TestOListBegin(void)
{
	int a = 9;
	oll_t* list = OListCreate(CompareInts, NULL);
	
	printf(TITLE "\nTest: OListBegin()\n" RESET);
	
	OListInsert(list, &a);
	
	if (*(int*)OListGetData(OListBegin(list)) == a)
	{
		printf(GREEN "PASSED: " RESET "Begin returns first element\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "Begin failed\n");
	}
	
	OListDestroy(list);
}

void TestOListEnd(void)
{
	oll_t* list = OListCreate(CompareInts, NULL);
	
	printf(TITLE "\nTest: OListEnd()\n" RESET);
	
	if (OListIsSameIter(OListBegin(list), OListEnd(list)))
	{
		printf(GREEN "PASSED: " RESET "End same as begin in empty list\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "End incorrect\n");
	}
	
	OListDestroy(list);
}

void TestOListNext(void)
{
	int a = 1, b = 2;
	oll_iter_t it1, it2;
	oll_t* list = OListCreate(CompareInts, NULL);
	
	printf(TITLE "\nTest: OListNext()\n" RESET);
	
	OListInsert(list, &a);
	OListInsert(list, &b);
	
	it1 = OListBegin(list);
	it2 = OListNext(it1);
	
	if (*(int*)OListGetData(it2) == 2)
	{
		printf(GREEN "PASSED: " RESET "Next returns correct iterator\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "Next incorrect\n");
	}
	
	OListDestroy(list);
}

void TestOListPrev(void)
{
	int a = 1, b = 2;
	oll_iter_t it1, it2;
	oll_t* list = OListCreate(CompareInts, NULL);
	
	printf(TITLE "\nTest: OListPrev()\n" RESET);
	
	OListInsert(list, &a);
	OListInsert(list, &b);
	
	it2 = OListEnd(list);
	it1 = OListPrev(it2);
	
	if (*(int*)OListGetData(it1) == 2)
	{
		printf(GREEN "PASSED: " RESET "Prev returns correct iterator\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "Prev incorrect\n");
	}
	
	OListDestroy(list);
}

void TestOListFind(void)
{
	int a = 1, b = 2, c = 3;
	oll_iter_t found;
	oll_t* list = OListCreate(CompareInts, NULL);
	
	printf(TITLE "\nTest: OListFind()\n" RESET);
	
	OListInsert(list, &a);
	OListInsert(list, &b);
	OListInsert(list, &c);
	
	found = OListFind(list, OListBegin(list), OListEnd(list), &b);
	
	if (*(int*)OListGetData(found) == 2)
	{
		printf(GREEN "PASSED: " RESET "Find found correct element\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "Find failed\n");
	}
	
	OListDestroy(list);
}

void TestOListFindIf(void)
{
	int a = 1, b = 2, c = 3;
	oll_iter_t found;
	oll_t* list = OListCreate(CompareInts, NULL);
	
	printf(TITLE "\nTest: OListFindIf()\n" RESET);
	
	OListInsert(list, &a);
	OListInsert(list, &b);
	OListInsert(list, &c);
	
	found = OListFindIf(OListBegin(list), OListEnd(list), IsMatch, &c);
	
	if (*(int*)OListGetData(found) == 3)
	{
		printf(GREEN "PASSED: " RESET "FindIf matched correctly\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "FindIf failed\n");
	}
	
	OListDestroy(list);
}

void TestOListGetData(void)
{
	int a = 10;
	oll_iter_t it;
	oll_t* list = OListCreate(CompareInts, NULL);
	
	printf(TITLE "\nTest: OListGetData()\n" RESET);
	
	it = OListInsert(list, &a);
	
	if (*(int*)OListGetData(it) == a)
	{
		printf(GREEN "PASSED: " RESET "GetData returns correct value\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "GetData incorrect\n");
	}
	
	OListDestroy(list);
}

void TestOListIsSameIter(void)
{
	int a = 1;
	oll_iter_t it1, it2;
	oll_t* list = OListCreate(CompareInts, NULL);
	
	printf(TITLE "\nTest: OListIsSameIter()\n" RESET);
	
	it1 = OListInsert(list, &a);
	it2 = it1;
	
	if (OListIsSameIter(it1, it2))
	{
		printf(GREEN "PASSED: " RESET "IsSameIter correct\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "IsSameIter failed\n");
	}
	
	OListDestroy(list);
}

void TestOListPopFront(void)
{
	int a = 1, b = 2;
	oll_iter_t it;
	oll_t* list = OListCreate(CompareInts, NULL);
	
	printf(TITLE "\nTest: OListPopFront()\n" RESET);
	
	OListInsert(list, &a);
	OListInsert(list, &b);
	OListPopFront(list);
	
	it = OListBegin(list);
	
	if (*(int*)OListGetData(it) == 2)
	{
		printf(GREEN "PASSED: " RESET "PopFront removed first\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "PopFront incorrect\n");
	}
	
	OListDestroy(list);
}

void TestOListPopBack(void)
{
	int a = 1, b = 2;
	oll_iter_t it;
	oll_t* list = OListCreate(CompareInts, NULL);
	
	printf(TITLE "\nTest: OListPopBack()\n" RESET);
	
	OListInsert(list, &a);
	OListInsert(list, &b);
	
	OListPopBack(list);
	
	it = OListBegin(list);
	
	if (*(int*)OListGetData(it) == 1 && OListIsSameIter(OListNext(it), OListEnd(list)))
	{
		printf(GREEN "PASSED: " RESET "PopBack removed last\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "PopBack incorrect\n");
	}
	
	OListDestroy(list);
}

void TestOListForEach(void)
{
	int a = 1, b = 2, c = 3;
	int mult = 2;
	oll_t* list = OListCreate(CompareInts, NULL);
	
	printf(TITLE "\nTest: OListForEach()\n" RESET);
	
	OListInsert(list, &a);
	OListInsert(list, &b);
	OListInsert(list, &c);
	
	OListForEach(OListBegin(list), OListEnd(list), MultiplyByParam, &mult);
	
	if (a == 2 && b == 4 && c == 6)
	{
		printf(GREEN "PASSED: " RESET "ForEach applied to all\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "ForEach failed: %d %d %d\n", a, b, c);
	}

	OListDestroy(list);
}

int CompareInts(const void* a, const void* b, void* param)
{
    (void)param;
    return *(int*)a - *(int*)b;
}

int IsMatch(const void* a, const void* b)
{
    return *(int*)a == *(int*)b;
}

int MultiplyByParam(void* data, void* param)
{
    *(int*)data *= *(int*)param;
    return SUCCESS;
}



