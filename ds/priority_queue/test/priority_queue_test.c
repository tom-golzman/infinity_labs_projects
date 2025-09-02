#include <stdio.h>
#include <stdlib.h>

#include "test_utils.h"
#include "priority_queue.h"

typedef struct
{
	int key;
	int value;
} pq_data_t;

int PQCmp(const void* data1, const void* data2, void* param)
{
	(void) param;
	return (((pq_data_t*)data1)->key - ((pq_data_t*)data2)->key);
}

int CompareInts(const void* a, const void* b, void* param)
{
	(void)param;
	return (*(int*)a - *(int*)b);
}

int MatchInts(const void* a, const void* b)
{
	return (*(int*)a == *(int*)b);
}

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
	return 0;
}

void TestPQCreate()
{
	priority_queue_t* pq = PQCreate(CompareInts, NULL);
	TITLE("Test: PQCreate()");

	RUN_TEST("PQ created and is empty", (NULL != pq && PQIsEmpty(pq)));

	PQDestroy(pq);
}

void TestPQDestroyNULL()
{
	TITLE("Test: PQDestroy(NULL)");
	PQDestroy(NULL);
	RUN_TEST("No crash on NULL destroy", TRUE);
}

void TestPQEnqueuePeek()
{
	int a = 10, b = 20, c = 5, d = 30, e = 40, f = 15;
	priority_queue_t* pq = PQCreate(CompareInts, NULL);

	TITLE("Test: PQEnqueue() + PQPeek()");
	PQEnqueue(pq, &a);
	PQEnqueue(pq, &b);
	PQEnqueue(pq, &c);
	PQEnqueue(pq, &d);
	PQEnqueue(pq, &e);
	PQEnqueue(pq, &f);

	RUN_TEST("Peek gives min", *(int*)PQPeek(pq) == 5);

	PQDestroy(pq);
}

void TestPQEnqueuePeekMultipleSamePriority()
{
	int a = 10, b = 10;
	priority_queue_t* pq = PQCreate(CompareInts, NULL);

	TITLE("Test: Enqueue with same priorities");
	PQEnqueue(pq, &a);
	PQEnqueue(pq, &b);

	RUN_TEST("Same priority values handled", *(int*)PQPeek(pq) == 10 && PQSize(pq) == 2);

	PQDestroy(pq);
}

void TestPQDequeue()
{
	int a = 30, b = 20, c = 5;
	priority_queue_t* pq = PQCreate(CompareInts, NULL);

	TITLE("Test: PQDequeue()");
	PQEnqueue(pq, &a);
	PQEnqueue(pq, &b);
	PQEnqueue(pq, &c);
	PQDequeue(pq);

	RUN_TEST("Top after dequeue is next min", *(int*)PQPeek(pq) == 20);

	PQDestroy(pq);
}

void TestPQIsEmpty()
{
	int a = 1;
	priority_queue_t* pq = PQCreate(CompareInts, NULL);

	TITLE("Test: PQIsEmpty()");

	RUN_TEST("Empty after creation", PQIsEmpty(pq));

	PQEnqueue(pq, &a);
	RUN_TEST("Not empty after insert", !PQIsEmpty(pq));

	PQDestroy(pq);
}

void TestPQSize()
{
	int a = 1, b = 2;
	priority_queue_t* pq = PQCreate(CompareInts, NULL);

	TITLE("Test: PQSize()");
	PQEnqueue(pq, &a);
	PQEnqueue(pq, &b);

	RUN_TEST("Size == 2 after 2 inserts", PQSize(pq) == 2);

	PQDestroy(pq);
}

void TestPQClear()
{
	int a = 1, b = 2;
	priority_queue_t* pq = PQCreate(CompareInts, NULL);

	TITLE("Test: PQClear()");
	PQEnqueue(pq, &a);
	PQEnqueue(pq, &b);
	PQClear(pq);

	RUN_TEST("Cleared queue is empty", PQIsEmpty(pq));

	PQDestroy(pq);
}

void TestPQClearEmpty()
{
	priority_queue_t* pq = PQCreate(CompareInts, NULL);

	TITLE("Test: PQClear() on empty queue");

	PQClear(pq);

	RUN_TEST("Clearing empty queue does not crash", TRUE);

	PQDestroy(pq);
}

void TestPQErase()
{
	int a = 1, b = 20, c = 300, x = 20000, y = 99;
	void* result = NULL;
	priority_queue_t* pq = PQCreate(CompareInts, NULL);

	TITLE("Test: PQErase()");
	PQEnqueue(pq, &a);
	PQEnqueue(pq, &b);
	PQEnqueue(pq, &c);
	PQEnqueue(pq, &x);

	result = PQErase(pq, MatchInts, &b);
	RUN_TEST("Erased correct element", (*(int*)result == 20 && PQSize(pq) == 3));

	result = PQErase(pq, MatchInts, &y);
	RUN_TEST("No match returns NULL", result == NULL);

	PQDestroy(pq);
}

void TestPQPeekEmpty()
{
	priority_queue_t* pq = PQCreate(CompareInts, NULL);

	TITLE("Test: PQPeek() on empty queue");

	RUN_TEST("Peek returns NULL", PQPeek(pq) == NULL);

	PQDestroy(pq);
}