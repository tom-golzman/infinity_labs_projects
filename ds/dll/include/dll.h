#ifndef __ILRD_DLL__
#define __ILRD_DLL__

/************************************includes************************************/
#include <stddef.h> /* size_t */

/************************************typedef************************************/
typedef struct node_t* iter_t;
typedef struct doubly_list_t dll_t;

typedef int (*action_t)(void* data, const void* param);
typedef int (*is_match_t)(const void* data1, const void* data2, const void*);

/************************************Functions************************************/
/*
# DESCRIPTION: Creates new double linked list
# RETURNS: dll_t* - pointer to a double linked list, or NULL on error
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
*/
dll_t* DListCreate(void);

/*
# DESCRIPTION: destroys double linked list
# RETURNS: (nothing)
# ARGUMENTS:
    @arg1 - dll_t* list
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
*/
void DListDestroy(dll_t* list);

/*
# DESCRIPTION: returns size of the list
# RETURNS: size_t - size of the list
# ARGUMENTS:
    @arg1 - const dll_t* list
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(n)
*/
size_t DListSize(const dll_t* list);

/*
# DESCRIPTION: returns whether the list is empty or not
# RETURNS: int - 1 on TRUE and 0 on FALSE
# ARGUMENTS:
    @arg1 - const dll_t* list
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
*/
int DListIsEmpty(const dll_t* list);

/*
# DESCRIPTION: returns iter of start of the list
# RETURNS: iter_t - start of the list
# ARGUMENTS:
    @arg1 - const dll_t* list
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
*/
iter_t DListBegin(const dll_t* list);

/*
# DESCRIPTION: returns iter of end of the list
# RETURNS: iter_t - end of the list
# ARGUMENTS:
    @arg1 - const dll_t* list
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
*/
iter_t DListEnd(const dll_t* list);

/*
# DESCRIPTION: returns previous of the iter
# RETURNS: iter_t - previous of the iter
# ARGUMENTS:
    @arg1 - const dll_t* list
# LIMITATIONS: Prev to the begin of the list - undefined.
# COMPLEXITY: 
    Time: O(1)
*/
iter_t DListPrev(iter_t iter);

/*
# DESCRIPTION: returns next of the iter
# RETURNS: iter_t - next of the iter
# ARGUMENTS:
    @arg1 - const dll_t* list
# LIMITATIONS: Next to the end of the list - undefined.
# COMPLEXITY: 
    Time: O(1)
*/
iter_t DListNext(iter_t iter);

/*
# DESCRIPTION: compares two iters
# RETURNS: 1 on TRUE and 0 on FALSE
# ARGUMENTS:
    @arg1 - iter_t iter1
    @arg2 - iter_t iter2
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
*/
int DListIsSameIter(iter_t iter1, iter_t iter2);

/*
# DESCRIPTION: returns data in iter
# RETURNS: void* - data in iter
# ARGUMENTS:
    @arg1 - iter_t iter
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
*/
void* DListGetData(iter_t iter);

/*
# DESCRIPTION: Sets data in iter to new_data
# RETURNS: (nothing)
# ARGUMENTS:
    @arg1 - iter_t iter
    @arg2 - void* new_data
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
*/
void DListSetData(iter_t iter, void* new_data);

/*
# DESCRIPTION: Inserts to the list before iter. If allocation failed, return ListEnd.
# RETURNS: iter_t - newly added iter in list
# ARGUMENTS:
    @arg1 - dll_t* list
    @arg2 - iter_t iter
    @arg3 - void* data
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
*/
iter_t DListInsert(dll_t* list, iter_t iter, void* data);

/*
# DESCRIPTION: removes iter from the list
# RETURNS: Returns iterator to the next element.
# ARGUMENTS:
    @arg1 - dll_t list (to update the head when removing first)
    @arg2 - iter_t iter
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
*/
iter_t DListRemove(iter_t iter);

/*
# DESCRIPTION: Inserts to the start of the list
# RETURNS: 0 on SUCCESS and 1 on FAILURE
# ARGUMENTS:
    @arg1 - dll_t* list
    @arg2 - void* data
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
*/
int DListPushFront(dll_t* list, void* data);

/*
# DESCRIPTION: Inserts at the end of the list
# RETURNS: 0 on SUCCESS and 1 on FAILURE
# ARGUMENTS:
    @arg1 - dll_t* list
    @arg2 - void* data
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
*/
int DListPushBack(dll_t* list, void* data);

/*
# DESCRIPTION: Removes from the start of the list
# RETURNS: (nothing)
# ARGUMENTS:
    @arg1 - dll_t* list
# LIMITATIONS: Pop from an empty list - undefined.
# COMPLEXITY: 
    Time: O(1)
*/
void DListPopFront(dll_t* list);

/*
# DESCRIPTION: Removes from the end of the list
# RETURNS: (nothing)
# ARGUMENTS:
    @arg1 - dll_t* list
# LIMITATIONS: Pop from an empty list - undefined.
# COMPLEXITY: 
    Time: O(1)
*/
void DListPopBack(dll_t* list);

/*
# DESCRIPTION: Performs action func on all iters in range excluding to
# RETURNS: 0 on SUCCESS and 1 on FAILURE
# ARGUMENTS:
    @arg1 - iter_t from
    @arg2 - iter_t to
    @arg3 - action_t action_func
    @arg4 - void* param
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(n)
*/
int DListForEach(iter_t from ,iter_t to, action_t action_func, const void* param);

/*
# DESCRIPTION: Searches the given iter range for iter containing data_to_find and stores in found_iter
# RETURNS: iter_t to the found iter or end when it wasn't found
# ARGUMENTS:
    @arg1 - iter_t from
    @arg2 - iter_t to
    @arg3 - is_match_t is_match
    @arg4 - void* param
    @arg5 - iter_t* found_iter
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(n)
*/
iter_t DListFind(iter_t from, iter_t to, is_match_t is_match, const void* is_match_param, void* data_to_find);

/*
# DESCRIPTION: Searches the given iter range for iter containing data_to_find and stores in result_list
# RETURNS: (nothing)
# ARGUMENTS:
    @arg1 - iter_t from
    @arg2 - iter_t to
    @arg3 - is_match_t is_match
    @arg4 - void* data_to_find
    @arg5 - dll_t* dest
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(n)
*/
void DListMultiFind(iter_t from, iter_t to, is_match_t is_match, const void* is_match_param, void* data, dll_t* dest);

/*
# DESCRIPTION: Merges two lists at splice_location iter by putting the dest before splice_location. Runs from from_source to to_source not including to_source so user can't splice dummy
# RETURNS: (nothing)
# ARGUMENTS:
    @arg1 - iter_t dest
    @arg2 - iter_t from
    @arg3 - iter_t to
# LIMITATIONS: no limitation
# COMPLEXITY: 
    Time: O(1)
*/
void DListSplice(iter_t dest, iter_t from, iter_t to);


#endif
