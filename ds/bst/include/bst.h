#ifndef __ILRD_BST_H__
#define __ILRD_BST_H__

/************************************includes************************************/
#include <stddef.h> /* size_t */

/************************************typedef************************************/
typedef struct bst bst_t;
typedef struct bst_node* bst_iter_t;

/**
	@DESCRIPTION: Comparisong between 2 datas using param
	@PARAMETERS:
			- void* current_data - data to compare against
			- void* data_to_compare - data to compare 
			- void* param - parameters used for the comparison
	@RETURNS: int - bigger/smaller/equal
	@LIMITATIONS: (none)
	@Time Complexity: O(?)
	@Space Complexity: O(?)
*/
typedef int (*bst_comparer_t)(const void* current_data, const void* data_to_compare, void* param);

/**
	@DESCRIPTION: Running in order over each value and using the foreach function on it.
	@PARAMETERS:
			- void* item1 - first item
			- void* param - parameters used for the comparison
	@RETURNS: int - status (SUCCESS / FAIL)
	@Limitations: (none)
	@Time Complexity: O(?)
	@Space Complexity:O(?)
*/
typedef int (*bst_action_func_t)(void* item, void* param);

/************************************Functions************************************/
/**
	@DESCRIPTION: Creates a pointer to a binary search tree (bst_t*)
	@PARAMETERS:
			- compare function
			- void* param - parameters for the comparison function
	@RETURNS: bst_t* a pointer to a binary search tree, NULL on failure
	@Limitations: compare can't be NULL
	@Time Complexity: O(1)
	@Space Complexity: O(1)
*/
bst_t* BSTCreate(bst_comparer_t comparer, void* param);

/**
	@Description: Destroys a bst (bst_t*)
	@PARAMETERS: bst_t* a pointer to a binary search tree
	@RETURNS: (none)
	@Limitations: bst can't be NULL
	@Time Complexity: O(n)
	@Space Complexity: O(1)
**/
void BSTDestroy(bst_t* bst);

/**
	@Description: Count the amount of nodes in the list
	@PARAMETERS: bst_t* a pointer to a binary search tree
	@RETURNS: size_t - the amount of nodes in the tree
	@Limitations: bst can't be NULL
	@Time Complexity: O(n)
	@Space Complexity: O(1)
**/
size_t BSTSize(const bst_t* bst);

/**
	@Description: Insert a value to the list
	@PARAMETERS:
			- bst_t* a pointer to a binary search tree
			- void* data - the data the user wants to insert to the binary search tree
	@RETURNS: bst_iter_t - iterator for a node, fail - end iterator
	@Limitations: bst can't be NULL, impossible to add data that's already in the tree
	@Time Complexity: O(log n)
	@Space Complexity: O(1)
**/
bst_iter_t BSTInsert(bst_t* bst, void* data);

/**
	@Description: Removes an iterator from the binary search tree
	@PARAMETERS: bst_iter_t iter - an iterator to the element we want to remove
	@RETURNS: void* - data
	@Limitations: iter needs to be valid.
	@Time Complexity: O(log n)
	@Space Complexity: O(1)
**/
void* BSTRemove(bst_iter_t iter);

/**
	@Description: Searches for an element in [from, to) range in the binary search tree
	@PARAMETERS:
			- bst_t* a pointer to a binary search tree
			- bst_iter_t from - iterator referring to start of range
			- bst_iter_t to - iterator referring to end of range (excluded)
			- void* data - the data we search the binary search tree for
	@RETURNS: bst_iter_t - iterator to a node that contains data. If couldn't find - return to iterator
	@Limitations: bst can't be NULL
	@Time Complexity: O(n)
	@Space Complexity: O(1)
**/
bst_iter_t BSTFind(const bst_t* bst, bst_iter_t from, bst_iter_t to, void* data);

/**
	@Description: Returns data in an iterator
	@PARAMETERS: bst_iter_t iter - an iterator
	@RETURNS: void* - data
	@Limitations: iter needs to be valid.
	@Time Complexity: O(1)
	@Space Complexity: O(1)
**/
void* BSTGetData(bst_iter_t iter);

/**
	@Description: Returns the iterator of the smallest value
	@PARAMETERS: bst_t* bst - a pointer to the binary tree
	@RETURNS: bst_iter_t, iterator to smallest value
	@Limitations: bst can't be NULL
	@Time Complexity: O(log n)
	@Space Complexity: O(1)
**/
bst_iter_t BSTBegin(const bst_t* bst);

/**
	@Description: Returns an iterator to the final node of the tree (invalid iterator)
	@PARAMETERS: bst_t* bst - a pointer to the binary tree
	@RETURNS: bst_iter_t, iterator to the end iterator (invalid iterator)
	@Limitations: bst can't be NULL
	@Time Complexity: O(1)
	@Space Complexity: O(1)
**/
bst_iter_t BSTEnd(const bst_t* bst);

/**
	@Description: Gets the next iterator
	@PARAMETERS: bst_iter_t iter - iterator we want to get its next
	@RETURNS: bst_iter_t, an iterator to the next value
	@Limitations: undefined behavior if iter is equal to BSTEnd
	@Time Complexity: O(log n)
	@Space Complexity: O(1)
**/
bst_iter_t BSTNext(bst_iter_t iter);

/**
	@Description: Gets the prev iterator
	@PARAMETERS: bst_iter_t iter - iterator we want to get its prev
	@RETURNS: bst_iter_t, an iterator to the prev value
	@Limitations: undefined behavior if iter is equal to BSTBegin
	@Time Complexity: O(log n)
	@Space Complexity: O(1)
**/
bst_iter_t BSTPrev(bst_iter_t iter);

/**
	@Description: operates action function on each node in [from, to) range in the binary search tree
	@PARAMETERS:
			- bst_iter_t from - iterator referring to start of range
			- bst_iter_t to - iterator referring to end of range (excluded)
			- bst_action_func_t action - pointer to function to operate on each node in the range
			- void* param - parameter for action function
	@RETURNS: returns 0 on success, 1 on fail
	@Limitations: undefined behavior if foreach_func is NULL
	@Time Complexity: O(n * m)
	@Space Complexity: O(1)
**/
int BSTForEach(bst_iter_t from, bst_iter_t to, bst_action_func_t action, void* param);

/**
	@DESCRIPTION: compares iter1 and iter2
	@PARAMETERS: bst_iter_t iter1, bst_iter_t iter2
	@RETURNS: returns 1 if iter1 and iter2 are the same iter, otherwise 0
	@LIMITATIONS: (none)
	@Time Complexity: O(1)
	@Space Complexity: O(1)
**/
int BSTIsSameIter(bst_iter_t iter1, bst_iter_t iter2);

/**
	@DESCRIPTION: determines if the binary search tree is empty.
	@PARAMS: bst - a pointer to a binary search tree
	@RETURNS: int - 1 = TRUE , 0 = FALSE
	@LIMITATIONS: bst can't be NULL
	@Time Complexity: O(1)
	@Space Complexity: O(1)
**/
int BSTIsEmpty(const bst_t* bst);

#endif /*__ILRD_BST_H__*/
