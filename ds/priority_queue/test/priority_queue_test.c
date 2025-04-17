/************************************includes************************************/
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc, free */

#include "priority_queue.h"

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define TITLE "\033[1;4m"

#define SUCCESS 0
#define FAIL 1

/************************************function declarations************************************/
void TestPQCreate();
void TestPQEnqueuePeek();
void TestPQDequeue();
void TestPQIsEmpty();
void TestPQSize();
void TestPQClear();
void TestPQErase();

int CompareInts(const void* a, const void* b, void* param);
int MatchInts(const void* a, const void* b);

/************************************main************************************/
int main(void)
{
    TestPQCreate();
    TestPQEnqueuePeek();
    TestPQDequeue();
    TestPQIsEmpty();
    TestPQSize();
    TestPQClear();
    TestPQErase();

    printf("\n");
    return 0;
}

/************************************Tests************************************/
void TestPQCreate()
{
    priority_queue_t* pq = PQCreate(CompareInts, NULL);
    
    printf(TITLE "\nTest: PQCreate()\n" RESET);
    
    if (pq && PQIsEmpty(pq))
    {
        printf(GREEN "PASSED: " RESET "Queue created and empty\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "Queue not initialized properly\n");
    }
    
    PQDestroy(pq);
}

void TestPQEnqueuePeek()
{
	int top = 0;
    int a = 10, b = 20, c = 5;
    priority_queue_t* pq = PQCreate(CompareInts, NULL);

    printf(TITLE "\nTest: PQEnqueue() + PQPeek()\n" RESET);

    PQEnqueue(pq, &a);
    PQEnqueue(pq, &b);
    PQEnqueue(pq, &c);

    top = *(int*)PQPeek(pq);
    if (top == 5)
    {
        printf(GREEN "PASSED: " RESET "Peek returns highest priority (smallest value)\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "Peek incorrect, got %d\n", top);
	}
	
    PQDestroy(pq);
}

void TestPQDequeue()
{
	int top = 0;
    int a = 30, b = 10, c = 20;
    priority_queue_t* pq = PQCreate(CompareInts, NULL);

    printf(TITLE "\nTest: PQDequeue()\n" RESET);

    PQEnqueue(pq, &a);
    PQEnqueue(pq, &b);
    PQEnqueue(pq, &c);

    PQDequeue(pq);

    top = *(int*)PQPeek(pq);
    if (top == 20)
    {
        printf(GREEN "PASSED: " RESET "Dequeue removed top element\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "Top element after dequeue incorrect: %d\n", top);
	}

    PQDestroy(pq);
}

void TestPQIsEmpty()
{
    int a = 1;
    priority_queue_t* pq = PQCreate(CompareInts, NULL);

    printf(TITLE "\nTest: PQIsEmpty()\n" RESET);

    if (PQIsEmpty(pq))
    {
        printf(GREEN "PASSED: " RESET "Queue initially empty\n");
	}
	
    PQEnqueue(pq, &a);

    if (!PQIsEmpty(pq))
    {
        printf(GREEN "PASSED: " RESET "Queue not empty after insert\n");
   	}
    else
    {
        printf(RED "FAILED: " RESET "Queue falsely reported as empty\n");
	}
	
    PQDestroy(pq);
}

void TestPQSize()
{
    int a = 1, b = 2;
    priority_queue_t* pq = PQCreate(CompareInts, NULL);

    printf(TITLE "\nTest: PQSize()\n" RESET);

    PQEnqueue(pq, &a);
    PQEnqueue(pq, &b);

    if (PQSize(pq) == 2)
    {
        printf(GREEN "PASSED: " RESET "Size is correct\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "Size incorrect\n");
	}
	
    PQDestroy(pq);
}

void TestPQClear()
{
    int a = 1, b = 2, c = 3;
    priority_queue_t* pq = PQCreate(CompareInts, NULL);

    printf(TITLE "\nTest: PQClear()\n" RESET);

    PQEnqueue(pq, &a);
    PQEnqueue(pq, &b);
    PQEnqueue(pq, &c);

    PQClear(pq);

    if (PQIsEmpty(pq))
    {
        printf(GREEN "PASSED: " RESET "Clear emptied the queue\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "Clear failed\n");
	}
	
    PQDestroy(pq);
}

void TestPQErase()
{
    int a = 1, b = 2, c = 3;
    priority_queue_t* pq = PQCreate(CompareInts, NULL);

    printf(TITLE "\nTest: PQErase()\n" RESET);

    PQEnqueue(pq, &a);
    PQEnqueue(pq, &b);
    PQEnqueue(pq, &c);

    PQErase(pq, MatchInts, &b);

    if (PQSize(pq) == 2 && *(int*)PQPeek(pq) == 1)
    {
        printf(GREEN "PASSED: " RESET "Erase removed the correct item\n");
    }
    else
    {
        printf(RED "FAILED: " RESET "Erase failed\n");
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

