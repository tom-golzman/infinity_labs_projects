/************************************includes************************************/
#include "sll_ex.h"
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define TITLE "\033[1;4m"

/************************************Functions Forward Decleration************************************/
void TestFlip();
void TestHasLoop();
void TestFindIntersection();

static node_t* CreateNode(int value);
static void FreeList(node_t* head);

/************************************main************************************/
int main(void)
{
	TestFlip();
	TestHasLoop();
	TestFindIntersection();
	
	return (0);
}

/************************************Functions************************************/
void TestFlip()
{
	node_t* n1 = CreateNode(1);
	node_t* n2 = CreateNode(2);
	node_t* n3 = CreateNode(3);
	node_t* flipped = NULL;
	
	n1->next = n2;
	n2->next = n3;
	n3->next = NULL;
	
	printf(TITLE "Test: Flip()\n" RESET);
	printf("n1 = 1 | n2 = 2 | n3 = 3\n");
	
	flipped = Flip(n1);
	
	if (*(int*)flipped->data == 3)
	{
		printf(GREEN "PASSED: " RESET);
		printf("flipped->data = 3\n");
	}
	else
	{
		printf(RED "FAILED: \n" RESET);
		printf("flipped->data != 3\n");		
	}
	
	if (*(int*)flipped->next->data == 2)
	{
		printf(GREEN "PASSED: " RESET);
		printf("flipped->next->data = 2\n");
	}
	else
	{
		printf(RED "FAILED: " RESET);
		printf("flipped->next->data != 2\n");
	}
	
	if (*(int*)flipped->next->next->data == 1)
	{
		printf(GREEN "PASSED: " RESET);
		printf("flipped->next->next->data = 1\n");
	}
	else
	{
		printf(RED "FAILED: " RESET);
		printf("flipped->next->next->data != 1\n");
	}
	
	if (flipped->next->next->next == NULL)
	{
		printf(GREEN "PASSED: " RESET);
		printf("flipped->next->next->next = NULL\n");
	}
	else
	{
		printf(RED "FAILED: " RESET);
		printf("flipped->next->next->next != NULL\n");
	}
	
	FreeList(flipped);
}

void TestHasLoop()
{
	node_t* n1 = CreateNode(1);
	node_t* n2 = CreateNode(2);
	node_t* n3 = CreateNode(3);
		
	n1->next = n2;
	n2->next = n3;
	n3->next = n1;
	
	printf(TITLE "\nTest: HasLoop()\n" RESET);
	
	printf("n1 -> n2 -> n3 -> n1\n");
	if (HasLoop((const node_t*)n1) == 1)
	{
		printf(GREEN "PASSED: " RESET);
		printf("HasLoop(n1) == TRUE\n");
	}
	else
	{
		printf(RED "FAILED: " RESET);
		printf("HasLoop(n1) == FALSE\n");		
	}
	
	n1->next = n2;
	n2->next = n3;
	n3->next = NULL;
	printf("n1 -> n2 -> n3 -> NULL\n");

	if (HasLoop((const node_t*)n1) == 0)
	{
		printf(GREEN "PASSED: " RESET);
		printf("HasLoop(n1) == FALSE\n");
	}
	else
	{
		printf(RED "FAILED: " RESET);
		printf("HasLoop(n1) == TRUE\n");		
	}
	
}

void TestFindIntersection()
{
	node_t* n1 = CreateNode(1);
	node_t* n2 = CreateNode(2);
	node_t* n3 = CreateNode(3);
	node_t* n4 = CreateNode(4);
	node_t* n5 = CreateNode(5);	
	
	n1->next = n2;
	n2->next = n3;
	n3->next = NULL;
	n4->next = n5;
	n5->next = n2;
	
	printf(TITLE "\nTest: FindIntersection()\n" RESET);
	
	printf("n1 -> n2 -> n3 -> NULL | n4 -> n5 -> n2\n");
	if (FindIntersection(n1, n4) == n2)
	{
		printf(GREEN "PASSED: " RESET);
		printf("FindIntersection(n1, n4) == TRUE\n");
	}
	else
	{
		printf(RED "FAILED: " RESET);
		printf("FindIntersection(n1, n4) == FALSE\n");
	}
}

static node_t* CreateNode(int value)
{
	node_t* node = (node_t*)malloc(sizeof(node_t));
	int* data = (int*)malloc(sizeof(int));

	if (node == NULL || data == NULL)
	{
		printf("Memory allocation failed\n");
		return (NULL);
	}

	*data = value;
	node->data = data;
	node->next = NULL;

	return node;	
}

static void FreeList(node_t* head)
{
	node_t* temp = NULL;
	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp->data);
		free(temp);
	}
}
