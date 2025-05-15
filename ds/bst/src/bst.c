/**
	Written By: Tom Golzman
	Date: 13/05/2025
	Reviewed By: 
**/

/************************************includes************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "bst.h"

/************************************define************************************/
#define TRUE (1)
#define FALSE (0)
#define SUCCESS (0)
#define FAIL (1)

/************************************typedef************************************/
typedef struct bst_node {
    void* data;
    struct bst_node* left;
    struct bst_node* right;
    struct bst_node* parent;
} bst_node_t;

struct bst {
    bst_node_t* root;   /* pointer to dummy whose left child is the real root */
    bst_comparer_t compare;
    void* param;
};

typedef bst_node_t* (*get_child_func_t)(bst_node_t* node);

typedef enum {
	LEFT_DIRECTION = 0,
	RIGHT_DIRECTION
} direction_t;

/************************************Private Functions************************************/
static bst_node_t* CreateNode(void* data, bst_node_t* parent);
static bst_node_t* BSTIterToNode(bst_iter_t iter);
static bst_iter_t BSTNodeToIter(bst_node_t* node);
static bst_node_t* IterateInOrder(bst_node_t* node, direction_t direction);
static bst_node_t* GetRightChild(bst_node_t* node);
static bst_node_t* GetLeftChild(bst_node_t* node);
#ifdef DEBUG
static int IsBegin(bst_node_t* node);
static int IsEnd(bst_node_t* node);
#endif

/************************************Functions************************************/
bst_t* BSTCreate(bst_comparer_t comparer, void* param)
{
	bst_node_t* dummy = NULL;
	bst_t* bst = NULL;
	
	assert(NULL != comparer);
	
	bst = (bst_t*)malloc(sizeof(*bst));
	if (NULL == bst)
	{
		return (NULL);
	}
	
	dummy = CreateNode(NULL, NULL);
	if (NULL == dummy)
	{
		free(bst);
		bst = NULL;
		
		return (NULL);
	}

	bst->root = dummy;
	bst->compare = comparer;
	bst->param = param;
	
	return (bst);
}

void BSTDestroy(bst_t* bst)
{
	bst_node_t* current_node = NULL;
	
	assert(NULL != bst);
	
	current_node = BSTBegin(bst);

	while (!BSTIsEmpty(bst))
	{
		BSTRemove(current_node);
		current_node = BSTIterToNode(BSTBegin(bst));
	}
	
	free(bst->root);
	bst->root = NULL;
	free(bst);
	bst = NULL;
}

size_t BSTSize(const bst_t* bst)
{
	bst_iter_t current_iter = NULL;
	size_t count = 0;
	
	assert(NULL != bst);
	
	current_iter = BSTBegin(bst);

	while (!BSTIsSameIter(BSTEnd(bst), current_iter))
	{
		count++;
		current_iter = BSTNext(BSTNodeToIter(current_iter));
	}
	
	return (count);
}

bst_iter_t BSTInsert(bst_t* bst, void* data)
{
	bst_node_t* new_node = NULL;
	bst_node_t* current_node = NULL;
	bst_node_t* parent_node = NULL;
	int compare_result = 0;
	
	assert(NULL != bst);
		
	current_node = bst->root->left;
	parent_node = bst->root;
	
	while (NULL != current_node)
	{
		compare_result = bst->compare(current_node->data, data, bst->param);
		parent_node = current_node;
	
		assert(0 != compare_result);
		
		if (compare_result > 0)
		{
			current_node = current_node->left;
		}
		else
		{
			current_node = current_node->right;
		}
	}
	
	new_node = CreateNode(data, parent_node);
	if (NULL == new_node)
	{
		return (BSTEnd(bst));	
	}
	
	if ((parent_node == bst->root) || compare_result > 0)
	{
		parent_node->left = new_node;
	}
	
	else
	{
		parent_node->right = new_node;
	}
	
	return (BSTNodeToIter(new_node));
}

void* BSTGetData(bst_iter_t iter)
{
	bst_node_t* current = BSTIterToNode(iter);
	
	assert(!IsEnd(current));

	return (current->data);
}

bst_iter_t BSTBegin(const bst_t* bst)
{
	bst_node_t* runner = NULL;	

	assert(NULL != bst);
	
	runner = BSTEnd(bst);
	
	while (NULL != runner->left)
	{
		runner = runner->left;
	}
	
	return (BSTNodeToIter(runner));
}

bst_iter_t BSTEnd(const bst_t* bst)
{
	assert(NULL != bst);
	
	return (BSTNodeToIter(bst->root));
}

int BSTIsEmpty(const bst_t* bst)
{
	assert(NULL != bst);

	return (NULL == bst->root->left);
}

bst_iter_t BSTNext(bst_iter_t iter)
{
	bst_node_t* current = BSTIterToNode(iter);
	
	assert(!IsEnd(current));
	
	return (BSTNodeToIter(IterateInOrder(current, RIGHT_DIRECTION)));
}

bst_iter_t BSTPrev(bst_iter_t iter)
{
	bst_node_t* current = BSTIterToNode(iter);
	
	assert(!IsBegin(current));
	
	return (BSTNodeToIter(IterateInOrder(current, LEFT_DIRECTION)));
}

int BSTForEach(bst_iter_t from, bst_iter_t to, bst_action_func_t action, void* param)
{
	bst_iter_t runner = from;
	int action_status = SUCCESS;
	
	assert(NULL != action);
	
	while (!BSTIsSameIter(runner, to))
	{
		action_status = action(BSTGetData(runner), param);
		if (FAIL == action_status)
		{
			return (FAIL);
		}
		runner = BSTNext(runner);
	}
	
	return (SUCCESS);
}

int BSTIsSameIter(bst_iter_t iter1, bst_iter_t iter2)
{
	return (BSTIterToNode(iter1) == BSTIterToNode(iter2));	
}

bst_iter_t BSTFind(const bst_t* bst, bst_iter_t from, bst_iter_t to, void* data)
{
	bst_iter_t runner = from;	
	
	assert(NULL != bst);
	
	while (!BSTIsSameIter(runner, to))
	{
		if (0 == bst->compare(BSTGetData(runner), data, bst->param))
		{
			return (runner);
		}
		
		runner = BSTNext(runner);
	}
	
	return (to);
}

void* BSTRemove(bst_iter_t iter)
{
	bst_node_t* node_to_remove = BSTIterToNode(iter);
	bst_node_t* parent = NULL;
	bst_node_t* child = NULL;
	bst_node_t* successor = NULL;
	void* data = NULL;
	
	assert(!IsEnd(node_to_remove));
	
	data = node_to_remove->data;
	
	/* two children */
	if (NULL != GetLeftChild(node_to_remove) && NULL != GetRightChild(node_to_remove))
	{
		successor = BSTNext(node_to_remove);
				
		/* swap data */
		node_to_remove->data = BSTGetData(successor);
		node_to_remove = successor;
	}
	
	parent = node_to_remove->parent;
	
	if (NULL != GetLeftChild(node_to_remove))
	{
		child = GetLeftChild(node_to_remove);
	}
	else
	{
		child = GetRightChild(node_to_remove);
	}
	
	/* one child */
	if (NULL != child)
	{
		if (parent->left == node_to_remove)
        {
            parent->left = child;
        }
        else
        {
            parent->right = child;
        }

        child->parent = parent;
	}
	/* no child */
	else
	{
		if (parent->left == node_to_remove)
        {
            parent->left = NULL;
        }
        else
        {
            parent->right = NULL;
        }
	}
	
	free(node_to_remove);
	node_to_remove = NULL;
	
	return (data);
}

/************************************Private Functions************************************/
static bst_node_t* CreateNode(void* data, bst_node_t* parent)
{
	bst_node_t* new_node = (bst_node_t*)malloc(sizeof(*new_node));
	if (NULL == new_node)
	{
		return (NULL);
	}
	
	new_node->data = data;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->parent = parent;
	
	return (new_node);
}

static bst_node_t* IterateInOrder(bst_node_t* node, direction_t direction)
{
	get_child_func_t get_child = NULL;
	get_child_func_t get_opposite_child = NULL;
	bst_node_t* child = NULL;
	bst_node_t* parent = NULL;
	bst_node_t* current = NULL;	
	bst_node_t* next = NULL;
	
	if (direction == RIGHT_DIRECTION)
	{
		get_child = GetRightChild;
		get_opposite_child = GetLeftChild;
	}
	else
	{
		get_child = GetLeftChild;
		get_opposite_child = GetRightChild;
	}
	
	child = get_child(node);
	
	if (NULL != child)
	{
		next = child;
		
		while (get_opposite_child(next))
		{
			next = get_opposite_child(next);
		}
		
		return (next);
	}
	
	current = node;
	parent = node->parent;
	
	while (NULL != parent && get_child(parent) == current)
	{
		current = parent;
		parent = parent->parent;
	}
	
	return (parent);
}

static bst_node_t* GetLeftChild(bst_node_t* node)
{
	return (node->left);
}

static bst_node_t* GetRightChild(bst_node_t* node)
{
	return (node->right);
}

#ifdef DEBUG
static int IsBegin(bst_node_t* node)
{
	bst_node_t* current = node;

	while (current->parent != NULL)
	{
		current = current->parent;
	}

	if (current->left == NULL)
	{
		return (node == current);
	}

	current = current->left;
	while (current->left != NULL)
	{
		current = current->left;
	}

	return (node == current);
}

static int IsEnd(bst_node_t* node)
{
	return (node->parent == NULL);
}
#endif

static bst_node_t* BSTIterToNode(bst_iter_t iter)
{
	return ((bst_node_t*)iter);
}

static bst_iter_t BSTNodeToIter(bst_node_t* node)
{
	return ((bst_iter_t)node);
}
