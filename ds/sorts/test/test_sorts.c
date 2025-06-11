/************************************includes************************************/
#include <stdio.h> /* printf */
#include <stdlib.h> /* rand(), RAND_MAX, qsort()*/
#include <time.h> /* clock_t */
#include <string.h> /* memcmp() */

#include "test_utils.h"
#include "sorts.h"

/************************************define************************************/
#define SIZE 100

/************************************Functions Forward Decleration************************************/
void TestBubbleSort();
void TestSelectionSort();
void TestInsertionSort();
void TestCountingSort();
void TestRadixSort();
void TestBinarySearchIterative();
void TestBinarySearchRecursive();
void TestMergeSort();
void TestQuickSort();
void TestHeapSort();

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
	TestBinarySearchIterative();
	TestBinarySearchRecursive();
	TestMergeSort();
	TestQuickSort();
	TestHeapSort();
	
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
	
	printf(BOLD_TITLE "\nTest BuubleSort():\n" RESET);

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
		
	printf(BOLD_TITLE "\nTest SelectionSort():\n" RESET);
	
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
		
	printf(BOLD_TITLE "\nTest InsertionSort():\n" RESET);
	
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
		
	printf(BOLD_TITLE "\nTest CountingSort():\n" RESET);
	
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
		
	printf(BOLD_TITLE "\nTest RadixSort():\n" RESET);
	
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

void TestBinarySearchIterative()
{
	int arr1[SIZE];
	size_t i = 0;
	int num1 = 10;
	int num2 = 150;
	size_t expected_index_result1 = 10;
	int result = FALSE;
	size_t index_result = 0;
		
	printf(BOLD_TITLE "\nTest BinarySearch() Iterative:\n" RESET);
	
	for (i = 1; i < SIZE; i++)
	{
		arr1[i] = i;
	}
	
	result = BinarySearchIterative(arr1, (size_t)SIZE, num1, &index_result);
    
	if (result == TRUE && index_result == expected_index_result1)
	{
		printf(GREEN "Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed! result = %d, index_result = %lu\n" RESET, result, index_result);
	}
	
	result = BinarySearchIterative(arr1, (size_t)SIZE, num2, &index_result);
    
	if (result == FALSE)
	{
		printf(GREEN "Test 2 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 2 Failed! result = %d\n" RESET, result);
	}
}

void TestBinarySearchRecursive()
{
	int arr1[SIZE];
	size_t i = 0;
	int num1 = 10;
	int num2 = 150;
	size_t expected_index_result1 = 10;
	int result = FALSE;
	size_t index_result = 0;
		
	printf(BOLD_TITLE "\nTest BinarySearch() Recursive:\n" RESET);
	
	for (i = 1; i < SIZE; i++)
	{
		arr1[i] = i;
	}
	
	result = BinarySearchRecursive(arr1, (size_t)SIZE, num1, &index_result);
    
	if (result == TRUE && index_result == expected_index_result1)
	{
		printf(GREEN "Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed! result = %d, index_result = %lu\n" RESET, result, index_result);
	}
	
	result = BinarySearchRecursive(arr1, (size_t)SIZE, num2, &index_result);
    
	if (result == FALSE)
	{
		printf(GREEN "Test 2 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 2 Failed! result = %d\n" RESET, result);
	}
}

void TestMergeSort()
{
	int min_rand_val = 1000000;
	int max_rand_val = 9999999;
	int arr[SIZE];
	size_t i = 0;
	int status = 1;
	
	printf(BOLD_TITLE "\nTest MergeSort():\n" RESET);
	
	for (i = 0; i < SIZE; ++i)
	{
		arr[i] = min_rand_val + (rand() % (max_rand_val - min_rand_val + 1));
	}
	
	if (IsSorted(arr, SIZE))
	{
		printf(RED "ERROR!\n" RESET);	
	}
	
	status = MergeSort(arr, SIZE);
	
	if (0 == status && IsSorted(arr, SIZE))
	{
		printf(GREEN "Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed! status = %d, IsSorted = %d\n" RESET, status, IsSorted(arr, SIZE));
	}
}

void TestQuickSort()
{
	int min_rand_val = 1000000;
	int max_rand_val = 9999999;
	int arr1[SIZE];
	int arr2[SIZE];
	size_t i = 0;
	clock_t start, end;
	double my_qsort_time = 0, qsort_time = 0;
	
	printf(BOLD_TITLE "\nTest QuickSort():\n" RESET);
	
	for (i = 0; i < SIZE; ++i)
	{
		arr1[i] = min_rand_val + (rand() % (max_rand_val - min_rand_val + 1));
		arr2[i] = arr1[i];
	}
	
	start = clock();
	QuickSort(arr1, SIZE, sizeof(int), CompareInts);
    end = clock();
    my_qsort_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    start = clock();
	qsort(arr2, SIZE, sizeof(int), CompareInts);
    end = clock();
    qsort_time = (double)(end - start) / CLOCKS_PER_SEC;

	if (IsSorted(arr1, SIZE) && IsSorted(arr2, SIZE))
	{
		printf(GREEN "my time: %f | qsort time: %f\n" RESET, my_qsort_time, qsort_time);
	}
	else
	{
		printf(RED "Test 1 Failed! IsSorted = %d\n" RESET, IsSorted(arr1, SIZE));
	}
}

void TestHeapSort()
{
	int min_rand_val = 1000000;
	int max_rand_val = 9999999;
	int arr1[SIZE];
	size_t i = 0;
	
	printf(BOLD_TITLE "\nTest HeapSort():\n" RESET);
	
	for (i = 0; i < SIZE; ++i)
	{
		arr1[i] = min_rand_val + (rand() % (max_rand_val - min_rand_val + 1));
	}

	HeapSort(arr1, SIZE);

	if (IsSorted(arr1, SIZE))
	{
		printf(GREEN "Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
	}
}

/***************************Private Functions***************************/
static int IsSorted(int* arr, size_t size)
{
	size_t i = 0;
	
	for (i = 0; i < size - 1; i++)
	{
		if (arr[i] > arr[i + 1])
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
