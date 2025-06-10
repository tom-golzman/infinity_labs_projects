/************************************includes************************************/
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc, free */

#include "test_utils.h" /* colors, titles, status, boolean */
#include "priority_queue.h"

/************************************function declarations************************************/
void TestPQCreate();
void TestPQDestroyNULL();
void TestPQEnqueuePeek();
void TestPQDequeue();
void TestPQIsEmpty();
void TestPQSize();
void TestPQClear();
void TestPQErase();
void TestPQPeekEmpty();
void TestPQClearEmpty();
void TestPQEnqueuePeekMultipleSamePriority();

int CompareInts(const void* a, const void* b, void* param);
int MatchInts(const void* a, const void* b);

/************************************main************************************/
int main(void)
{
	TestPQCreate();
	TestPQDestroyNULL();
	TestPQEnqueuePeek();
	TestPQEnqueuePeekMultipleSamePriority();
	TestPQDequeue();
	TestPQIsEmpty();
	TestPQSize();
	TestPQClear();
	TestPQClearEmpty();
	TestPQErase();
	TestPQPeekEmpty();

    printf("\n");

    return (0);
}

/************************************Tests************************************/
void TestPQCreate()
{
    priority_queue_t* pq = PQCreate(CompareInts, NULL);
    
    printf(BOLD_TITLE "\nTest: PQCreate()\n" RESET);
    
    if (NULL != pq && PQIsEmpty(pq))
    {
        printf(GREEN "TEST 1 PASSED\n" RESET);
    }
    else
    {
        printf(RED "TEST 1 FAILED: " RESET "Queue not initialized properly. IsEmpty()=%d\n", PQIsEmpty(pq));
    }
    PQDestroy(pq);
}

void TestPQDestroyNULL()
{
    printf(BOLD_TITLE "\nTest: PQDestroy(NULL)\n" RESET);
    PQDestroy(NULL);
    printf(GREEN "TEST 1 PASSED\n" RESET);
}

void TestPQEnqueuePeek()
{
    int a = 10, b = 20, c = 5, d = 30, e = 40, f = 15;
    priority_queue_t* pq = PQCreate(CompareInts, NULL);
    printf(BOLD_TITLE "\nTest: PQEnqueue() + PQPeek()\n" RESET);

	PQEnqueue(pq, &a);
PQPrintTree(pq);
	PQEnqueue(pq, &b);
PQPrintTree(pq);
	PQEnqueue(pq, &c);
PQPrintTree(pq);
	PQEnqueue(pq, &d);
PQPrintTree(pq);
	PQEnqueue(pq, &e);
PQPrintTree(pq);
	PQEnqueue(pq, &f);
PQPrintTree(pq);	

    if (*(int*)PQPeek(pq) == 5)
    {
        printf(GREEN "TEST 1 PASSED\n" RESET);
    }
    else
    {
        printf(RED "TEST 1 FAILED: " RESET "Top element = %d\n", *(int*)PQPeek(pq));
    }
    
    PQDestroy(pq);
}

void TestPQEnqueuePeekMultipleSamePriority()
{
    int a = 10, b = 10;
    priority_queue_t* pq = PQCreate(CompareInts, NULL);
    printf(BOLD_TITLE "\nTest: Enqueue with same priorities\n" RESET);

    PQEnqueue(pq, &a);
    PQEnqueue(pq, &b);

    if (*(int*)PQPeek(pq) == 10 && PQSize(pq) == 2)
    {
        printf(GREEN "TEST 1 PASSED\n" RESET);
    }
    else
    {
        printf(RED "TEST 1 FAILED: " RESET "Same priority values not handled correctly\n");
    }
    PQDestroy(pq);
}

void TestPQDequeue()
{
	int a = 30, b = 20, c = 5;
	priority_queue_t* pq = PQCreate(CompareInts, NULL);

	printf(BOLD_TITLE "\nTest: PQDequeue()\n" RESET);

	PQEnqueue(pq, &a);
	PQEnqueue(pq, &b);
	PQEnqueue(pq, &c);
	PQPrintTree(pq);
	PQDequeue(pq);
	
	PQPrintTree(pq);

	if (*(int*)PQPeek(pq) == 20)
	{
		printf(GREEN "TEST 1 PASSED\n" RESET);
	}
	else
	{
		printf(RED "TEST 1 FAILED: " RESET "Top after dequeue = %d\n", *(int*)PQPeek(pq));
	}
	
	PQDestroy(pq);
}

void TestPQIsEmpty()
{
    int a = 1;
    priority_queue_t* pq = PQCreate(CompareInts, NULL);
    printf(BOLD_TITLE "\nTest: PQIsEmpty()\n" RESET);

    if (PQIsEmpty(pq))
    {
        printf(GREEN "TEST 1 PASSED\n" RESET);
    }

    PQEnqueue(pq, &a);
    if (!PQIsEmpty(pq))
    {
        printf(GREEN "TEST 2 PASSED\n" RESET);
    }
    else
    {
        printf(RED "TEST 2 FAILED: " RESET "Should not be empty\n");
    }
    PQDestroy(pq);
}

void TestPQSize()
{
    int a = 1, b = 2;
    priority_queue_t* pq = PQCreate(CompareInts, NULL);
    printf(BOLD_TITLE "\nTest: PQSize()\n" RESET);

    PQEnqueue(pq, &a);
    PQEnqueue(pq, &b);

    if (PQSize(pq) == 2)
    {
        printf(GREEN "TEST 1 PASSED\n" RESET);
    }
    else
    {
        printf(RED "TEST 1 FAILED: " RESET "Wrong size\n");
    }
    PQDestroy(pq);
}

void TestPQClear()
{
    int a = 1, b = 2;
    priority_queue_t* pq = PQCreate(CompareInts, NULL);
    printf(BOLD_TITLE "\nTest: PQClear()\n" RESET);

    PQEnqueue(pq, &a);
    PQEnqueue(pq, &b);
    PQClear(pq);

    if (PQIsEmpty(pq))
    {
        printf(GREEN "TEST 1 PASSED\n" RESET);
    }
    else
    {
        printf(RED "TEST 1 FAILED: " RESET "Queue not cleared\n");
    }
    
    PQDestroy(pq);
}

void TestPQClearEmpty()
{
    priority_queue_t* pq = PQCreate(CompareInts, NULL);
    
    printf(BOLD_TITLE "\nTest: PQClear() on empty queue\n" RESET);
    
    PQClear(pq);
    
    printf(GREEN "TEST 1 PASSED\n" RESET);
    
    PQDestroy(pq);
}

void TestPQErase()
{
    int a = 1, b = 20, c = 300, x = 20000, y = 99;
    void* result = NULL;
    priority_queue_t* pq = PQCreate(CompareInts, NULL);
    
    printf(BOLD_TITLE "\nTest: PQErase()\n" RESET);

    PQEnqueue(pq, &a);
    PQEnqueue(pq, &b);
    PQEnqueue(pq, &c);
    PQEnqueue(pq, &x);
   
    result = PQErase(pq, MatchInts, &b);
   
    if (*(int*)result == 20 && PQSize(pq) == 3)
    {
        printf(GREEN "TEST 1 PASSED\n" RESET);
    }
    else
    {
        printf(RED "TEST 1 FAILED: " RESET "Erase failed. result=%d\n", *(int*)result);
    }

    result = PQErase(pq, MatchInts, &y);
    if (result == NULL)
    {
        printf(GREEN "TEST 2 PASSED\n" RESET);
    }
    else
    {
        printf(RED "TEST 2 FAILED: " RESET "Unexpected erase\n");
    }
    PQDestroy(pq);
}

void TestPQPeekEmpty()
{
    priority_queue_t* pq = PQCreate(CompareInts, NULL);
    printf(BOLD_TITLE "\nTest: PQPeek() on empty queue\n" RESET);

    if (PQPeek(pq) == NULL)
    {
        printf(GREEN "TEST 1 PASSED\n" RESET);
    }
    else
    {
        printf(RED "TEST 1 FAILED: " RESET "Peek not NULL on empty\n");
    }
    PQDestroy(pq);
}

/************************************Helpers************************************/
int CompareInts(const void* a, const void* b, void* param)
{
    (void)param;
    
    return (*(int*)a - *(int*)b);
}

int MatchInts(const void* a, const void* b)
{
    return (*(int*)a == *(int*)b);
}

