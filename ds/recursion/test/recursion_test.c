/************************************includes************************************/
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "recursion.h"

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define BOLD_TITLE "\033[1;4m"
#define TITLE "\033[4m"

/************************************Functions Forward Decleration************************************/
void TestRecursiveFibonacci();
void TestIterativeFibonacci();
void TestFlipList();
void TestSortStack();
void TestStrLen();
void TestStrCmp();
void TestStrCpy();
void TestStrCat();
void TestStrStr();

static void PrintList(const recursion_node_t* head);
static recursion_node_t* NodeCreate(int* data);
static void ListDestroy(recursion_node_t* head);
static void PrintStack(stack_t* stack);

/************************************main************************************/
int main(void)
{
	TestRecursiveFibonacci();
	TestIterativeFibonacci();
	TestFlipList();
	TestSortStack();
	TestStrLen();
	TestStrCmp();
	TestStrCpy();
	TestStrCat();
	TestStrStr();
	
	printf("\n");
	
	return (0);
}

/************************************Functions************************************/
void TestRecursiveFibonacci()
{
	printf(TITLE "\nTest: RecursiveFibonacci()\n" RESET);

	
	if (RecursiveFibonacci(1) == 1)
	{
		printf(GREEN "Test 2 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 2 FAILED!\n" RESET);
	}
	
	if (RecursiveFibonacci(2) == 1)
	{
		printf(GREEN "Test 3 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 3 FAILED!\n" RESET);
	}

	if (RecursiveFibonacci(3) == 2)
	{
		printf(GREEN "Test 4 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 4 FAILED!\n" RESET);
	}
	
	if (RecursiveFibonacci(5) == 5)
	{
		printf(GREEN "Test 5 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 5 FAILED!\n" RESET);
	}
	
	if (RecursiveFibonacci(10) == 55)
	{
		printf(GREEN "Test 6 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 6 FAILED!\n" RESET);
	}
}

void TestIterativeFibonacci()
{
	printf(TITLE "\nTest: IterativeFibonacci()\n" RESET);

	
	if (IterativeFibonacci(1) == 1)
	{
		printf(GREEN "Test 2 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 2 FAILED!\n" RESET);
	}
	
	if (IterativeFibonacci(2) == 1)
	{
		printf(GREEN "Test 3 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 3 FAILED!\n" RESET);
	}

	if (IterativeFibonacci(3) == 2)
	{
		printf(GREEN "Test 4 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 4 FAILED!\n" RESET);
	}
	
	if (IterativeFibonacci(5) == 5)
	{
		printf(GREEN "Test 5 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 5 FAILED!\n" RESET);
	}
	
	if (IterativeFibonacci(10) == 55)
	{
		printf(GREEN "Test 6 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 6 FAILED!\n" RESET);
	}
}

void TestFlipList()
{
	int x = 1, y = 2, z = 3;
	recursion_node_t* new_head = NULL;
	
	recursion_node_t* node1 = NodeCreate(&x);
	node1->next = NodeCreate(&y);
	node1->next->next = NodeCreate(&z);
	
	printf(TITLE "\nTest: FlipList()\n" RESET);
		
	PrintList(node1);
	
	new_head = FlipList(node1);
	
	PrintList(new_head);
	
	ListDestroy(new_head);
}

recursion_node_t* NodeCreate(int* data)
{
	recursion_node_t* node = (recursion_node_t*)malloc(sizeof(recursion_node_t));
	if (NULL == node)
	{
		return (NULL);
	}
	
	node->next = NULL;
	node->data = data;
	
	return (node);
}

void ListDestroy(recursion_node_t* head)
{
    recursion_node_t* current = NULL;
    recursion_node_t* next = NULL;

    current = head;
    
    while (NULL != current)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

void PrintList(const recursion_node_t* head)
{
    printf("List: ");
    while (head != NULL)
    {
        printf("%d -> ", *(int*)(head->data));
        head = head->next;
    }
    printf("NULL\n");
}

void TestSortStack()
{
	stack_t* stack = Create(4, sizeof(int));
	
	int a = 2, b = 1, c = 4, d = 0;
	
	printf(TITLE "\nTest: FlipList()\n" RESET);
		
	Push(stack, &a);
	Push(stack, &b);
	Push(stack, &c);
	Push(stack, &d);
	
	PrintStack(stack);

	SortStackRescursive(stack);
	
	printf("\n");
	
	PrintStack(stack);
}

static void PrintStack(stack_t* stack)
{
    size_t count = Count(stack);
    size_t i = 0;
    int* temp_arr = (int*)malloc(count * sizeof(int));
	int* data = NULL;
	
    if (NULL == temp_arr)
    {
        printf("Memory allocation failed\n");
        return;
    }

    printf("Stack: \n");

    for (i = 0; i < count; ++i)
    {
        data = (int*)Pop(stack);
        temp_arr[i] = *data;
        printf("%d\n", temp_arr[i]);
    }

    for (i = count; i > 0; --i)
    {
        Push(stack, &temp_arr[i-1]);
    }

    free(temp_arr);
}

void TestStrLen()
{
	char str1[] = "abc\0";
	char str2[] = "\0";
	size_t str1_size = StrLen(str1);
	size_t str2_size = StrLen(str2);
	
	printf(TITLE "\nTest: StrLen()\n" RESET);
		
	if (str1_size == 3)
	{
		printf(GREEN "Test 1 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 FAILED! size:%lu\n" RESET, str1_size);
	}
	
	if (str2_size == 0)
	{
		printf(GREEN "Test 2 PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test 2 FAILED!\n" RESET);
	}
}

void TestStrCmp()
{
	char str1[] = "EQUAL";
	char str2[] = "EQUAL";
	char str3[] = "NOT EQUAL";
	char str4[] = "NOT EQUA";
	
	printf(TITLE "\nTest: StrCmp()\n" RESET);
	
	if(StrCmp(str1,str2) == 0)
	{
		printf(GREEN "Test 1 PASSED!\n" RESET);
	}
	
	else
	{
		printf(RED "Test 1 FAILED!\n" RESET);
	}
	
	if(StrCmp(str3,str4) == 76)
	{
		printf(GREEN "Test 2 PASSED!\n" RESET);
	}
	
	else
	{
		printf(RED "Test 2 FAILED!\n" RESET);
	}
}

void TestStrCpy()
{
	char src[] = "copy this\0";
	char dest[] = "here here here\0";
	
	printf(TITLE "\nTest: StrCpy()\n" RESET);
	
	printf("src: %s | dest: %s\n", src, dest);
	StrCpy(dest, src);
	printf("My StrCpy => src: %s | dest: %s\n", src, dest);
}

void TestStrCat()
{
	char src[] = "here";
	char dest[11] = "copied ";
	char* result = StrCat(dest, src);
	
	printf(TITLE "\nTest: StrCat()\n" RESET);
	printf("dest: %s | src: %s\n", dest, src);
	printf("Result => %s\n", result);
	if(StrCmp(result, "copied here") == 0)
	{
		printf(GREEN "Test 1 PASSED!\n" RESET);
	}
	
	else
	{
		printf(RED "Test 1 FAILED!\n" RESET);
	}
}

void TestStrStr()
{
	char haystack[] = "1234aabc5678";
	char needle[] = "abc";
	char* result = StrStr(haystack, needle);
	
	printf(TITLE "\nTest: StrStr()\n" RESET);
	printf("haystack: %s | needle: %s\n", haystack, needle);
	
	printf("result: %s\n", result);
}
