#ifndef __ILRD_RECURSION_H__
#define __ILRD_RECURSION_H__

/************************************includes************************************/
#include "stack.h" /* stack_t */

/************************************typedef************************************/
typedef struct recursion_node {
	struct recursion_node* next;
	void* data;
} recursion_node_t;

/************************************Functions************************************/
int RecursiveFibonacci(int element_index);
int IterativeFibonacci(int element_index);
recursion_node_t* FlipList(recursion_node_t* node);
stack_t* SortStackRescursive(stack_t* stack);
size_t StrLen(const char* str);
int StrCmp(const char* str1, const char* str2);
char* StrCpy(char* dest, const char* src);
char* StrCat(char* dest, const char* src);
char* StrStr(const char* haystack, const char* needle);

#endif /*__ILRD_RECURSION_H__*/
