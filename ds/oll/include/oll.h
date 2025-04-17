#ifndef __ILRD_OLL__
#define __ILRD_OLL__

/************************************includes************************************/
#include <stddef.h> /* size_t */

#include "dll.h"

/************************************typedef************************************/
typedef struct ordered_list oll_t;

struct ordered_list_iter {
	iter_t iter;
	#ifndef NDEBUG /* not not debug == debug */
		const oll_t* list;
	#endif
};

typedef struct ordered_list_iter oll_iter_t; 

/*
	RETURNS: 0 on success, 1 on failure
*/
typedef int (*olist_action_t)(void* data, void* param);

/*
	DESCRIPTION: compare between 2 arguments with a reference to external param
	RETURNS: negative value when data1 is smaller (or earlier) than data2 0 when they are equal, or positive when data 1 is bigger (or later) than data2.
	ARGUMENTS:
		@param1 - const void* data1
		@param2 - const void* data2
		@param3 - void* param - can be used to reference an external data source.
	NOTES:
		data1 is the item from the list to compare against. (during insert it will be iterated through all items until correct location was found)
		data2 is the new item inserted.
*/
typedef int (*olist_comparer_t)(const void* data1, const void* data2, void* param);

/*
	RETURNS TRUE/FALSE
*/
typedef int (*olist_is_match_t)(const void* data1, const void* data2);

/************************************Functions************************************/
/*
	DESCRIPTION: Creates a new sorted/organized list
	RETURNS: oll_t* - a pointer to a new sorted list or NULL on error.
	ARGUMENTS: 
		@param1 - compare_func_t compare_func - the list will be sorted according to this function.
	LIMITATIONS: (none)
	NOTES: (none)
	COMPLEXITY: 
		Time: O(1)
*/
oll_t* OListCreate(olist_comparer_t comparer, void* param);

/*
	DESCRIPTION: Destroys the list and freeing everything inside of it
	RETURNS: (nothing)
	ARGUMENTS:
		@arg1 - oll_t* list - our list
	LIMITATIONS: (none)
	NOTES: 
	COMPLEXITY: 
		Time: O(n)
*/
void OListDestroy(oll_t* list);

/*
	DESCRIPTION: Adds a new node to the list 
	RETURNS: iterator to the node we added. on failure return end iterator.
	ARGUMENTS:
		@arg1 - oll_t* list - the sorted list
		@arg2 - void* data, the data we add to the list
	LIMITATIONS: (none)
	NOTES: 
	COMPLEXITY: 
		Time: O(n)
*/
oll_iter_t OListInsert(oll_t* list, void* data);

/*
	DESCRIPTION: Remove the iterator iter
	RETURNS: iter_t, The iterator that comes after the iterator we removed.
	ARGUMENTS:
		@arg1 - iter_t iter - the iterator we try to remove
	LIMITATIONS: Undefined when trying to remove end iterator.
	NOTES: 
	COMPLEXITY: 
		Time: O(1)
*/
oll_iter_t OListRemove(oll_iter_t iter);

/*
	DESCRIPTION: Merging src into dest. dest will be sorted, src will be emptied.
	RETURNS: (nothing)
	ARGUMENTS:
		@arg1 - oll_t* dest - the list to expand with items from src
		@arg2 - oll_t* src - the list that is consumed by dest.
	LIMITATIONS: (none)
	NOTES: src will be emptied (its size will be 0) - can be reused or destrotyed with destroy.
	COMPLEXITY: 
		Time: O(n+m) n - length of dest, m - length of src
*/
void OListMerge(oll_t* dest, oll_t* src);

/*
	DESCRIPTION: Calculating the size of the list
	RETURNS: size_t - the size of the list
	ARGUMENTS:
		@arg1 - const  oll_t* list
	LIMITATIONS: (none)
	NOTES: 
	COMPLEXITY: 
		Time: O(n)
*/
size_t OListSize(const oll_t* list);

/*
	DESCRIPTION: Checks if the list is empty.
	RETURNS: 1 if True and 0 if False
	ARGUMENTS:
		@arg1 - const oll_t* list
	LIMITATIONS: (None)
	NOTES: 
	COMPLEXITY: 
		Time: O(1)
*/
int OListIsEmpty(const oll_t* list);

/*
	DESCRIPTION: Returns an iterator to the first node in the list.
	RETURNS: iter_t, iterator of the first value in the list, or end iterator if the list is empty.
	ARGUMENTS:
		@arg1 - const oll_t* list
	LIMITATIONS: (none)
	NOTES: 
	COMPLEXITY: 
		Time: O(1)
*/
oll_iter_t OListBegin(const oll_t* list);

/*
	DESCRIPTION: Returns end iterator.
	RETURNS: iter_t, iterator of the end of the list
	ARGUMENTS:
		@arg1 - const oll_t* list
	LIMITATIONS: (none)
	NOTES: 
	COMPLEXITY: 
		Time: O(1)
*/
oll_iter_t OListEnd(const oll_t* list);

/*
	DESCRIPTION: Returns an iterator to the next node of the given iter
	RETURNS: iter_t - next of the iter
	ARGUMENTS: 
		@arg1 - iter_t iter
	LIMITATIONS: undefined behavior when iter is end iterator.
	NOTES: 
	COMPLEXITY: 
		Time: O(1)
*/
oll_iter_t OListNext(oll_iter_t iter);

/*
	DESCRIPTION: returns previous of the iter
	RETURNS: iter_t - previous of the iter
	ARGUMENTS: 
		@arg1 - iter_t iter
	LIMITATIONS: undefined behavior when iter is the first node (iter begin).
	NOTES: 
	COMPLEXITY: 
		Time: O(1)
*/
oll_iter_t OListPrev(oll_iter_t iter);

/*
	DESCRIPTION: Performs action func on all iters in range from excluding to ([from, to))
	RETURNS: 0 if the action was performed on all items successfully or 1 if it failed on one item.
	ARGUMENTS:
		@arg1 - iter_t from
		@arg2 - iter_t to
		@arg3 - action_t action
		@arg4 - void* param
	LIMITATIONS:
		- undefined behavior when action function changes the data in a way the disrupts the order of the items.
		- undefined behavior when iterators are from 2 different lists.
	NOTES: the function exits right after the first item that action_t function failed on.
	COMPLEXITY: 
		Time: O(n)
*/
void OListForEach(oll_iter_t from ,oll_iter_t to, olist_action_t action, void* param);

/*
	DESCRIPTION: Checking between from and to ([from-to)) to see if we can find data. If we find it, return its iterator. or iterator to on failure.
	RETURNS: the iterator of the data we found, to if not found
	ARGUMENTS:
		@arg1 - oll_t* list, used to access the compare function
		@arg2 - iter_t from, Starting position 
		@arg3 - iter_t to, End position (excluded)
		@arg4 - void* data_compare_param, the data we compare.
	LIMITATIONS: undefined behavior when iterators are from 2 different lists.
	NOTES: 
	COMPLEXITY: 
		Time: O(n)
*/
oll_iter_t OListFind(const oll_t* list, oll_iter_t from, oll_iter_t to, const void* data_to_find);

/*
	DESCRIPTION: Checking between from and to ([from-to)) to see if we can find data. If we find it, return its iterator. or iterator to on failure.
	RETURNS: the iterator of the data we found, to if not found
	ARGUMENTS:
		@arg1 - iter_t from, Starting position 
		@arg2 - iter_t to, End position (excluded)
		@arg3 - is_match_t is_match, comparison function for the data we check and the current iterator's data
		@arg4 - void* data_to_find, the data we compare against.
	LIMITATIONS: undefined behavior when iterators are from 2 different lists.
	NOTES: 
	COMPLEXITY: 
		Time: O(n)
*/
oll_iter_t OListFindIf(oll_iter_t from, oll_iter_t to, olist_is_match_t is_match, const void* data_to_find);

/*
	DESCRIPTION: retreives data stored in element at a giver iterator position
	RETURNS: void* type
	ARGUMENTS:
		@arg1 - iter_t iter1
	LIMITATIONS: undefined when end iter.
	NOTES: 
	COMPLEXITY: 
		Time: O(1)
*/
void* OListGetData(oll_iter_t iter);

/*
	DESCRIPTION: check is 2 iters are the same
	RETURNS: int
	ARGUMENTS:
		@arg1 - iter_t iter1
		@arg2 - iter_T iter2
	LIMITATIONS: undefined behavior when iterators are from 2 different lists.
	NOTES: 
	COMPLEXITY: 
		Time: O(1)
*/
int OListIsSameIter(oll_iter_t iter1, oll_iter_t iter2);

/*
	DESCRIPTION: Removes from the start of the list
	RETURNS: (nothing)
	ARGUMENTS:
		@arg1 - oll_t* list
	LIMITATIONS: undefined when list is empty
	COMPLEXITY: 
		Time: O(1)
*/
void OListPopFront(oll_t* list);

/*
	DESCRIPTION: Removes from the end of the list
	RETURNS: (nothing)
	ARGUMENTS:
		@arg1 - oll_t* list
	LIMITATIONS: undefined when list is empty
	COMPLEXITY: 
		Time: O(1)
*/
void OListPopBack(oll_t* list);

#endif
