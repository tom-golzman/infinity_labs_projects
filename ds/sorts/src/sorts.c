/**
	Written By: Tom Golzman
	Date: 12/05/2025
	Reviewed By: 
**/

/************************************includes************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "sorts.h"

/*************************************define*************************************/
#define SUCCESS (0)
#define FAIL (1)

/**********************Private Functions Forward Decleration**********************/
static void SwapInts(int* x, int* y);
static void CopyArray(int* arr1, int* arr2, size_t size);
static void CumulativeCount(int* arr, size_t size);
static int CountingSortByDigit(int* arr, size_t size, int exponent);

/************************************Functions************************************/
void BubbleSort(int* arr, size_t size)
{
	size_t i = 0, j = 0;
	
	assert(NULL != arr);
	
	for (i = 0; i < size - 1; i++)
	{
		/* inner loop compares pair of elements */
		for (j = 0; j < size - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				/* swap pair of elements if they are in wrong order */
				SwapInts(&arr[j], &arr[j + 1]);
			}
		}
	}
}

void SelectionSort(int* arr, size_t size)
{
	size_t i = 0, j = 0;
	size_t min_index = 0;
	
	assert(NULL != arr);
	
	/* move the boundary of the unsorted sub-array */
	for (i = 0; i < size - 1; i++)
	{
		min_index = i;
		
		/* finds the index of the minimum element in the unsorted part in the array */
		for (j = i + 1; j < size; j++)
		{
			if (arr[j] < arr[min_index])
			{
				min_index = j;
			}
		}
		
		/* swaps the found minimum element with the first unsorted element */
		if (min_index != i)
		{
			SwapInts(&arr[i], &arr[min_index]);
		}
	}
}

void InsertionSort(int* arr, size_t size)
{
	size_t i = 0;
	int temp = 0;
	int j = 0;
	
	assert(NULL != arr);
	
	/* iterate each element starting from the second */
	for (i = 1; i < size; i++)
	{
		/* swap backwards while previous elements are bigger */
		for (j = i; (j > 0) && (arr[j - 1] > arr[j]); j--)
		{
			temp = arr[j];
			arr[j] = arr[j - 1];
			arr[j - 1] = temp;
		}
	}
}

int CountingSort(int* arr, size_t size)
{
	int max_val = 0, min_val = 0, val = 0;
	int range = 0;
	size_t i = 0;
	int* count = NULL;
	int* output = NULL;
	
	assert(NULL != arr);
	
	output = (int*)malloc(size * sizeof(int));
	if (NULL == output)
	{
		return (FAIL);
	}
	
	/* find the maximum and minimum values */
	max_val = arr[0];
	min_val = arr[0];
	for (i = 1; i < size; i++)
	{
		if (arr[i] > max_val)
		{
			max_val = arr[i];
		}
		else if (arr[i] < min_val)
		{
			min_val = arr[i];
		}
	}
	
	range = max_val - min_val + 1;

	count = (int*)calloc((size_t)range, sizeof(int));
	if (NULL == count)
	{
		free(output);
		output = NULL;
		
		return (FAIL);	
	}
	
	/* count occurrences of each value */
	for (i = 0; i < size; i++)
	{
		count[arr[i] - min_val]++;
	}
	
	/* cumulative count of count[] */
	CumulativeCount(count, range);
	
	/* build output[] from right to left */
	for (i = size; i > 0; i--)
	{
		val = arr[i - 1];
		output[count[val - min_val] - 1] = val;
		count[val - min_val]--;
	}
	
	/* copy from output[] to arr[] (original array) */
	CopyArray(arr, output, size);
	
	free(output);
	output = NULL;
	free(count);
	count = NULL;
	
	return (SUCCESS);
}

int RadixSort(int* arr, size_t size)
{
	size_t i = 0;
	int exponent = 0;
	int max_val = 0;
	
	assert(NULL != arr);
	
	/* find the maximum value */
	for (i = 0; i < size; i++)
	{
		if (arr[i] > max_val)
		{
			max_val = arr[i];
		}
	}
	
	for (exponent = 1; (max_val / exponent) > 0; exponent *= 10)
	{
		if (FAIL == CountingSortByDigit(arr, size, exponent))
		{
			return (FAIL);
		}
	}
	
	return (SUCCESS);
}

/***************************Private Functions***************************/
static void SwapInts(int* x, int* y)
{
	if (x == y)
	{
		return;
	}
	
	*x = *x + *y;
	*y = *x - *y;
	*x = *x - *y;
}

static void CopyArray(int* arr1, int* arr2, size_t size)
{
	size_t i = 0;
	
	for (i = 0; i < size; i++)
	{
		arr1[i] = arr2[i];
	}	
}

static void CumulativeCount(int* arr, size_t size)
{
	size_t i = 0;
	
	for (i = 1; i < size; i++)
	{
		arr[i] += arr[i - 1];
	}
}

static int CountingSortByDigit(int* arr, size_t size, int exponent)
{
	size_t i = 0;
	int count[10] = {0}; /* digits 0-9 */
	int digit = 0, val = 0;
	int* output = (int*)malloc(size * sizeof(int));
	if (NULL == output)
	{
		return (FAIL);
	}
	
	/* count occurrences of digits at current digit position */
	for (i = 0; i < size; i++)
	{
		digit = (arr[i] / exponent) % 10;
		count[digit]++;
	}

	/* cumulative count of count[] */
	CumulativeCount(count, 10);
	
	/* build output[] from right to left */
	for (i = size; i > 0; i--)
	{
		val = arr[i - 1];
		digit = (val / exponent) % 10;
		output[count[digit] - 1] = val;
		count[digit]--;
	}
	
	/* copy from output[] to arr[] (original array) */
	CopyArray(arr, output, size);
	
	free(output);
	output = NULL;
	
	return (SUCCESS);
}
