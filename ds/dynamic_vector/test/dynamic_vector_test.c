/************************************includes************************************/
#include "dynamic_vector.h"
#include <stdio.h> /* printf */

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define TITLE "\033[1;4m"

#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define FAIL 1

/************************************Functions Forward Decleration************************************/
void TestCreate();
void TestGetAccessToElem();
void TestPushBack();
void TestPopBack();
void TestSetElem();
void TestResize();
void TestShrinkToFit();

static void PrintVector(dvector_t* vector, size_t size);
static void InitVector(dvector_t* vector, int* arr, size_t size);

/************************************main************************************/
int main(void)
{
	TestCreate();
	TestGetAccessToElem();
	TestPushBack();
	TestPopBack();
	TestSetElem();
	TestResize();
	TestShrinkToFit();
	
	return (0);
}

/************************************Functions************************************/
void TestCreate()
{
	size_t capacity1 = 0;
	size_t elem_size1 = 4;
	dvector_t* vector1 = NULL;
		
	size_t capacity2 = 3;
	size_t elem_size2 = 4;
	dvector_t* vector2 = NULL;
	
	printf(TITLE "Test Create():\n" RESET);
	
	vector1 = VectorCreate(capacity1, elem_size1);
	vector2 = VectorCreate(capacity2, elem_size2);
	
	printf("vector1 = VectorCreate(0, 4);\n");
	printf("vector 1 elem_size = %lu\n", VectorGetElemSize(vector1));
	if((VectorCapacity(vector1) == DEFAULT_CAPACITY) && (VectorGetElemSize(vector1) == elem_size1))
	{
		printf(GREEN "TEST 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "TEST 1 FAILED!\n" RESET);
	}
	
	printf("vector2 = VectorCreate(3, 4);\n");
	printf("vector 2 elem_size = %lu\n", VectorGetElemSize(vector2));
	if((VectorCapacity(vector2) == capacity2) && (VectorGetElemSize(vector2) == elem_size2))
	{
		printf(GREEN "TEST 2 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "TEST 2 FAILED!\n" RESET);
	}
	
	VectorDestroy(vector1);
	VectorDestroy(vector2);
}

void TestGetAccessToElem()
{
	size_t capacity = 3;
	size_t elem_size = 4;
	dvector_t* vector = NULL;
	int* elem;
	int arr[] = {10, 20, 30};
	size_t index1 = 0, index2 = 2;
	
	printf(TITLE "\nTest GetAccessToElem():\n" RESET);
	
	vector = VectorCreate(capacity, elem_size);
	InitVector(vector, arr, capacity);
	PrintVector(vector, capacity);
	
	elem = VectorGetAccessToElem(vector, index1);
	printf("GetAccessToElem(vector, 0)\n");
	if (*elem == arr[index1])
	{
		printf(GREEN "TEST 1 PASSED!\n" RESET);
	}
	else
	{	
		printf("GetAccessToElem(vector, 0) = %d\n", *elem);
		printf(RED "TEST 1 FAILED!\n" RESET);
	}
	
	elem = VectorGetAccessToElem(vector, index2);
	printf("GetAccessToElem(vector, 2)\n");
	if (*elem == arr[index2])
	{
		printf(GREEN "TEST 2 PASSED!\n" RESET);
	}
	else
	{
		printf("GetAccessToElem(vector, 2) = %d\n", *elem);	
		printf(RED "TEST 2 FAILED!\n" RESET);
	}

	VectorDestroy(vector);
}

void TestPushBack()
{
	size_t capacity = 3;
	size_t elem_size = 4;
	size_t size = 3;
	dvector_t* vector = NULL;
	int arr[] = {10, 20, 30};
	int elem_to_push = 40;
	
	printf(TITLE "\nTest PushBack():\n" RESET);
	
	vector = VectorCreate(capacity, elem_size);
	InitVector(vector, arr, capacity);
	PrintVector(vector, capacity);
	
	printf("VectorPushBack(vector, 40)\n");
	if ((VectorPushBack(vector, &elem_to_push) == SUCCESS) && (VectorSize(vector) == size + 1) && (VectorCapacity(vector) == capacity * 2))
	{
		PrintVector(vector, VectorSize(vector));
		printf(GREEN "TEST PASSED!\n" RESET);
	}
	else
	{	
		PrintVector(vector, VectorSize(vector));
		printf("size = %lu, capacity = %lu\n", VectorSize(vector), VectorCapacity(vector));
		printf(RED "TEST FAILED!\n" RESET);
	}

	VectorDestroy(vector);
}

void TestPopBack()
{
	size_t capacity = 3;
	size_t elem_size = 4;
	size_t size = 3;
	dvector_t* vector = NULL;
	int arr[] = {10, 20, 30};
	
	printf(TITLE "\nTest PopBack():\n" RESET);
	
	vector = VectorCreate(capacity, elem_size);
	InitVector(vector, arr, capacity);
	PrintVector(vector, capacity);
	
	printf("Vector size before PopBack = %lu\n", VectorSize(vector));	
	printf("VectorPopBack(vector, 40)\n");
	VectorPopBack(vector);
	printf("Vector size after PopBack = %lu\n", VectorSize(vector));
	if (VectorSize(vector) == size - 1)
	{
		PrintVector(vector, VectorSize(vector));
		printf(GREEN "TEST PASSED!\n" RESET);
	}
	else
	{	
		PrintVector(vector, VectorSize(vector));
		printf("size = %lu\n", VectorSize(vector));
		printf(RED "TEST FAILED!\n" RESET);
	}

	VectorDestroy(vector);
}

void TestSetElem()
{
	size_t capacity = 3;
	size_t elem_size = 4;
	dvector_t* vector = NULL;
	int arr[] = {10, 20, 30};
	int elem_to_set = 100;
	
	printf(TITLE "\nTest SetElem():\n" RESET);
	
	vector = VectorCreate(capacity, elem_size);
	InitVector(vector, arr, capacity);
	PrintVector(vector, capacity);
	
	printf("VectorSetElem(vector, 2, 100)\n");
	VectorSetElem(vector, (size_t)2, &elem_to_set);
	if (*(int*)VectorGetAccessToElem(vector, 2) == elem_to_set)
	{
		PrintVector(vector, 3);
		printf(GREEN "TEST PASSED!\n" RESET);
	}
	else
	{	
		PrintVector(vector, VectorSize(vector));
		printf("size = %lu\n", VectorSize(vector));
		printf(RED "TEST FAILED!\n" RESET);
	}

	VectorDestroy(vector);	
}

void TestResize()
{
	size_t capacity = 10;
	size_t elem_size = 4;
	dvector_t* vector = NULL;
	int arr[] = {10, 20, 30};
	size_t new_capacity = 5;
	
	printf(TITLE "\nTest Resize():\n" RESET);
	
	vector = VectorCreate(capacity, elem_size);
	InitVector(vector, arr, 3);
	printf("Vector capacity before resize = %lu\n", VectorCapacity(vector));
	
	printf("VectorResize(vector, 5)\n");
	VectorResize(vector, new_capacity);
	printf("Vector capacity after resize = %lu\n", VectorCapacity(vector));
	if (VectorCapacity(vector) == new_capacity)
	{
		printf(GREEN "TEST PASSED!\n" RESET);
	}
	else
	{	
		PrintVector(vector, 3);
		printf(RED "TEST FAILED!\n" RESET);
	}

	VectorDestroy(vector);		
}

void TestShrinkToFit()
{
	size_t capacity = 10;
	size_t elem_size = 4;
	dvector_t* vector = NULL;
	int arr[] = {10, 20, 30};
	size_t new_capacity = 6;
	
	printf(TITLE "\nTest ShrinkToFit():\n" RESET);
	
	vector = VectorCreate(capacity, elem_size);
	InitVector(vector, arr, 3);
	printf("Vector capacity before ShrinkToFit = %lu\n", VectorCapacity(vector));
	
	printf("ShrinkToFit(vector)\n");
	VectorShrinkToFit(vector);
	printf("Vector capacity after ShrinkToFit = %lu\n", VectorCapacity(vector));
	if (VectorCapacity(vector) == new_capacity)
	{
		printf(GREEN "TEST PASSED!\n" RESET);
	}
	else
	{	
		PrintVector(vector, 3);
		printf(RED "TEST FAILED!\n" RESET);
	}

	VectorDestroy(vector);		
}

static void PrintVector(dvector_t* vector, size_t size)
{
	int* elem;
	size_t i = 0;

	printf("Vector elements: ");

	for (i = 0; i < size; i++)
	{
		elem = (int*)VectorGetAccessToElem(vector, i);
		printf("%d ", *elem);
	}

	printf("\n");
}

static void InitVector(dvector_t* vector, int* arr, size_t size)
{
	size_t i = 0;
	
	for (i = 0; i < size; i++)
	{
		VectorPushBack(vector, &arr[i]);
	}
}

