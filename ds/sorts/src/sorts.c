/**
	Written By: Tom Golzman
	Date: 12/05/2025
	Reviewed By: Sami
**/

/************************************includes************************************/
#include <assert.h>	/* assert */
#include <stdlib.h>	/* malloc */

#include "utils.h"	/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM() */
#include "sorts.h"

/**********************Private Functions Forward Decleration**********************/
static void SwapInts(int* x, int* y);
static void CopyArray(int* arr1, int* arr2, size_t size);
static void CumulativeCount(int* arr, size_t size);
static int CountingSortByDigit(int* arr, size_t size, int exponent);
static int BinarySearchHelper(int* arr, size_t low, size_t high, int num, size_t* index_result);
static void MergeSortHelper(int* arr, int* temp, size_t low, size_t high);
static void Merge(int* arr, int* temp, size_t low, size_t mid, size_t high);
static void QuickSortHelper(char* base, size_t low, size_t high, size_t elem_size, int (*compar)(const void*, const void*));
static size_t Partition(char* base, size_t low, size_t high, size_t elem_size, int (*compar)(const void*, const void*));
static void Swap(void* x, void* y, size_t elem_size);
static void BuildMaxHeap(int* arr, size_t size);
static void HeapifyUp(int* arr, size_t index);
static void HeapifyDown(int* arr, size_t index, size_t size);
static size_t Max(int* arr, size_t size, size_t index1, size_t index2);
static size_t GetParentIdx(size_t current);
static size_t GetLeftChildIdx(size_t parent);
static size_t GetRightChildIdx(size_t parent);

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
	
	/* iterate each element lowing from the second */
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

int BinarySearchIterative(int* arr, size_t size, int num, size_t* index_result)
{
	size_t low = 0;
	size_t high = size - 1;
	size_t mid = 0;
	
	assert(NULL != arr);

	if (size < 1)
	{
		return (FALSE);
	}
	
	while (low <= high)
	{
		mid = low + (high - low) / 2;
		
		if (arr[mid] == num)
		{
			*index_result = mid;
			return (TRUE);
		}
		
		if (arr[mid] > num)
		{
			high = mid - 1;
		}
		
		else
		{
			low = mid + 1;
		}
	}
	
	return (FALSE);
}

int BinarySearchRecursive(int* arr, size_t size, int num, size_t* index_result)
{
	size_t low = 0;
	size_t high = size - 1;
	
	assert(NULL != arr);

	if (size < 1)
	{
		return (FALSE);
	}
	
	return (BinarySearchHelper(arr, low, high, num, index_result));
}

int MergeSort(int* arr, size_t size)
{
	size_t low = 0;
	size_t high = size - 1;
	int* temp = NULL;
	
	assert(NULL != arr);
	
	if (size < 1)
	{
		return (FAIL);
	}
	
	temp = (int*)malloc(size * sizeof(int));
	if (NULL == temp)
	{
		return (FAIL);
	}
	
	MergeSortHelper(arr, temp, low, high);
	
	free(temp);
	temp = NULL;
	
	return (SUCCESS);
}

void QuickSort(void* base, size_t num_of_elem, size_t elem_size, int (*compar)(const void*, const void*))
{
	size_t low = 0;
	size_t high = num_of_elem - 1;
	
	assert(NULL != base);
	assert(NULL != compar);
	
	if (num_of_elem < 1 || elem_size < 1)
	{
		return;
	}
	
	QuickSortHelper((char*)base, low, high, elem_size, compar);
}

void HeapSort(int* arr, size_t size)
{
	int* base = NULL;
	size_t i = 0;
	
	/* assert */
	assert(NULL != arr);
	assert(size > 0);
	
	base = --arr;
	
	/* build max heap */
	BuildMaxHeap(base, size);

	/* for each element in the heap - last to first */
	for (i = size; i > 1; --i)
	{
		/* swap the first element (biggest) with current */
		SwapInts(&base[1], &base[i]);

		/* heapify down the top */
		HeapifyDown(base, 1, i - 1);
	}
}

/************************************Private Functions************************************/
static void QuickSortHelper(char* base, size_t low, size_t high, size_t elem_size, int (*compar)(const void*, const void*))
{
	size_t pivot_index = 0;
	
	if (low < high)
	{
		pivot_index = Partition(base, low, high, elem_size, compar);
		
		if (pivot_index > 0)
		{
			QuickSortHelper(base, low, pivot_index - 1, elem_size, compar);
		}
		
		QuickSortHelper(base, pivot_index + 1, high, elem_size, compar);
	}
}

static size_t Partition(char* base, size_t low, size_t high, size_t elem_size, int (*compar)(const void*, const void*))
{
	char* pivot = base + (high * elem_size);
	size_t i = low;
	size_t j = low;
	
	for (i = low; i < high; ++i)
	{
		if (compar(base + (i * elem_size), pivot) < 0)
		{
			Swap(base + (j * elem_size), base + (i * elem_size), elem_size);
			++j;
		}
	}
	
	Swap(base + (j * elem_size), base + (high * elem_size), elem_size);
	
	return (j);
}

static void Swap(void* x, void* y, size_t elem_size)
{
	size_t i = 0;
	char* x_runner = (char*)x;
	char* y_runner = (char*)y;
	char temp = 0;
	
	for (i = 0; i < elem_size; ++i)
	{
		temp = x_runner[i];
		x_runner[i] = y_runner[i];
		y_runner[i] = temp;
	}
}

static void SwapInts(int* x, int* y)
{
	int temp = *x;
	*x = *y;
	*y = temp;
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

static int BinarySearchHelper(int* arr, size_t low, size_t high, int num, size_t* index_result)
{
	size_t mid = low + (high - low) / 2;
	
	if (arr[mid] == num)
	{
		*index_result = mid;		
		return (TRUE);
	}
	
	if (low >= high)
	{
		return (FALSE);
	}
	
	else if (arr[mid] > num)
	{
		return (BinarySearchHelper(arr, low, mid - 1, num, index_result));
	}
	
	
	else
	{
		return (BinarySearchHelper(arr, mid + 1, high, num, index_result));
	}
}

static void MergeSortHelper(int* arr, int* temp, size_t low, size_t high)
{
	size_t mid = low + (high - low) / 2;
	
	if (low >= high)
	{
		return;
	}
	
	MergeSortHelper(arr, temp, low, mid);
	MergeSortHelper(arr, temp, mid + 1, high);	
	Merge(arr, temp, low, mid, high);
}

static void Merge(int* arr, int* temp, size_t low, size_t mid, size_t high)
{
	size_t first_arr_runner = low;
	size_t second_arr_runner = mid + 1;
	size_t temp_index = low;
	
	/* merge to temp the sorted arrays */
	while (first_arr_runner <= mid && second_arr_runner <= high)
	{
		if (arr[first_arr_runner] <= arr[second_arr_runner])
		{
			temp[temp_index] = arr[first_arr_runner];
			++first_arr_runner;
		}
		else
		{
			temp[temp_index] = arr[second_arr_runner];
			++second_arr_runner;
		}
		++temp_index;
	}
	
	/* merge the rest in the first array */
	while (first_arr_runner <= mid)
	{
		temp[temp_index] = arr[first_arr_runner];
		++temp_index;
		++first_arr_runner;
	}
	
	/* merge the rest in the second array */
	while (second_arr_runner <= high)
	{
		temp[temp_index] = arr[second_arr_runner];
		++temp_index;
		++second_arr_runner;
	}
	
	/* copy from temp to the original array */
	for (first_arr_runner = low; first_arr_runner <= high; ++first_arr_runner)
	{
		arr[first_arr_runner] = temp[first_arr_runner];
	}
}

static void BuildMaxHeap(int* arr, size_t size)
{
	size_t i = 0;
	
	/* assert */
	assert(NULL != arr);
	assert(size > 0);
	
	/* for each element in array - last to first */
	for (i = (size / 2) ; i >= 1; --i)
	{
		/* heapify down */
		HeapifyDown(arr, i, size);
	}
}

static void HeapifyUp(int* arr, size_t curr)
{
	size_t parent = GetParentIdx(curr);
	
	/* asssert */
	assert(NULL != arr);
	assert(curr > 0);
	
	/* while current is before parent */
	while (curr > 1 && arr[curr] > arr[parent])
	{
		/* swap current and parent*/
		SwapInts(&arr[curr], &arr[parent]);
		
		/* update current and parent index */
		curr = parent;
		parent = GetParentIdx(curr);
	}
}

static void HeapifyDown(int* arr, size_t curr, size_t size)
{
	size_t left_child = GetLeftChildIdx(curr);
	size_t right_child = GetRightChildIdx(curr);
	size_t largest_child = 0;
	
	/* assert */
	assert(NULL != arr);
	assert(size > 0);
	assert(curr > 0);	
		
	/* find the largest child */
	largest_child = Max(arr, size, left_child, right_child);
	if (0 == largest_child)
	{
		return;
	}
	
	/* while current is samller than the largest child */
	while (arr[curr] < arr[largest_child])
	{
		/* swap the current with largest child */
		SwapInts(&arr[curr], &arr[largest_child]);
		
		/* update current and children index */
		curr = largest_child;
		left_child = GetLeftChildIdx(curr);
		right_child = GetRightChildIdx(curr);

		/* find the largest child of the new current */
		largest_child = Max(arr, size, left_child, right_child);
		if (0 == largest_child)
		{
			return;
		}
	}
}

static size_t GetParentIdx(size_t current)
{
	assert(current > 0);
	
	return (current / 2);
}

static size_t GetLeftChildIdx(size_t parent)
{
	assert(parent > 0);
	
	return (parent * 2);
}

static size_t GetRightChildIdx(size_t parent)
{
	assert(parent > 0);
	
	return ((parent * 2) + 1);
}

static size_t Max(int* arr, size_t size, size_t index1, size_t index2)
{
	assert(NULL != arr);
	
	if (index1 > size)
	{
		return (0);
	}
	
	if (index2 > size)
	{
		return (index1);
	}
	
	return (arr[index1] > arr[index2] ? index1 : index2);
}
