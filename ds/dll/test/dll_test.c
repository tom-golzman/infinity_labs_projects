/************************************includes************************************/
#include "dll.h"
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define TITLE "\033[1;4m"
#define SUCCESS 0
#define FAIL 1

/************************************Functions Forward Decleration************************************/
void TestDListCreate();
void TestDListPushFrontBack();
void TestDListPopFrontBack();
void TestDListSizeIsEmpty();
void TestDListSetGetData();
void TestDListIsSameIter();
void TestDListPrevNext();
void TestDListInsertRemove();
void TestDListFind();
void TestDListMultiFind();
void TestDListSplice();
void TestDListForEach();

/************************************main************************************/
int main(void)
{
    TestDListCreate();
    TestDListPushFrontBack();
    TestDListPopFrontBack();
    TestDListSizeIsEmpty();
    TestDListSetGetData();
    TestDListIsSameIter();
    TestDListPrevNext();
    TestDListInsertRemove();
    TestDListFind();
    TestDListMultiFind();
    TestDListSplice();
    TestDListForEach();

    return 0;
}

/************************************Functions************************************/

void TestDListCreate()
{
    dll_t* list = DListCreate();
    printf(TITLE "\nTest: DListCreate()\n" RESET);
    if (list != NULL && DListIsEmpty(list))
    {
        printf(GREEN "PASSED: " RESET "List created and is empty\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "List not created or not empty\n");
    }
    DListDestroy(list);
}

void TestDListPushFrontBack()
{
    int a = 1, b = 2;
    dll_t* list = DListCreate();
    printf(TITLE "\nTest: DListPushFront() / DListPushBack()\n" RESET);
    DListPushFront(list, &a);
    DListPushBack(list, &b);
    if (*(int*)DListGetData(DListBegin(list)) == 1 &&
        *(int*)DListGetData(DListPrev(DListEnd(list))) == 2)
    {
        printf(GREEN "PASSED: " RESET "Push front/back works\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "Push front/back failed\n");
    }
    DListDestroy(list);
}

void TestDListPopFrontBack()
{
    int a = 1, b = 2;
    dll_t* list = DListCreate();
    printf(TITLE "\nTest: DListPopFront() / DListPopBack()\n" RESET);

    DListPushFront(list, &a);
    DListPushBack(list, &b);

    DListPopFront(list);

    if (*(int*)DListGetData(DListBegin(list)) == 2)
    {
        printf(GREEN "PASSED: " RESET "Pop front removed correct item\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "Pop front incorrect\n");
    }

    DListPopBack(list);
    if (DListIsEmpty(list))
    {
        printf(GREEN "PASSED: " RESET "Pop back made list empty\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "Pop back failed\n");
    }

    DListDestroy(list);
}

void TestDListSizeIsEmpty()
{
    int a = 5;
    dll_t* list = DListCreate();
    printf(TITLE "\nTest: DListSize() / DListIsEmpty()\n" RESET);

    if (DListIsEmpty(list) && DListSize(list) == 0)
    {
        printf(GREEN "PASSED: " RESET "Empty list returns size 0\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "Empty list size incorrect\n");
    }

    DListPushBack(list, &a);
    if (!DListIsEmpty(list) && DListSize(list) == 1)
    {
        printf(GREEN "PASSED: " RESET "Non-empty list size correct\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "Non-empty list size incorrect\n");
    }

    DListDestroy(list);
}

void TestDListSetGetData()
{
    int a = 10, b = 20;
    dll_t* list = DListCreate();
    iter_t iter;

    printf(TITLE "\nTest: DListSetData() / DListGetData()\n" RESET);

    DListPushBack(list, &a);
    iter = DListBegin(list);
    DListSetData(iter, &b);

    if (*(int*)DListGetData(iter) == 20)
    {
        printf(GREEN "PASSED: " RESET "Set/Get data updated correctly\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "Set/Get data failed\n");
    }

    DListDestroy(list);
}

void TestDListIsSameIter()
{
    int a = 1;
    dll_t* list = DListCreate();
    iter_t iter1, iter2;

    printf(TITLE "\nTest: DListIsSameIter()\n" RESET);

    DListPushBack(list, &a);
    iter1 = DListBegin(list);
    iter2 = DListBegin(list);

    if (DListIsSameIter(iter1, iter2))
    {
        printf(GREEN "PASSED: " RESET "SameIter returns true\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "SameIter failed\n");
    }

    DListDestroy(list);
}

void TestDListPrevNext()
{
    int a = 1, b = 2;
    dll_t* list = DListCreate();
    iter_t iter1, iter2;

    printf(TITLE "\nTest: DListPrev() / DListNext()\n" RESET);

    DListPushBack(list, &a);
    DListPushBack(list, &b);

    iter1 = DListBegin(list);
    iter2 = DListNext(iter1);

    if (DListPrev(iter2) == iter1)
    {
        printf(GREEN "PASSED: " RESET "Prev/Next consistent\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "Prev/Next inconsistent\n");
    }

    DListDestroy(list);
}

void TestDListInsertRemove()
{
    int a = 10;
    dll_t* list = DListCreate();
    iter_t iter;

    printf(TITLE "\nTest: DListInsert() / DListRemove()\n" RESET);

    iter = DListInsert(list, DListEnd(list), &a);

    if (*(int*)DListGetData(iter) == a)
    {
        printf(GREEN "PASSED: " RESET "Insert placed correct data\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "Insert failed\n");
    }

    DListRemove(iter);
    if (DListIsEmpty(list))
    {
        printf(GREEN "PASSED: " RESET "Remove successful\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "Remove failed\n");
    }

    DListDestroy(list);
}

int IntIsMatch(const void* data1, const void* data2, const void* param)
{
    return (*(int*)data1 == *(int*)data2);
	
	(void) param;
}

void TestDListFind()
{
    int a = 3, b = 7, c = 11;
    dll_t* list = DListCreate();
    iter_t result;

    printf(TITLE "\nTest: DListFind()\n" RESET);

    DListPushBack(list, &a);
    DListPushBack(list, &b);
    DListPushBack(list, &c);

    result = DListFind(DListBegin(list), DListEnd(list), IntIsMatch, NULL, &b);

    if (*(int*)DListGetData(result) == b)
    {
        printf(GREEN "PASSED: " RESET "Find found correct element\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "Find failed\n");
    }

    DListDestroy(list);
}

void TestDListMultiFind()
{
    int a = 5, b = 8, c = 5;
    dll_t* list = DListCreate();
    dll_t* result = DListCreate();

    printf(TITLE "\nTest: DListMultiFind()\n" RESET);

    DListPushBack(list, &a);
    DListPushBack(list, &b);
    DListPushBack(list, &c);

    DListMultiFind(DListBegin(list), DListEnd(list), IntIsMatch, NULL, &a, result);

    if (DListSize(result) == 2)
    {
        printf(GREEN "PASSED: " RESET "MultiFind found all matches\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "MultiFind result incorrect\n");
    }

    DListDestroy(list);
    DListDestroy(result);
}

void TestDListSplice()
{
	int a = 1, b = 2, c = 3;
	dll_t* list1 = DListCreate();
	dll_t* list2 = DListCreate();
	iter_t from, to, it;

	printf(TITLE "\nTest: DListSplice()\n" RESET);

	DListPushBack(list1, &a);
	DListPushBack(list1, &b);
	DListPushBack(list2, &c);

	from = DListBegin(list1);
	to = DListEnd(list1);

	DListSplice(DListEnd(list2), from, to);

	it = DListBegin(list2);
	if (*(int*)DListGetData(it) == c && *(int*)DListGetData(DListNext(it)) == a &&
		*(int*)DListGetData(DListNext(DListNext(it))) == b)
	{
		printf(GREEN "PASSED: " RESET "Splice moved nodes correctly\n");
	}
	else
	{
		printf(RED "FAILED: " RESET "Splice failed\n");
	}

	DListDestroy(list1);
	DListDestroy(list2);
}

int MultiplyByParam(void* data, const void* param)
{
    *(int*)data *= *(int*)param;
    return (SUCCESS);
}

void TestDListForEach()
{
    int a = 1, b = 2, c = 3;
    const int multiplier = 2;
    dll_t* list = DListCreate();

    printf(TITLE "\nTest: DListForEach()\n" RESET);

    DListPushBack(list, &a);
    DListPushBack(list, &b);
    DListPushBack(list, &c);

    DListForEach(DListBegin(list), DListEnd(list), MultiplyByParam, &multiplier);

    if (a == 2 && b == 4 && c == 6)
    {
        printf(GREEN "PASSED: " RESET "ForEach applied function to all\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "ForEach result incorrect: %d %d %d\n", a, b, c);
    }

    DListDestroy(list);
}

