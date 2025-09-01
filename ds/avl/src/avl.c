/**
	Written By: Tom Golzman
	Date: 26/05/2025
**/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "avl.h"
#include "utils.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct avl_node {
	void* key;
	void* data;
	size_t height;
	struct avl_node* children[2];
} avl_node_t;

struct avl {
	avl_node_t* root;
	avl_comparer_t comparer;
	void* param;
};

enum {
	LEFT_CHILD = 0,
	RIGHT_CHILD
};

static void DestroyHelper(avl_node_t* node);
static size_t SizeHelper(avl_node_t* node);
static void* FindHelper(avl_node_t* node, const void* key, avl_comparer_t comparer, void* param);
static int ForEachHelper(avl_node_t* node, avl_action_func_t action, void* param);
static avl_node_t* InsertHelper(avl_t* avl, avl_node_t* node, const void* key, const void* param);
static avl_node_t* CreateNode(const void* key, const void* data);
static int NodeHeight(avl_node_t* node);
static avl_node_t* RotateRight(avl_node_t* node);
static avl_node_t* RotateLeft(avl_node_t* node);
static void UpdateHeight(avl_node_t* node);
static avl_node_t* RemoveHelper(avl_t* avl, avl_node_t* node, const void* key, void** removed_data);
static avl_node_t* FindMinNode(avl_node_t* node);
static avl_node_t* Balance(avl_node_t* node);

avl_t* AVLCreate(avl_comparer_t comparer, void* param)
{
	avl_t* avl = NULL;
	
	assert(NULL != comparer);
	
	avl = (avl_t*)malloc(sizeof(*avl));
	if (NULL == avl)
	{
		return (NULL);
	}
	
	avl->root = NULL;
	avl->comparer = comparer;
	avl->param = param;
	
	return (avl);
}

void AVLDestroy(avl_t* avl)
{
	if (NULL == avl)
	{
		return;
	}
		
	if (!AVLIsEmpty(avl))
	{
		DestroyHelper(avl->root);
	}
	
	free(avl);
	avl = NULL;
}

size_t AVLSize(const avl_t* avl)
{
	assert(NULL != avl);

	if (AVLIsEmpty(avl))
	{
		return (0);
	}
	
	return (SizeHelper(avl->root));
}

int AVLIsEmpty(const avl_t* avl)
{
	assert(NULL != avl);
	
	if (NULL == avl->root)
	{
		return (TRUE);
	}
	
	return (FALSE);
}

void* AVLFind(const avl_t* avl, const void* key)
{
	assert(NULL != avl);

	return (FindHelper(avl->root, key, avl->comparer, avl->param));
}

size_t AVLHeight(const avl_t* avl)
{
	assert(NULL != avl);
	
	if (NULL == avl->root)
	{
		return (0);
	}
	
	return (avl->root->height);
}

int AVLForEach(avl_t* avl, avl_action_func_t action, void* param)
{
	assert(NULL != avl);
	assert(NULL != action);
	
	return (ForEachHelper(avl->root, action, param));
}

int AVLInsert(avl_t* avl, const void* key, const void* data)
{
	avl_node_t* new_node = NULL;
	
	assert(NULL != avl);
	
	if (NULL == avl->root)
	{
		new_node = CreateNode(key, data);
		if (NULL == new_node)
		{
			return (FAIL);
		}
		avl->root = new_node;
		
		return (SUCCESS);
	}
	
	avl->root = InsertHelper(avl, avl->root, key, data);
	if (NULL == avl->root)
	{
		return (FAIL);
	}
	
	return (SUCCESS);
}

void* AVLRemove(avl_t* avl, const void* key)
{
	void* removed_data = NULL;
	
	assert(NULL != avl);
	
	avl->root = RemoveHelper(avl, avl->root, key, &removed_data);
	
	return (removed_data);
}

size_t AVLMultiFind(const avl_t* avl, const void** keys, size_t keys_size, void** out_result)
{
	size_t found_flag = FALSE;
	void* find_result = NULL;
	
	assert(NULL != avl);
	assert(NULL != keys);
	assert(NULL != out_result);

	if (keys_size == 0)
	{
		return (0);
	}
	
	find_result = AVLFind(avl, keys[keys_size - 1]);
	if (NULL != find_result)
	{
		found_flag = TRUE;
	}
	
	out_result[keys_size - 1] = find_result;
	
	return (found_flag + AVLMultiFind(avl, keys, keys_size - 1, out_result));
}

size_t AVLMultiRemove(avl_t* avl, const void** keys, size_t keys_size, void** out_result)
{
	size_t removed_flag = FALSE;	
	void* remove_result = NULL;

	assert(NULL != avl);
	assert(NULL != keys);
	assert(NULL != out_result);
	
	if (keys_size == 0)
	{
		return (0);
	}
	
	remove_result = AVLRemove(avl, keys[keys_size - 1]);
	if (NULL != remove_result)
	{
		removed_flag = TRUE;
	}
	
	out_result[keys_size - 1] = remove_result;
	
	return (removed_flag + AVLMultiFind(avl, keys, keys_size - 1, out_result));	
}

static void DestroyHelper(avl_node_t* node)
{
	if (NULL == node)
	{
		return;
	}
	
	DestroyHelper(node->children[LEFT_CHILD]);
	DestroyHelper(node->children[RIGHT_CHILD]);
	
	free(node);
	node = NULL;
}

static size_t SizeHelper(avl_node_t* node)
{
	if (NULL == node)
	{
		return (0);
	}
	
	return (1 + SizeHelper(node->children[LEFT_CHILD]) + SizeHelper(node->children[RIGHT_CHILD]));
}

static void* FindHelper(avl_node_t* node, const void* key, avl_comparer_t comparer, void* param)
{
	void* data = NULL;
	int compare_result = 0;
	
	assert(NULL != comparer); 

	if (NULL == node)
	{
		return (NULL);
	}
	
	compare_result = comparer(node->key, key, param);
	if (0 == compare_result)
	{
		return (node->data);
	}
	
	else if (compare_result > 0)
	{
		data = FindHelper(node->children[LEFT_CHILD], key, comparer, param);
	}
	
	else
	{
		data = FindHelper(node->children[RIGHT_CHILD], key, comparer, param);
	}
	
	return (data);
}

static int ForEachHelper(avl_node_t* node, avl_action_func_t action, void* param)
{
	int status = 0;
	
	assert(NULL != action);
	
	if (NULL == node)
	{
		return (SUCCESS);
	}
	
	status = ForEachHelper(node->children[LEFT_CHILD], action, param);
	if (SUCCESS != status)
	{
		return (status);
	}
	
	status = action(node->data, param);
	if (SUCCESS != status)
	{
		return (status);
	}
	
	return (ForEachHelper(node->children[RIGHT_CHILD], action, param));
}

static avl_node_t* InsertHelper(avl_t* avl, avl_node_t* node, const void* key, const void* data)
{
	avl_node_t* new_node = NULL;
	int compare_result = 0;
	
	if (NULL == node)
	{
		new_node = CreateNode(key, data);	
		return (new_node);
	}
	
	compare_result = avl->comparer(node->key, key, avl->param);
	
	assert(compare_result != 0);
	
	if (compare_result > 0)
	{
		node->children[LEFT_CHILD] = InsertHelper(avl, node->children[LEFT_CHILD], key, data);
	}
	
	else
	{
		node->children[RIGHT_CHILD] = InsertHelper(avl, node->children[RIGHT_CHILD], key, data);
	}
	
	node = Balance(node);
	
	return (node);
}
	
static avl_node_t* Balance(avl_node_t* node)
{
	int balance = 0;
	
	if (NULL == node)
	{
		return (NULL);
	}

	UpdateHeight(node);
	
	balance = NodeHeight(node->children[LEFT_CHILD]) - NodeHeight(node->children[RIGHT_CHILD]);
	
	if (balance > 1)
	{
		if (NULL != node->children[LEFT_CHILD] && NodeHeight(node->children[LEFT_CHILD]->children[RIGHT_CHILD]) > NodeHeight(node->children[LEFT_CHILD]->children[LEFT_CHILD]))
		{
			node->children[LEFT_CHILD] = RotateLeft(node->children[LEFT_CHILD]);
		}
		
		node = RotateRight(node);
	}
	
	else if (balance < -1)
	{
		if (NULL != node->children[RIGHT_CHILD] && NodeHeight(node->children[RIGHT_CHILD]->children[LEFT_CHILD]) > NodeHeight(node->children[RIGHT_CHILD]->children[RIGHT_CHILD]))
		{
			node->children[RIGHT_CHILD] = RotateRight(node->children[RIGHT_CHILD]);
		}
		
		node = RotateLeft(node);
	}
	
	return (node);
}

static avl_node_t* CreateNode(const void* key, const void* data)
{
	avl_node_t* new_node = (avl_node_t*)malloc(sizeof(*new_node));
	if (NULL == new_node)
	{
		return (NULL);
	}

	new_node->key = (void*)key;
	new_node->data = (void*)data;
	new_node->height = 1;
	new_node->children[LEFT_CHILD] = NULL;
	new_node->children[RIGHT_CHILD] = NULL;
	
	return (new_node);
}

static void UpdateHeight(avl_node_t* node)
{
	if (NULL == node)
	{
		return;
	}
	
	node->height = 1 + MAX(NodeHeight(node->children[LEFT_CHILD]), NodeHeight(node->children[RIGHT_CHILD]));			
}

static int NodeHeight(avl_node_t* node)
{
	if (NULL == node)
	{
		return (0);
	}
	
	return (node->height);
}

static avl_node_t* RotateRight(avl_node_t* node)
{
	avl_node_t* left_right_child = node->children[LEFT_CHILD]->children[RIGHT_CHILD];
	avl_node_t* left_child = node->children[LEFT_CHILD];
	
	node->children[LEFT_CHILD] = left_right_child;
	left_child->children[RIGHT_CHILD] = node;
	
	UpdateHeight(node);
	UpdateHeight(left_child);
		
	return (left_child);
}

static avl_node_t* RotateLeft(avl_node_t* node)
{
	avl_node_t* right_left_child = node->children[RIGHT_CHILD]->children[LEFT_CHILD];
	avl_node_t* right_child = node->children[RIGHT_CHILD];
	
	node->children[RIGHT_CHILD] = right_left_child;
	right_child->children[LEFT_CHILD] = node;

	UpdateHeight(node);
	UpdateHeight(right_child);
	
	return (right_child);
}

static avl_node_t* RemoveHelper(avl_t* avl, avl_node_t* node, const void* key, void** removed_data)
{
	avl_node_t* successor = NULL;
	avl_node_t* return_node = NULL;
	int compare_result = 0;
	void* tmp_removed_data = NULL;

	if (NULL == node)
	{
		return (NULL);
	}
	
	compare_result = avl->comparer(node->key, key, avl->param);
	
	if (0 == compare_result)
	{
		*removed_data = node->data;
		/* no children */
		if (node->children[LEFT_CHILD] == NULL && node->children[RIGHT_CHILD] == NULL)
		{
			free(node);
			node = NULL;
			
			return (NULL);
		}
		
		/* only left child */
		if (node->children[LEFT_CHILD] != NULL && node->children[RIGHT_CHILD] == NULL)
		{
			return_node = node->children[LEFT_CHILD];
			
			free(node);
			node = NULL;
			
			return (return_node);
		}
		
		/* only right child */
		if (node->children[LEFT_CHILD] == NULL && node->children[RIGHT_CHILD] != NULL)
		{
			return_node = node->children[RIGHT_CHILD];
			
			free(node);
			node = NULL;
			
			return (return_node);
		}
		
		/* two children */
		if (node->children[LEFT_CHILD] != NULL && node->children[RIGHT_CHILD] != NULL)
		{
			successor = FindMinNode(node->children[RIGHT_CHILD]);
			node->key = successor->key;
			node->data = successor->data;
			
			node->children[RIGHT_CHILD] = RemoveHelper(avl, node->children[RIGHT_CHILD], successor->key, &tmp_removed_data);
			
			return (node);
		}
	}
	
	if (compare_result > 0)
	{
		node->children[LEFT_CHILD] = RemoveHelper(avl, node->children[LEFT_CHILD], key, removed_data);
	}
	
	else
	{
		node->children[RIGHT_CHILD] = RemoveHelper(avl, node->children[RIGHT_CHILD], key, removed_data);
	}
	
	node = Balance(node);
	
	return (node);
}

static avl_node_t* FindMinNode(avl_node_t* node)
{
	if (NULL == node->children[LEFT_CHILD])
	{
		return (node);
	}
	
	return (FindMinNode(node->children[LEFT_CHILD]));
}

/************************************Print Functions************************************/
static void PrintTreeHelper(avl_node_t* node, print_func_t print_func, int depth);

void PrintTree(avl_t* avl, print_func_t print_func)
{
	PrintTreeHelper(avl->root, print_func, 0);
}

static void PrintTreeHelper(avl_node_t* node, print_func_t print_func, int depth)
{
	int i = 0;
	
	if (NULL == node)
	{
		return;
	}

	PrintTreeHelper(node->children[RIGHT_CHILD], print_func, depth + 1);

	for (i = 0; i < depth; ++i)
	{
		printf("    ");
	}

	print_func(node->key);
	printf("\n");

	PrintTreeHelper(node->children[LEFT_CHILD], print_func, depth + 1);
}