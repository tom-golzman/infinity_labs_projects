#ifndef __ILRD_AVL_H__
#define __ILRD_AVL_H__

/************************************includes************************************/
#include <stddef.h> /* size_t */

/************************************typedef************************************/
typedef struct avl avl_t;

/**
	@DESCRIPTION:Compare between 2 data items to determine order between them
	@PARAMETERS:
	   @param current_data - item in the tree to perform comparison on
	   @param data_to_compare - external data to compare against
	   @param param - optional parameter used for the comparison
	@RETURNS: 0 if they are equal, (>0) positive if current_data > data_to_compare, and (<0) negative otherwise.
	@LIMITATIONS: (none)
**/
typedef int (*avl_comparer_t)(const void* current_data, const void* data_to_compare, void* param);

/**
	@DESCRIPTION: Running in order over each value and using the foreach function on it.
	@PARAMS:
	   @param item  - item in the tree to perform task on.
	   @param param - optional parameter to use in the action function.
	@RETURNS: 0 on SUCCESS or 1 on FAILURE.
	@LIMITATIONS: (none)
**/
typedef int (*avl_action_func_t)(void* item, void* param);

/************************************Functions************************************/
/**
	@DESCRIPTION: Creates an AVL tree object.
	@PARAMS:
	   @param comparer - a compare function (mandatory).
	   @param param - optional patameter for the comparison function.
	@RETURNS: a pointer to a new AVL tree object or NULL on error.
	@LIMITATIONS: undefined behavior when comparer is NULL.
	@COMPLEXITY: O(1)
**/
avl_t* AVLCreate(avl_comparer_t comparer, void* param);

/**
	@DESCRIPTION: Destroys an AVL tree.
	@PARAMS:
	   @param avl - a pointer to an AVL tree created with AVLCreate().
	@RETURNS: (none)
	@LIMITATIONS: undefined behavior when AVL is NULL.
	@COMPLEXITY: O(n)
**/
void AVLDestroy(avl_t* avl);

/**
	@DESCRIPTION: Counts the amount of nodes in the AVL.
	@PARAMS:
	   @param avl - a pointer to a contant AVL tree object.
	@RETURNS: the amount of nodes in the tree.
	@LIMITATIONS: undefined behavior when avl is NULL.
	@COMPLEXITY: O(n).
**/
size_t AVLSize(const avl_t* avl);

/**
	@DESCRIPTION: determines if the AVL tree is empty.
	@PARAMS:
		@param avl - a pointer to a contant AVL tree object.
	@RETURNS: 1 - when list is empty, 0 otherwise.
	@LIMITATIONS: undefined behavior when AVL is NULL.
	@COMPLEXITY: O(1).
*/
int AVLIsEmpty(const avl_t* avl);

/**
	@DESCRIPTION: Insert a key-value pair to the AVL tree by reference.
	@PARAMS:
		@param avl - a pointer to an AVL tree.
		@param key - the key matching the data item to add to the tree.
		@param data - the data item matching the key to add to the tree.
	@RETURNS: on SUCCESS returns 0 or FAILURE returns 1.
	@LIMITATIONS: undefined behavior when avl can't be NULL. and when attempting to add data that's already exists in the tree.
	@COMPLEXITY: O(log n).
*/
int AVLInsert(avl_t* avl, const void* key, const void* data);

/**
	@DESCRIPTION: Removes an item from the AVL tree.
	@PARAMS:
		@param avl - a pointer to an AVL tree.
		@param key - the data to remove.
	@RETURNS: reference to removed data item, or NULL when the item was not found.
	@LIMITATIONS: undefined behavior when AVL is NULL.
	@COMPLEXITY: O(log n).
*/
void* AVLRemove(avl_t* avl, const void* key);

/**
	@DESCRIPTION: Find specific data in the AVL.
	@PARAMS:
	   @param avl - a pointer to constant AVL.
	   @param key - the key to search for inside the AVL tree.
	@RETURNS: reference to the data that match the given key, or NULL when the item was not found.
	@LIMITATIONS: undefined behavior when AVL is NULL.
	@COMPLEXITY: O(log n).
*/
void* AVLFind(const avl_t* avl, const void* key);

/**
	@DESCRIPTION: Returns the height of the AVL tree.
	@PARAMS:
		@param avl - a pointer to the AVL tree we want to know its height
	@RETURNS: value representing height of the tree
	@LIMITATIONS: undefined behavior when AVL is NULL.
	@COMPLEXITY: O(1)
*/
size_t AVLHeight(const avl_t* avl);

/**
	@DESCRIPTION: performs an action on all items in the AVL.
	@PARAMS:
	   @param avl - a pointer to AVL.
	   @param action - a pointer to AVL.
	   @param param - an optional param for the `action` function.
	@RETURNS: returns 0 on success, 1 on error.
	@LIMITATIONS: undefined when any of AVL, action_func, and param is NULL.
	@COMPLEXITY: O(n * m). n = number of items in the tree. m - complexity of the `action` function.
	@NOTE: Stops on the first failure or when done performing action on all items.
*/
int AVLForEach(avl_t* avl, avl_action_func_t action, void* param);

/**
	@DESCRIPTION: Searches for items in the AVL tree by their keys. The references to the items are populated in a given buffer.
	@PARAMS:
	   @param avl - a pointer to constant AVL.
	   @param keys - array of keys to find.
	   @param keys_size - size of keys array.
	   @param out_result - buffer that will be populated with references to items found in the tree.
	@RETURNS: amount of items found from the tree. out_result is populated with returned values.
	@LIMITATIONS: undefined when AVL is NULL, keys or out_result is NULL.
	@COMPLEXITY: O(log(n) * m). n - number of items in the tree. m - keys_size
*/
size_t AVLMultiFind(const avl_t* avl, const void** keys, size_t keys_size, void** out_result);

/**
	@DESCRIPTION: Search and remove items from the AVL by list of keys. The references to the removed items are populated in a given buffer.
	@PARAMS:
	   @param avl - a pointer to AVL.
	   @param keys - array of keys to find and remove.
	   @param keys_size - size of keys array.
	   @param out_result - buffer that will be populated with references to items removed from the tree.
	@RETURNS: amount of items found and removed from the tree. out_result is populated with returned values.
	@LIMITATIONS: undefined when AVL is NULL, keys or out_result is NULL.
	@COMPLEXITY: O(log(n) * m). n = number of items in the tree. m = keys_size.
*/
size_t AVLMultiRemove(avl_t* avl, const void** keys, size_t keys_size, void** out_result);

/************************************Print Functions************************************/
typedef void (*print_func_t)(const void* data);
void PrintTree(avl_t* avl, print_func_t print_func);

#endif /*__ILRD_AVL_H__*/
