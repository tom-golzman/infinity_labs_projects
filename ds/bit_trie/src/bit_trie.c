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
enum { LEFT = 0, RIGHT = 1 , NUM_OF_CHILDREN = 2};

/************************************typedef************************************/
typedef struct btrie_node {
	struct btrie_node* children[NUM_OF_CHILDREN];
	int is_full;
} btrie_node_t;

struct bit_trie {
	btrie_node_t root;
	size_t num_bits;
};

/**************************Private Functions Decleration**************************/
static btrie_node_t* CreateNode();
static void DestroySubTree(btrie_node_t* node);
static unsigned long GetHelper(btrie_node_t** node, size_t num_bits, size_t level, unsigned long value);
static int IsFull(btrie_node_t* node);
static unsigned long GetBit(unsigned long value, size_t num_bits, size_t level);
static unsigned long ResetValue(unsigned long value, size_t num_bits, size_t level);
static void FreeHelper(btrie_node_t** node, size_t num_bits, size_t level, unsigned long value);

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
	
	/* initialize trie fields */
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
	
	BTrieClear(trie);
	/*
	DEBUG_ONLY(
		trie->root = BAD_MEM(btrie_node_t);
		trie->num_bits = 0;
		trie->is_full = FALSE;
	);*/
	
	free(trie);
}

unsigned long BTrieGet(bit_trie_t* trie, unsigned long value)
{
	btrie_node_t* root = NULL;
	
	assert(NULL != trie);
	
	root = &(trie->root);
	
	/* call the recursive function */
	return GetHelper(&root, trie->num_bits, 0, value);
}

void BTrieClear(bit_trie_t* trie)
{
	assert(NULL != trie);
	
	/* destroy the left and right subtrees of the root */
	DestroySubTree(trie->root.children[LEFT]);
	DestroySubTree(trie->root.children[RIGHT]);
}

void BTrieFree(bit_trie_t* trie, unsigned long value)
{
	btrie_node_t* root = NULL;
	
	assert(NULL != trie);
	assert(0 != value);
	
	root = &(trie->root);
	
	/* call the recursive function */
	FreeHelper(&(root), trie->num_bits, 0, value);
}

/************************************Private Functions************************************/
static btrie_node_t* CreateNode()
{
	btrie_node_t* new_node = (btrie_node_t*)calloc(1, sizeof(btrie_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	new_node->children[LEFT] = NULL;
	new_node->children[RIGHT] = NULL;
	
	return new_node;
}

static void DestroySubTree(btrie_node_t* node)
{
	if (NULL == node || node == BAD_MEM(btrie_node_t*))
	{
		return;
	}
	
	DestroySubTree(node->children[LEFT]);
	node->children[LEFT] = NULL;
	DestroySubTree(node->children[RIGHT]);
	node->children[RIGHT] = NULL;
	
	DEBUG_ONLY(
		node->children[LEFT] = BAD_MEM(btrie_node_t*);
		node->children[RIGHT] = BAD_MEM(btrie_node_t*);
	);
	
	free(node);
}

static unsigned long GetHelper(btrie_node_t** node, size_t num_bits, size_t level, unsigned long value)
{
	unsigned int bit = 0;
	size_t result = 0;
		
	assert(NULL != node);
	
	/* if the subtree is full */
	if (IsFull(*node))
	{
		return 0;
	}
	
	/* if reached the bottom of the tree (leaf) */
	if (level == num_bits)
	{
		/* if the leaf is unused - set its value */
		*node = BAD_MEM(btrie_node_t*);
			
		return value;
	}
	
	/* if the leaf isn't at tree's level - create a node*/
	if (NULL == *node)
	{
		/* create a new node */
		*node = CreateNode();
		if (NULL == *node)
		{
			return 0;
		}
	}

	/* try the requested value */
	bit = GetBit(value, num_bits ,level);
	
	/* recursive call with the requested value */
	result = GetHelper(&(*node)->children[bit], num_bits, level + 1, value);
	
	/* if requested was LEFT and isn't available - try RIGHT */
	if (LEFT == bit && 0 == result)
	{
		/* reset the number and check the right side */
		value = ResetValue(value, num_bits, level);
		
		result = GetHelper(&(*node)->children[RIGHT], num_bits, level + 1, value);
	}

	/* mark node full if both childrenren are full */
	(*node)->is_full = (IsFull((*node)->children[LEFT])) && (IsFull((*node)->children[RIGHT]));
	
	return result;
}

static int IsFull(btrie_node_t* node)
{
	return BAD_MEM(btrie_node_t*) == node || (NULL != node && node->is_full);
}

static unsigned long GetBit(unsigned long value, size_t num_bits, size_t level)
{
	return (value >> (num_bits - level - 1)) & 1UL;
}

static unsigned long ResetValue(unsigned long value, size_t num_bits, size_t level)
{
	size_t bit_idx = num_bits - level - 1;
	
	/* reset all the bits after bit_idx */
	value &= (~0UL) << bit_idx;
	
	/* switch the bit_idx to 1 */
	value |= (1UL << bit_idx);
	
	return value;
}

static void FreeHelper(btrie_node_t** node, size_t num_bits, size_t level, unsigned long value)
{
	size_t bit = 0;
	
	assert(NULL != node); 
	assert(NULL != *node);
	
	/* when reached the end - free the leaf */
	if (level == num_bits)
	{
		*node = NULL;
	
		return;	
	}
	
	bit = GetBit(value, num_bits, level);

	/* call the recursive function */	
	FreeHelper(&(*node)->children[bit], num_bits, level + 1, value);
	
	/* update the field is_full */
	(*node)->is_full = IsFull((*node)->children[LEFT]) && IsFull((*node)->children[RIGHT]);
}
