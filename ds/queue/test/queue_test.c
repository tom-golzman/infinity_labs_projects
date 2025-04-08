/************************************includes************************************/
#include <stdio.h> /* printf */
#include "queue.h"

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define TITLE "\033[1;4m"

/************************************Functions Forward Decleration************************************/
static int IsMatch(const void* data1, const void* data2);

/************************************main************************************/
int main(void)
{
	queue_t* queue = QueueCreate();
	int val1 = 10, val2 = 20, val3 = 30;
	int* peeked_val = NULL;
	size_t size = 0;
	int* found = NULL;
	
	printf(TITLE "Enqueue first element:\n" RESET);
	printf("Status: %d\n", QueueEnqueue(queue, &val1));
	peeked_val = QueuePeek(queue);
	printf("Peeked val: %d\n", *peeked_val);
	size = QueueSize(queue);
	printf("Queue size: %lu\n", size);
	
	printf(TITLE "\nEnqueue second element:\n" RESET);
	printf("Status: %d\n", QueueEnqueue(queue, &val2));
	peeked_val = QueuePeek(queue);
	printf("Peeked val: %d\n", *peeked_val);
	size = QueueSize(queue);
	printf("Queue size: %lu\n", size);
	
	printf(TITLE "\nEnqueue third element:\n" RESET);
	printf("Status: %d\n", QueueEnqueue(queue, &val3));
	peeked_val = QueuePeek(queue);
	printf("Peeked val: %d\n", *peeked_val);
	size = QueueSize(queue);
	printf("Queue size: %lu\n", size);
	
	printf(TITLE "\nDequeue:\n" RESET);
	QueueDequeue(queue);
	peeked_val = QueuePeek(queue);	
	printf("Peeked val after dequeue: %d\n", *peeked_val);
	size = QueueSize(queue);
	printf("Queue size after dequeue: %lu\n", size);
	
	printf(TITLE "\nFind val \"20\":\n" RESET);	
	found = QueueFind(queue, &val2, IsMatch);
	printf("Found value: %d\n", *(int*)found);	
	
	printf(TITLE "\nCurrent queue:\n" RESET);	
	peeked_val = QueuePeek(queue);
	printf("%d ", *peeked_val);
	QueueDequeue(queue);
	peeked_val = QueuePeek(queue);
	printf("%d \n", *peeked_val);
	QueueDequeue(queue);
	
	QueueDestroy(queue);
	
	return (0);
}

/************************************Functions************************************/
static int IsMatch(const void* data1, const void* data2)
{
	return (*(int*)data1 == *(int*)data2);
}
