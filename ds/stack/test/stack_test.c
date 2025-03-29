/*----------------------includes----------------------*/
#include <stdio.h> /* printf */
#include "stack.h"

/*----------------------main----------------------*/
int main()
{
	size_t capacity = 5;
	size_t elem_size = sizeof(int);
	stack_t* stack = NULL;
	int values[] = {1, 2, 3, 4, 5};
	int* curr_elem = 0;
	int i = 0;
	
	/*----------------------Create()----------------------*/
	stack = Create(capacity, elem_size);
	
	/*----------------------Push()----------------------*/
	for (i = 0; i < 5; i++)
	{
		Push(stack, &values[i]);
	}
	
	/*----------------------Peek()----------------------*/
	printf("\033[1mTest Peek():\033[0m\n");	
	curr_elem = Peek(stack);
	printf("Top element: %d\n", *curr_elem);
	
	/*----------------------IsFull() = TRUE----------------------*/	
	printf("\n\033[1mTest IsFull() = TRUE:\033[0m\n");
	printf("Count = %lu\n", Count(stack));
	
	if (IsFull(stack) == 1)
	{
		printf("Stack is full\n");
	}
	else
	{
		printf("Stack is not full\n");
	}
	
	/*----------------------Pop()----------------------*/
	printf("\n\033[1mTest Pop():\033[0m\n");	
	Pop(stack);
	curr_elem = Peek(stack);
	printf("Peek after Pop(): %d\n", *curr_elem);
		
	/*----------------------IsFull() = FALSE----------------------*/	
	printf("\n\033[1mTest IsFull() = FALSE:\033[0m\n");
	if (IsFull(stack) == 1)
	{
		printf("Stack is full\n");
	}
	else
	{
		printf("Stack is not full\n");
	}
	
	/*----------------------IsEmpty() = FALSE----------------------*/	
	printf("\n\033[1mTest IsEmpty() = FALSE:\033[0m\n");
	if (IsEmpty(stack) == 1)
	{
		printf("Stack is empty\n");
	}
	else
	{
		printf("Stack is not empty\n");
	}
	
	/*----------------------Count()----------------------*/
	printf("\n\033[1mTest Count():\033[0m\n");
	printf("Count = %lu\n", Count(stack));	
	
	/*----------------------IsEmpty() = TRUE----------------------*/	
	printf("\n\033[1mTest IsEmpty() = TRUE:\033[0m\n");
	for (i = 0; i < 4; i++)
	{
		Pop(stack);
	}
	
	printf("Count = %lu\n", Count(stack));	
	
	if (IsEmpty(stack) == 1)
	{
		printf("Stack is empty\n");
	}
	else
	{
		printf("Stack is not empty\n");
	}
	
	/*----------------------Capacity()----------------------*/
	printf("\n\033[1mTest Capacity():\033[0m\n");
	printf("Capacity = %lu\n", Capacity(stack));	
	
	Destroy(stack);
	
	return (0);
}


