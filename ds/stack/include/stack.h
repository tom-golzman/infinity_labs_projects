#pragma once

/*----------------------includes----------------------*/
#include <stddef.h> /* size_t */	

/*---------------------typedef---------------------*/
typedef struct stack_internal_t stack_t;

/*---------------------Functions Forward Decleration---------------------*/
stack_t* Create(size_t capacity, size_t elem_size);
void Push(stack_t* s, void* data);
void* Pop(stack_t* s);
void* Peek(const stack_t* s);
int IsEmpty(const stack_t* s);
int IsFull(const stack_t* s); 
size_t Count(const stack_t* s);
size_t Capacity(const stack_t *stack);
void Destroy(stack_t* s);
