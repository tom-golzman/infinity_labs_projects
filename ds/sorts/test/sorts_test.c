/************************************includes************************************/
#include <stdio.h> /* printf */
#include <stdlib.h> /* rand(), RAND_MAX, qsort()*/
#include <time.h> /* clock_t */
#include <string.h> /* memcmp() */

#include "sorts.h"

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define BOLD_TITLE "\033[1;4m"
#define TITLE "\033[4m"

#define TRUE (1)
#define FALSE (0)

#define SIZE 5000

/************************************Functions Forward Decleration************************************/
void TestBubbleSort();
void TestSelectionSort();
void TestInsertionSort();
void TestCountingSort();
void TestRadixSort();

static int IsSorted(int* arr, size_t size);
static int CompareInts(const void* x, const void* y);

/************************************main************************************/
int main(void)
{
	TestBubbleSort();
	TestSelectionSort();
	TestInsertionSort();
	TestCountingSort();
	TestRadixSort();
	
	printf("\n");
	
	return (0);
}

/************************************Functions************************************/
void TestBubbleSort()
{
	int arr1[SIZE];
	int arr2[SIZE];
	size_t i = 0;
	clock_t start, end;
	double bubble_time = 0, qsort_time = 0;
	
	printf(TITLE "\nTest BuubleSort():\n" RESET);

	for (i = 0; i < SIZE; i++)
	{
		arr1[i] = (rand() % 10000) - ((RAND_MAX % 10000) / 2);
		arr2[i] = arr1[i];
	}
	
    start = clock();
	BubbleSort(arr1, SIZE);
    end = clock();
    bubble_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    start = clock();
	qsort(arr2, SIZE, sizeof(int), CompareInts);
    end = clock();
    qsort_time = (double)(end - start) / CLOCKS_PER_SEC;
	
	if (IsSorted(arr1, SIZE) && IsSorted(arr2, SIZE))
	{
		printf(GREEN "bubble time: %.3f | qsort time: %.3f\n" RESET, bubble_time, qsort_time);
	}
	else
	{
		printf(RED "Test Failed!\n" RESET);
	}
}

void TestSelectionSort()
{
	int arr1[SIZE];
	int arr2[SIZE];
	size_t i = 0;
	clock_t start, end;
	double selection_time = 0, qsort_time = 0;
		
	printf(TITLE "\nTest SelectionSort():\n" RESET);
	
	for (i = 0; i < SIZE; i++)
	{
		arr1[i] = (rand()) - ((RAND_MAX) / 2);
		arr2[i] = arr1[i];
	}
	
    start = clock();
	SelectionSort(arr1, SIZE);
    end = clock();
    selection_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    start = clock();
	qsort(arr2, SIZE, sizeof(int), CompareInts);
    end = clock();
    qsort_time = (double)(end - start) / CLOCKS_PER_SEC;
    	
	if (IsSorted(arr1, SIZE) && IsSorted(arr2, SIZE))
	{
		printf(GREEN "selection time: %.3f | qsort time: %.3f\n" RESET, selection_time, qsort_time);
	}
	else
	{
		printf(RED "Test Failed!\n" RESET);
	}
}

void TestInsertionSort()
{
	int arr1[SIZE];
	int arr2[SIZE];
	size_t i = 0;
	clock_t start, end;
	double insertion_time = 0, qsort_time = 0;
		
	printf(TITLE "\nTest InsertionSort():\n" RESET);
	
	for (i = 0; i < SIZE; i++)
	{
		arr1[i] = (rand()) - ((RAND_MAX) / 2);
		arr2[i] = arr1[i];
	}
	
    start = clock();
	InsertionSort(arr1, SIZE);
    end = clock();
    insertion_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    start = clock();
	qsort(arr2, SIZE, sizeof(int), CompareInts);
    end = clock();
    qsort_time = (double)(end - start) / CLOCKS_PER_SEC;
    	
	if (IsSorted(arr1, SIZE) && IsSorted(arr2, SIZE))
	{
		printf(GREEN "insertion time: %.3f | qsort time: %.3f\n" RESET, insertion_time, qsort_time);
	}
	else
	{
		printf(RED "Test Failed!\n" RESET);
	}
}

void TestCountingSort()
{
	int arr1[SIZE];
	int arr2[SIZE];
	size_t i = 0;
	clock_t start, end;
	double counting_time = 0, qsort_time = 0;
		
	printf(TITLE "\nTest CountingSort():\n" RESET);
	
	for (i = 0; i < SIZE; i++)
	{
		arr1[i] = (rand() % (200 + 1)) - 100;
		arr2[i] = arr1[i];
	}
	
    start = clock();
	CountingSort(arr1, SIZE);
    end = clock();
    counting_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    start = clock();
	qsort(arr2, SIZE, sizeof(int), CompareInts);
    end = clock();
    qsort_time = (double)(end - start) / CLOCKS_PER_SEC;
    	
	if (IsSorted(arr1, SIZE) && IsSorted(arr2, SIZE))
	{
		printf(GREEN "counting time: %f | qsort time: %f\n" RESET, counting_time, qsort_time);
	}
	else
	{
		printf(RED "Test Failed!\n" RESET);
	}	
}

void TestRadixSort()
{
	int min_rand_val = 1000000;
	int max_rand_val = 9999999;
	int arr1[SIZE];
	int arr2[SIZE];
	size_t i = 0;
	clock_t start, end;
	double radix_time = 0, qsort_time = 0;
		
	printf(TITLE "\nTest RadixSort():\n" RESET);
	
	for (i = 0; i < SIZE; i++)
	{
		arr1[i] = min_rand_val + (rand() % (max_rand_val - min_rand_val + 1));
		arr2[i] = arr1[i];
	}
	
    start = clock();
	RadixSort(arr1, SIZE);
    end = clock();
    radix_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    start = clock();
	qsort(arr2, SIZE, sizeof(int), CompareInts);
    end = clock();
    qsort_time = (double)(end - start) / CLOCKS_PER_SEC;
    	
	if (IsSorted(arr1, SIZE) && IsSorted(arr2, SIZE))
	{
		printf(GREEN "radix time: %f | qsort time: %f\n" RESET, radix_time, qsort_time);
	}
	else
	{
		printf(RED "Test Failed!\n" RESET);
	}	
}

/***************************Private Functions***************************/
static int IsSorted(int* arr, size_t size)
{
	size_t i = 0;
	
	for (i = 1; i < size; i++)
	{
		if (arr[i - 1] > arr[i])
		{
			return (FALSE);
		}
	}
	
	return (TRUE);
}

static int CompareInts(const void* x, const void* y)
{
    return (*(int*)x - *(int*)y);
}
