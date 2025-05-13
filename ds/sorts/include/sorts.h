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

#endif /*__ILRD_SORTS_H__*/
