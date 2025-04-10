/**
	Written By: Tom Golzman
	Date: 09/04/2025
	Reviewed By: Yuval Almog
**/

/************************************includes************************************/
#include <assert.h> /* assert */
#include "queue.h" /* queue */
#include "sll_ex.h"

/************************************define************************************/
#define TRUE (1)
#define FALSE (0)

/************************************Functions************************************/
node_t* Flip(node_t* head)
{
	node_t* next = NULL;
	node_t* prev = NULL;
	node_t* curr = NULL;
	
	assert (NULL != head);
	
	curr = head;
	
	while (NULL != curr)
	{
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	
	return (prev);
}

int HasLoop(const node_t* head)
{
	queue_t* queue = QueueCreate();
	node_t* curr = NULL;
	node_t* temp_next = NULL;
	int status = FALSE;
	
	assert(NULL != head);

	if (NULL == queue)
	{
		return (-1);
	}
	curr = (node_t*)head;
	temp_next = (node_t*)head->next;
	
	while(NULL != temp_next && FALSE == status)
	{
		if (temp_next == head)
		{
			status = TRUE;
		}
		else
		{
			curr = temp_next;
			QueueEnqueue(queue, curr->next);
			temp_next = curr->next;
			curr->next = (node_t*)head;
		}
	}
	
	curr = (node_t*)head->next;
	
	while (NULL != curr && FALSE == QueueIsEmpty(queue))
	{
		curr->next = QueuePeek(queue);
		QueueDequeue(queue);
		curr = curr->next;
	}
	
	return (status);
}

node_t* FindIntersection(node_t* head_1, node_t* head_2)
{
	queue_t* queue1 = QueueCreate();
	queue_t* queue2 = QueueCreate();	
	node_t* curr_1 = NULL;
	node_t* curr_2 = NULL;
	node_t* temp_next = NULL;
	node_t* result = NULL;
		
	assert(NULL != head_1);
	assert(NULL != head_2);
	
	if (NULL == queue1)
	{
		return (NULL);
	}
	
	if (NULL == queue2)
	{
		return (NULL);
	}
	
	temp_next = head_1->next;
	
	while (NULL != temp_next)
	{
		curr_1 = temp_next;
		QueueEnqueue(queue1, curr_1->next);
		temp_next = curr_1->next;
		curr_1->next = head_1;
	}
	
	temp_next = head_2;
	while (NULL != temp_next)
	{		
		curr_2 = temp_next;
		if (curr_2->next == head_1)
		{
			result = curr_2;
			break;
		}

		QueueEnqueue(queue2, curr_2->next);
		temp_next = curr_2->next;
		curr_2->next = NULL;
	}
	
	curr_1 = head_1;
	while (FALSE == QueueIsEmpty(queue1))
	{
		curr_1->next = (node_t*)QueuePeek(queue1);
		QueueDequeue(queue1);
		curr_1 = curr_1->next;
	}
	
	curr_2 = head_2;
	while (FALSE == QueueIsEmpty(queue2))
	{
		curr_2->next = (node_t*)QueuePeek(queue2);
		QueueDequeue(queue2);
		curr_2 = curr_2->next;
	}
	
	QueueDestroy(queue1);
	queue1 = NULL;
	
	QueueDestroy(queue2);
	queue2 = NULL;
		
	return (result);
}
