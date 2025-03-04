#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#pragma once

/* Exercise 1 */
void Swap (int* ptr1, int* ptr2);

/* Exercise 2 */
void* CpyArray(void* src, size_t elem_size, size_t num_elem, void* dest);

/* Exercise 3 */
void PrintAddress();

/* Exercise 4 */
void SwapSizeT(size_t* a, size_t* b);
void SwapPointers(size_t** ptr1, size_t** ptr2);

/* Exercise 5 */
void StrLen();
void StrCmp();


