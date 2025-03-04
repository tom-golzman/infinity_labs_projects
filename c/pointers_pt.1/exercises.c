#include <stdio.h>

/* Exercise 1 */
void Swap(int* ptr1, int* ptr2)
{
	int tmp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = tmp;
}

/* Exercise 2 */
void* CpyArray(void* src, size_t elem_size, size_t num_elem, void* dest)
{
	if (src == NULL || dest == NULL)
	{
		return NULL;
	}
	
	return memcpy(dest, src, elem_size * num_elem);	
}

/* Exercise 3 */
void PrintAddress()
{
	static int s_i =7;
	int i = 7;
	int* ptr = &i;
	int* ptr2 = (int*)malloc(sizeof(int));

	printf("\nAddress of i: %p\n", (void*)&s_i);
	printf("Address of i: %p\n", (void*)&i);
	printf("Address of ptr: %p\n", (void*)&ptr);
	printf("Address of ptr2: %p\n", (void*)&ptr2);

	if (ptr)
	{
		int** ptr3 = &ptr;	
		printf("Address of ptr3: %p\n", (void*)&ptr3);
	}

	free (ptr2);
}

/* Exercise 4 */
void SwapSizeT(size_t* a, size_t* b)
{
	size_t temp = *a;
	*a = *b;
	*b = temp;
}

void SwapPointers(size_t** a, size_t** b)
{
	size_t* p1 = (size_t*)a;
	size_t* p2 = (size_t*)b;
	
	SwapSizeT(p1, p2);
}

/* Exercise strings */
size_t StrLen(const char* str)
{
	size_t len = 0;
	
	assert(str != NULL);
	
	while (str[len] != '\0')
	{
		len++;
	}
	
	return len;
}

int StrCmp(const char* str1, const char* str2)
{
	int diff;
	
	assert(str1 != NULL);
	assert(str2 != NULL);	
	
	while ((*str1 != '\0') && (*str2 != '\0') && (*str1 == *str2))
	{
		str1++;
		str2++;
	}
	
	diff = *str1 - *str2;
	
	return (diff);
}



















