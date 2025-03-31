/**
	Written By: Tom Golzman
	Date: 26/02/2025
	Reviewed By: Sami 
**/

/*----------------------includes----------------------*/
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

/*----------------------define----------------------*/
#define TRUE 1
#define FALSE 0

/*---------------------typedef---------------------*/
typedef struct node {
	struct node* next;
	void* data;
} node_t;

typedef struct list{
	node_t* head;
	node_t* tail;
} list_t;

typedef node_t* iter_t;

typedef int (*action_t)(void* data, void* param);
typedef int (*is_match_t)(const void* data1, const void* data2);

/*-------------------------functions forward decleration-------------------------*/
static iter_t NodeToIter(node_t* node);
static node_t* IterToNode(iter_t iter);
static int Increament(void* data, void* param);
void ListForEach(iter_t from ,iter_t to, action_t action_func, void* param);

/*-------------------------functions-------------------------*/
list_t* ListCreate(void)
{
	list_t* list = NULL;
	node_t* dummy = NULL;
	
	list = (list_t*)malloc(sizeof(list_t));
	if (NULL == list)
	{
		return (NULL);
	}
	
	dummy = (node_t*)malloc(sizeof(node_t));
	if (NULL == dummy)
	{
		free (list);
		list = NULL;
		
		return (NULL);
	}
	
	dummy->next = NULL;
	dummy->data = list;
	list->head = dummy;
	list->tail = dummy;
	
	return (list);
}

iter_t ListInsert(list_t* list, iter_t iter, void* data)
{
	node_t* new_node = NULL;
	node_t* casted_iter = IterToNode(iter);

	assert(NULL != list);
	
	new_node = (node_t*)malloc(sizeof(node_t));
	if (NULL == new_node)
	{
		return (NodeToIter(list->tail));
	}

	new_node->next = casted_iter->next;
	new_node->data = casted_iter->data;
	casted_iter->data = data;
	
	if (NULL == casted_iter->next)
	{
		list->tail = new_node;
	}
	
	casted_iter->next = new_node;
	
	return (iter);
}

iter_t ListRemove(iter_t iter)
{
	node_t* temp = iter->next;
	node_t* casted_iter = IterToNode(iter);
		
	if (NULL == casted_iter->next->next)
	{
		((list_t*)casted_iter->next->data)->tail = casted_iter;
	}
	
	casted_iter->data = casted_iter->next->data;
	casted_iter->next = casted_iter->next->next;
	
	free(temp);
	temp = NULL;
	
	return (NodeToIter(casted_iter));
}

int ListIsEmpty(const list_t* list)
{
	assert(NULL != list);

	if (list->head == list->tail)
	{
		return (TRUE);
	}
	
	return (FALSE);
}

iter_t ListNext(iter_t iter)
{
	node_t* casted_iter = IterToNode(iter);
	
	return (NodeToIter(casted_iter->next));	
}

iter_t ListBegin(const list_t* list)
{
	assert(NULL != list);

	return (NodeToIter(list->head));
}

iter_t ListEnd(const list_t* list)
{
	assert(NULL != list);
	
	return (NodeToIter(list->tail));
}

void ListForEach(iter_t from ,iter_t to, action_t action_func, void* param)
{
	node_t* casted_from = IterToNode(from);
	node_t* casted_to = IterToNode(to);	
	node_t* runner = NULL;
	
	assert(NULL != action_func);
	
	for (runner = casted_from; runner != casted_to; runner = runner->next)
	{
		if (FALSE == action_func(runner->data, param))
		{
			return ;
		}
	}
}

size_t ListSize(const list_t* list)
{
	size_t counter = 0;
	
	assert(NULL != list);
	
	ListForEach(ListBegin(list), ListEnd(list), Increament, &counter);

	return (counter);
}

void* ListGetData(iter_t iter)
{
	node_t* casted_iter = IterToNode(iter);
	
	return (casted_iter->data);
}

void ListSetData(iter_t iter, void* data)
{
	node_t* casted_iter = IterToNode(iter);
	
	casted_iter->data = data;
}

int IterIsEqual(iter_t iter1, iter_t iter2)
{
	node_t* casted_iter1 = IterToNode(iter1);
	node_t* casted_iter2 = IterToNode(iter2);
		
	if (casted_iter1 == casted_iter2)
	{
		return (TRUE);
	}
	
	return (FALSE);
}

void ListDestroy(list_t* list)
{
    node_t* current = NULL;
    node_t* next = NULL;

    assert(NULL != list);

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

iter_t ListFind(list_t* list, iter_t from, iter_t to, is_match_t is_match, void* data_to_find)
{
	node_t* casted_from = IterToNode(from);
	node_t* casted_to = IterToNode(to);	
	node_t* runner = IterToNode(from);

	assert(NULL != list);
	assert(NULL != is_match);

	for (runner = casted_from; runner != casted_to; runner = runner->next)
	{
		if (is_match(runner->data, data_to_find))
		{
			return (NodeToIter(runner));
		}
		
	}

	return (to);
}

static int Increament(void* data, void* param)
{
	size_t* counter = (size_t*)param;
	(void)data;
	
	(*counter)++;
	
	return (TRUE);
}

static iter_t NodeToIter(node_t* node)
{
	assert(NULL != node);	
	
	return ((iter_t)node);
}

static node_t* IterToNode(iter_t iter)
{
	return ((node_t*)iter);
}
