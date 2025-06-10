#ifndef __ILRD_SORTS_H__
#define __ILRD_SORTS_H__

/************************************includes************************************/
#include <stddef.h> /* size_t */

/************************************typedef************************************/

/************************************define************************************/

/************************************Functions************************************/
void BubbleSort(int* arr, size_t size);
void SelectionSort(int* arr, size_t size);
void InsertionSort(int* arr, size_t size);
int CountingSort(int* arr, size_t size);
int RadixSort(int* arr, size_t size);
int BinarySearchIterative(int* arr, size_t size, int num, size_t* index_result);
int BinarySearchRecursive(int* arr, size_t size, int num, size_t* index_result);
int MergeSort(int* arr, size_t size);
void QuickSort(void* base, size_t num_of_elem, size_t elem_size, int (*compar)(const void*, const void*));
void HeapSort(void* base, size_t num_of_elem, size_t elem_size, int (*compar)(const void*, const void*));

#endif /*__ILRD_SORTS_H__*/
