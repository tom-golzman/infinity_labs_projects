/**
	Written By: Tom Golzman
	Date: 12/04/2025
	Reviewed By: Hagai Levy
**/

/************************************includes************************************/
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include "dll.h"

/************************************define************************************/
#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define FAIL 1

/************************************typedef************************************/
typedef struct node_t {
	void* data;
	struct node_t* next;
	struct node_t* prev;
} node_t;

struct doubly_list_t {
	node_t* head;
	node_t* tail;
};

/*-------------------------functions forward decleration-------------------------*/
static iter_t NodeToIter(node_t* node);
static node_t* IterToNode(iter_t iter);

/************************************Functions************************************/
dll_t* DListCreate(void)
{
	dll_t* list = NULL;
	node_t* head_dummy = NULL;
	node_t* tail_dummy = NULL;

	list = (dll_t*)malloc(sizeof(dll_t));
	if (NULL == list)
	{
		list = NULL;
		
		return (NULL);
	}

	head_dummy = (node_t*)malloc(sizeof(node_t));
	tail_dummy = (node_t*)malloc(sizeof(node_t));

	if (NULL == head_dummy || NULL == tail_dummy)
	{
		free(head_dummy);
		free(tail_dummy);
		free(list);
		
		list = NULL;
		head_dummy = NULL;
		tail_dummy = NULL;
		
		return (NULL);
	}

	head_dummy->prev = NULL;
	head_dummy->next = tail_dummy;
	head_dummy->data = NULL;

	tail_dummy->prev = head_dummy;
	tail_dummy->next = NULL;
	tail_dummy->data = NULL;

	list->head = head_dummy;
	list->tail = tail_dummy;

	return (list);
}

void DListDestroy(dll_t* list)
{
	node_t* current = NULL;
	node_t* next = NULL;

	if (NULL == list)
	{
		return;
	}

	current = list->head;

	while (NULL != current)
	{
		next = current->next;
		free(current);
		current = next;
	}

	free(list);
	list = NULL;
}

size_t DListSize(const dll_t* list)
{
	size_t count = 0;
	node_t* current = NULL;

	assert(NULL != list);

	current = list->head->next;
	while (current != list->tail)
	{
		++count;
		current = current->next;
	}

	return (count);
}

int DListIsEmpty(const dll_t* list)
{
	assert(NULL != list);
	
	return (list->head->next == list->tail);
}

iter_t DListBegin(const dll_t* list)
{
	assert(NULL != list);
	
	return (NodeToIter(list->head->next));
}

iter_t DListEnd(const dll_t* list)
{
	assert(NULL != list);
	
	return (NodeToIter(list->tail));
}

iter_t DListPrev(iter_t iter)
{
	assert(NULL != IterToNode(iter)->prev);
	
	return (NodeToIter(IterToNode(iter->prev)));
}

iter_t DListNext(iter_t iter)
{
	assert(NULL != IterToNode(iter)->next);
	
	return (NodeToIter(IterToNode(iter->next)));
}

int DListIsSameIter(iter_t iter1, iter_t iter2)
{
	return (IterToNode(iter1) == IterToNode(iter2));
}

void* DListGetData(iter_t iter)
{
	node_t* casted_iter = IterToNode(iter);
	
	return (casted_iter->data);
}

void DListSetData(iter_t iter, void* new_data)
{
	node_t* casted_iter = IterToNode(iter);
	
	casted_iter->data = new_data;
}

iter_t DListInsert(dll_t* list, iter_t iter, void* data)
{
	node_t* new_node = NULL;
	node_t* casted_iter = IterToNode(iter);

	assert(NULL != list);

	new_node = (node_t*)malloc(sizeof(node_t));
	if (NULL == new_node)
	{
		return (DListEnd(list));
	}

	new_node->data = data;
	new_node->next = casted_iter;
	new_node->prev = casted_iter->prev;
	
	casted_iter->prev->next = new_node;
	casted_iter->prev = new_node;

	return (NodeToIter(new_node));
}

iter_t DListRemove(iter_t iter)
{
	node_t* casted_iter = IterToNode(iter);
	node_t* result = NULL;
	
	assert(NULL != casted_iter->next);
	
	result = casted_iter->next;
	casted_iter->prev->next = casted_iter->next;
	casted_iter->next->prev = casted_iter->prev;

	free(casted_iter);
	casted_iter = NULL;
	
	return (NodeToIter(result));
}

int DListPushFront(dll_t* list, void* data)
{
	iter_t result;
	
	assert(NULL != list);

	result = DListInsert(list, DListBegin(list), data);
	
	if (DListIsSameIter(result, DListEnd(list)))
	{
		return (FAIL);
	}
	
	return (SUCCESS);
}

int DListPushBack(dll_t* list, void* data)
{
	iter_t result;
	
	assert(NULL != list);

	result = DListInsert(list, DListEnd(list), data);
	
	if (DListIsSameIter(result, DListEnd(list)))
	{
		return (FAIL);
	}
	
	return (SUCCESS);
}

void DListPopFront(dll_t* list)
{
	assert(NULL != list);
	assert(!DListIsEmpty(list));
	
	DListRemove(DListBegin(list));
}

void DListPopBack(dll_t* list)
{
	assert(NULL != list);
	assert(!DListIsEmpty(list));
	
	DListRemove(DListPrev(DListEnd(list)));
}

int DListForEach(iter_t from ,iter_t to, action_t action_func, const void* param)
{
	node_t* runner = IterToNode(from);
	node_t* end = IterToNode(to);
	
	assert(NULL != action_func);

	while (runner != end)
	{
		if (FAIL == action_func(runner->data, param))
		{
			return (FAIL);
		}
		runner = runner->next;
	}
	
	return (SUCCESS);
}

iter_t DListFind(iter_t from, iter_t to, is_match_t is_match, const void* is_match_param, void* data_to_find)
{
	node_t* runner = IterToNode(from);
	node_t* end = IterToNode(to);
	
	assert(NULL != is_match);

	while (runner != end)
	{
		if (TRUE == is_match(runner->data, data_to_find, is_match_param))
		{
			return (NodeToIter(runner));
		}
		runner = runner->next;
	}

	return (to);
}

void DListMultiFind(iter_t from, iter_t to, is_match_t is_match, const void* is_match_param, void* data, dll_t* dest)
{
	node_t* runner = IterToNode(from);
	node_t* end = IterToNode(to);
	
	assert(NULL != is_match);
	assert(NULL != dest);

	while (runner != end)
	{
		if (TRUE == is_match(runner->data, data, is_match_param))
		{
			DListPushBack(dest, runner->data);
		}
		runner = runner->next;
	}
}

void DListSplice(iter_t dest, iter_t from, iter_t to)
{
	node_t* casted_dest = IterToNode(dest);
	node_t* casted_from = IterToNode(from);
	node_t* casted_to = IterToNode(to);
	node_t* before_from = casted_from->prev;
	node_t* before_dest = casted_dest->prev;
	node_t* last = casted_to->prev;

	assert(casted_from != casted_to);

	before_from->next = casted_to;
	casted_to->prev = before_from;

	casted_from->prev = before_dest;
	before_dest->next = casted_from;
	last->next = casted_dest;
	casted_dest->prev = last;
}

static iter_t NodeToIter(node_t* node)
{
	assert(NULL != node);

	return (iter_t)node;
}

static node_t* IterToNode(iter_t iter)
{
	return (node_t*)iter;
}
