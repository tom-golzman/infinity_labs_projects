/**
	Written By: Tom Golzman
	Date: 12/06/2025
	Reviewed By: 
**/

/************************************includes************************************/
#include <assert.h> /* assert() */
#include <stdlib.h> /* calloc(), free() */

#include "utils.h"	/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM() */
#include "bit_trie.h"

/************************************define************************************/
#define NUM_OF_CHILDREN 2

enum { LEFT = 0, RIGHT = 1 };

/************************************typedef************************************/
typedef struct btrie_node {
	struct btrie_node* child[NUM_OF_CHILDREN];
	int is_full;
} btrie_node_t;

struct bit_trie {
	btrie_node_t* root;
	size_t num_bits;
};

/**************************Private Functions Decleration**************************/
static btrie_node_t* CreateNode();
static void DestroyNode(btrie_node_t* node);
static unsigned long GetHelper(btrie_node_t** node, size_t num_bits, size_t level, unsigned long trie_value);
static int IsFull(btrie_node_t* node);
static unsigned long GetBit(unsigned long trie_value, size_t num_bits, size_t level);
static unsigned long ResetValue(unsigned long value, size_t num_bits, size_t level);
static int FreeHelper(btrie_node_t** node, size_t num_bits, size_t level, unsigned long trie_value);

/************************************Functions************************************/
bit_trie_t* BitTrieCreate(size_t num_bits)
{
	bit_trie_t* trie = NULL;
	
	assert(num_bits > 0);
	
	/* allocate memory for the trie struct */
	trie = (bit_trie_t*)calloc(1, sizeof(bit_trie_t));
	if (NULL == trie)
	{
		return NULL;
	}
	
	/* allocate memory for the root node, and initialize trie fields */
	trie->root = CreateNode();
	if (NULL == trie->root)
	{
		BTrieDestroy(trie);
		trie = NULL;
		
		return NULL;
	}
	
	trie->num_bits = num_bits;

	/* return the new strie */
	return trie;
}

void BTrieDestroy(bit_trie_t* trie)
{
	if (NULL == trie)
	{
		return;
	}
	
	DestroyNode(trie->root);
	
	DEBUG_ONLY(
		trie->root = BAD_MEM(btrie_node_t*);
	);
	
	free(trie);
}

unsigned long BTrieGet(bit_trie_t* trie, unsigned long trie_value)
{
	assert(NULL != trie);
	
	/* if the tree is full - return 0 */
	if (IsFull(trie->root))
	{
		return 0;
	}
	
	/* call the recursive function */
	return GetHelper(&(trie->root), trie->num_bits, 0, trie_value);
}

void BTrieClear(bit_trie_t* trie)
{
	if (NULL == trie)
	{
		return;
	}
	
	/* destroy the left and right subtrees of the root */
	DestroyNode(trie->root->child[LEFT]);
	DestroyNode(trie->root->child[RIGHT]);
}

void BTrieFree(bit_trie_t* trie, unsigned long trie_value)
{
	/* call the recursive function */
	FreeHelper(&(trie->root), trie->num_bits, 0, trie_value);
}

/************************************Private Functions************************************/
static btrie_node_t* CreateNode()
{
	btrie_node_t* new_node = (btrie_node_t*)calloc(1, sizeof(btrie_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	new_node->child[LEFT] = NULL;
	new_node->child[RIGHT] = NULL;
	
	return new_node;
}

static void DestroyNode(btrie_node_t* node)
{
	if (NULL == node || node == BAD_MEM(btrie_node_t*))
	{
		return;
	}
	
	DestroyNode(node->child[LEFT]);
	DestroyNode(node->child[RIGHT]);
	
	DEBUG_ONLY(
		node->child[LEFT] = BAD_MEM(btrie_node_t*);
		node->child[RIGHT] = BAD_MEM(btrie_node_t*);
	);
	
	free(node);
}

static unsigned long GetHelper(btrie_node_t** node, size_t num_bits, size_t level, unsigned long trie_value)
{
	unsigned int bit = 0;
	size_t result = 0;
		
	assert(NULL != node);
	
	/* if reached the bottom of the tree */
	if (level == num_bits)
	{
		/* if the leaf is uded */
		if (BAD_MEM(btrie_node_t*) == *node)
		{
			return 0;
		}

		/* if the leaf is unused - set its value */
		*node = BAD_MEM(btrie_node_t*);
			
		return trie_value;
	}
	
	/* if the leaf isn't at tree's level*/
	if (NULL == *node)
	{
		/* create a new node */
		*node = CreateNode();
		if (NULL == *node)
		{
			return 0;
		}
	}

	if (IsFull(*node))
	{
		return 0;
	}
	
	bit = GetBit(trie_value, num_bits ,level);
	
	/* recursive call with the child */
	result = GetHelper(&(*node)->child[bit], num_bits, level + 1, trie_value);
	
	/* if wasnt found an avaliable value on left */
	if (LEFT == bit && 0 == result)
	{
		/* reset the number and check the right side */
		trie_value = ResetValue(trie_value, num_bits, level);
		
		result = GetHelper(&(*node)->child[RIGHT], num_bits, level, trie_value);
	}

	/* mark node full if both children are full */
	(*node)->is_full = (IsFull((*node)->child[LEFT])) && (IsFull((*node)->child[RIGHT]));
	
	/* if all the subtree is full - return 0, else return the result */
	if (IsFull(*node))
	{
		return 0;
	}
	
	return result;
}

static int IsFull(btrie_node_t* node)
{
	return BAD_MEM(btrie_node_t*) == node || NULL != node && node->is_full;
}

static unsigned long GetBit(unsigned long trie_value, size_t num_bits, size_t level)
{
	return (trie_value >> (num_bits - level - 1)) & 1;
}

static unsigned long ResetValue(unsigned long value, size_t num_bits, size_t level)
{
	size_t bit_idx = num_bits - level - 1;
	
	/* reset all the bits after bit_idx */
	value &= ~((1 << (bit_idx + 1)) - 1);
	
	/* switch the bit_idx to 1 */
	value |= (1 << bit_idx);
	
	return value;
}

static int FreeHelper(btrie_node_t** node, size_t num_bits, size_t level, unsigned long trie_value)
{
	size_t bit = 0;
	int should_free = FALSE;
	
	if (NULL == node || NULL == *node || *node == BAD_MEM(btrie_node_t*))
	{
		return FALSE;
	}
	
	/* when reached the end - free the leaf */
	if (level == num_bits)
	{
		if (*node == BAD_MEM(btrie_node_t*))
		{
			*node = NULL;
			
			return TRUE;
		}
		
		return FALSE;
	}
	
	bit = GetBit(trie_value, num_bits, level);
	
	should_free = FreeHelper(&(*node)->child[bit], num_bits, level + 1, trie_value);
	
	if (TRUE == should_free)
	{
		free((*node)->child[bit]);
		(*node)->child[bit] = NULL;
	}
	
	/* update the field is_full */
	(*node)->is_full = IsFull((*node)->child[LEFT]) && IsFull((*node)->child[RIGHT]);

	/* if the node doesnt have children and doesnt represent a full node - free it */
	return (NULL == (*node)->child[LEFT]) && (NULL == (*node)->child[RIGHT]) && (TRUE != (*node)->is_full);
}
