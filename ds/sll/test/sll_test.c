#include <stdio.h>
#include <assert.h>
#include "sll.h"

/*-------------------------Functions Forward Decleration-------------------------*/
void TestCreateAndDestroy();
void TestInsertAndGet();
void TestSetData();
void TestRemove();
void TestSize();
void TestForEach();
void TestIterIsEqual();
void TestFind();

static int PrintInt(void* data, void* param);
static int IncrementBy(void* data, void* param);
static int IsMatch(const void* data1, const void* data2);

/*----------------------Main----------------------*/
int main()
{
	TestCreateAndDestroy();
	TestInsertAndGet();
	TestSetData();
	TestRemove();
	TestSize();
	TestForEach();
	TestIterIsEqual();
	TestFind();

	printf("\nAll tests completed\n");
	
	return 0;
}

/*----------------------Test Functions----------------------*/
void TestCreateAndDestroy()
{
	list_t* list = ListCreate();
	if (list == NULL || !ListIsEmpty(list))
	{
		printf("[FAIL] TestCreateAndDestroy\n");
		return;
	}
	ListDestroy(list);
	printf("[PASS] TestCreateAndDestroy\n");
}

void TestInsertAndGet()
{
	list_t* list = ListCreate();
	int val = 7;
	iter_t it = ListBegin(list);
	ListInsert(list, it, &val);
	if (*(int*)ListGetData(it) != 7)
	{
		printf("[FAIL] TestInsertAndGet\n");
		ListDestroy(list);
		return;
	}
	ListDestroy(list);
	printf("[PASS] TestInsertAndGet\n");
}

void TestSetData()
{
	list_t* list = ListCreate();
	int a = 1;
	int b = 99;
	iter_t it = ListBegin(list);
	ListInsert(list, it, &a);
	ListSetData(it, &b);
	if (*(int*)ListGetData(it) != 99)
	{
		printf("[FAIL] TestSetData\n");
		ListDestroy(list);
		return;
	}
	ListDestroy(list);
	printf("[PASS] TestSetData\n");
}

void TestRemove()
{
	list_t* list = ListCreate();
	int a = 10, b = 20;
	iter_t it = ListBegin(list);
	ListInsert(list, it, &a);
	it = ListNext(it);
	ListInsert(list, it, &b);
	ListRemove(ListBegin(list));
	if (*(int*)ListGetData(ListBegin(list)) != 20)
	{
		printf("[FAIL] TestRemove\n");
		ListDestroy(list);
		return;
	}
	ListDestroy(list);
	printf("[PASS] TestRemove\n");
}

void TestSize()
{
	list_t* list = ListCreate();
	int i, x;
	for (i = 0; i < 5; ++i)
	{
		x = i;
		ListInsert(list, ListBegin(list), &x);
	}
	if (ListSize(list) != 5)
	{
		printf("[FAIL] TestSize\n");
		ListDestroy(list);
		return;
	}
	ListDestroy(list);
	printf("[PASS] TestSize\n");
}

void TestForEach()
{
	list_t* list = ListCreate();
	int a = 1, b = 2, c = 3;
	int inc = 10;
	ListInsert(list, ListBegin(list), &c);
	ListInsert(list, ListBegin(list), &b);
	ListInsert(list, ListBegin(list), &a);

	printf("Before increment: ");
	ListForEach(ListBegin(list), ListEnd(list), PrintInt, NULL);
	printf("\n");

	ListForEach(ListBegin(list), ListEnd(list), IncrementBy, &inc);

	printf("After increment: ");
	ListForEach(ListBegin(list), ListEnd(list), PrintInt, NULL);
	printf("\n[PASS] TestForEach\n");

	ListDestroy(list);
}

void TestIterIsEqual()
{
	list_t* list = ListCreate();
	int x = 100;
	iter_t head = ListBegin(list);
	ListInsert(list, head, &x);
	if (!IterIsEqual(ListNext(head), ListNext(head)) || IterIsEqual(head, ListNext(head)))
	{
	  printf("[FAIL] TestIterIsEqual\n");
	  ListDestroy(list);
	  return;
	}
	ListDestroy(list);
	printf("[PASS] TestIterIsEqual\n");
}

void TestFind()
{
	list_t* list = ListCreate();
	int vals[] = {5, 10, 15, 20};
	int i;
	int target = 15;
	iter_t found;
	iter_t it = ListBegin(list);

	for (i = 0; i < 4; ++i)
	{
	  ListInsert(list, it, &vals[3 - i]);
	}

	found = ListFind(list, ListBegin(list), ListEnd(list), IsMatch, &target);
	if (*(int*)ListGetData(found) != 15)
	{
	  printf("[FAIL] TestFind\n");
	  ListDestroy(list);
	  return;
	}
	ListDestroy(list);
	printf("[PASS] TestFind\n");
}

/*----------------------Static Functions----------------------*/
static int PrintInt(void* data, void* param)
{
	(void)param;
	printf("%d ", *(int*)data);
	return 1;
}

static int IncrementBy(void* data, void* param)
{
	*(int*)data += *(int*)param;
	return 1;
}

static int IsMatch(const void* data1, const void* data2)
{
	return (*(int*)data1 == *(int*)data2);
}

