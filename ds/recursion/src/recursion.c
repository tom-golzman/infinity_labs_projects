/**
	Written By: Tom Golzman
	Date: 25/05/2025
	Reviewed By: Sami
**/

/************************************includes************************************/
#include <stddef.h> /* NULL */
#include <assert.h> /* assert */
#include <string.h> /* strncmp() */

#include "recursion.h"

/************************************Private Functions************************************/
static int FibonacciWrapper(int elem, int prev_prev, int prev);
static void SortStackHelper(stack_t* stack, int* curr);
static void StrCatHelper(char* dest, const char* src);
static size_t StrLenHelper(const char* str, size_t size);
static char* StrStrHelper(const char* haystack, const char* needle, size_t needle_size);

/************************************Functions************************************/
int RecursiveFibonacci(int element_index)
{
	if (1 == element_index)
	{
		return (1);
	}

	return (FibonacciWrapper(element_index, 0, 1));
}

static int FibonacciWrapper(int elem, int prev_prev, int prev)
{
	if (elem == 1)
	{
		return (prev);
	}
	
	return (FibonacciWrapper(elem - 1, prev, prev_prev + prev));
}

int IterativeFibonacci(int element_index)
{
	int i = 0;
	int sum = 1;
	int prev = 1;
	int prev_prev = 0;
	
	for (i = 1; i < element_index; i++)
	{
		sum = prev + prev_prev;
		prev_prev = prev;
		prev = sum;
		
	}
	
	return (sum);
}

recursion_node_t* FlipList(recursion_node_t* node)
{
	recursion_node_t* new_head = NULL;
	
	assert(NULL != node);
	
	if (NULL == node || NULL == node->next)
	{
		return (node);
	}
	
	new_head = FlipList(node->next);
	
	node->next->next = node;
	node->next = NULL;
	
	return (new_head);
}

stack_t* SortStackRescursive(stack_t* stack)
{
	int* popped = NULL;
	
	if (Count(stack) <= 1)
	{
		return (stack);
	}	
	
	popped = (int*)Pop(stack);
	
	SortStackRescursive(stack);
	
	SortStackHelper(stack, popped);
	
	return (stack);
}

static void SortStackHelper(stack_t* stack, int* curr)
{
	int popped = 0;
	
	if (IsEmpty(stack) || *(int*)Peek(stack) < *curr)
	{
		Push(stack, curr);
		return;
	}
	
	popped = *(int*)Pop(stack);
	
	SortStackHelper(stack, curr);
	
	Push(stack, &popped);
}

size_t StrLen(const char* str)
{
	return (StrLenHelper(str, 0));
}

static size_t StrLenHelper(const char* str, size_t size)
{
	if (*str == '\0')
	{
		return (size);
	}
	
	str++;
	
	return (StrLenHelper(str, size + 1));
}

int StrCmp(const char* str1, const char* str2)
{
	if (*str1 == '\0' || *str1 != *str2)
	{
		return (*str1 - *str2);
	}
	
	str1++;
	str2++;
	
	return (StrCmp(str1, str2));
}

char* StrCpy(char* dest, const char* src)
{
	if (*src == '\0')
	{
		*dest = '\0';
		
		return (dest);
	}
	
	*dest = *src;
	dest++;
	src++;
	
	return (StrCpy(dest, src));
}

char* StrCat(char* dest, const char* src)
{
	StrCatHelper(dest, src);

	return (dest);
}

static void StrCatHelper(char* dest, const char* src)
{
	if (*dest == '\0')
	{
		StrCpy(dest, src);
		return;
	}
	
	dest++;
	
	StrCatHelper(dest, src);
}

char* StrStr(const char* haystack, const char* needle)
{
	size_t needle_size = StrLen(needle);
	
	if (needle_size == 0)
	{
		return ((char*)haystack);
	}
	
	return (StrStrHelper(haystack, needle, needle_size));
}

static char* StrStrHelper(const char* haystack, const char* needle, size_t needle_size)
{
	if (*haystack == '\0')
	{
		return (NULL);
	}
	
	if (0 == strncmp(haystack, needle, needle_size))
	{
		return ((char*)haystack);
	}
	
	haystack++;
	
	return (StrStrHelper(haystack, needle, needle_size));
}
