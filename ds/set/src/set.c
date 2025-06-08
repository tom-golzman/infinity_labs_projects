/**
	Written By: Tom Golzman
	Date: 04/06/2025
	Reviewed By: 
**/

/************************************includes************************************/
#include <assert.h> /* assert() */
#include <stdlib.h> /* malloc(), free() */

#include "dll.h"	/* dll_t, iter_t */
#include "set.h"

/************************************define************************************/
enum {
	TRUE = 1,
	FALSE = 0,
	SUCCESS = 0,
	FAIL = 1
};

/************************************typedef************************************/
struct set {
	size_t capacity;
	set_is_same_key_t is_same_key;
	const void* is_same_param;
	hash_func_t hash_func;
	const void* hash_func_param;
	dll_t** table;
};

typedef struct {
	iter_t iter;
	int was_found;
} set_find_iter_result_t;

/************************************Private Functions************************************/
static set_find_iter_result_t SetFindKeyInBucket(const set_t* set, const void* key);

/************************************Functions************************************/
set_t* SetCreate(size_t capacity, set_is_same_key_t is_same_key, const void* is_same_param, hash_func_t hash_func, const void* hash_func_param)
{
	set_t* set = NULL;
	dll_t** table = NULL;
	dll_t* list = NULL;
	dll_t** curr = NULL;
	dll_t** end = NULL;
	
	/* assert */
	assert(capacity > 0);
	assert(NULL != is_same_key);
	assert(NULL != hash_func);
	
	/* allocate memory for the set and the table*/
	set = (set_t*)malloc(sizeof(*set));
	if (NULL == set)
	{
		return (NULL);
	}

	table = (dll_t**)calloc(capacity, sizeof(dll_t*));
	if (NULL == table)
	{
		free(set);
		set = NULL;
		
		return (NULL);
	}
	
	/* initialize the functions (is_same_key & hash_func) and the params */
	set->table = table;
	set->capacity = capacity;
	set->is_same_key = is_same_key;
	set->is_same_param = is_same_param;
	set->hash_func = hash_func;
	set->hash_func_param = hash_func_param;
	
	/* initialize array elements with an empty dll */
	curr = set->table;
	end = set->table + capacity;
	
	while (curr < end)
	{
		list = DListCreate();
		if (NULL == list)
		{
			SetDestroy(set);
			set = NULL;
			
			return (NULL);
		}
		
		*curr = list;
		++curr;
	}
	
	return (set);
}

void SetDestroy(set_t* set)
{
	dll_t** curr = NULL;
	dll_t** end = NULL;
	
	if (NULL == set)
	{
		return;
	}
	
	curr = set->table;
	end = set->table + set->capacity;
	
	/* destroy all the lists in the table */
	while (curr < end)
	{
		DListDestroy(*curr);
		*curr = NULL;
		
		++curr;
	}
	
	/* free the memory of the set and the table */
	free(set->table);
	set->table = NULL;
	
	free(set);
	set = NULL;
}

int SetInsert(set_t* set, const void* key)
{
	size_t index = 0;
	int insert_status = 0;
	dll_t* bucket = NULL;
	
	/* assert */
	assert(NULL != set);
	
	/* calculate the hash value of the key */
	index = set->hash_func(key, set->hash_func_param) % set->capacity;

	/* push back the hash value in the place and increase the size */
	bucket = set->table[index];
	
	insert_status = DListPushBack(bucket, (void*)key);
	if (SUCCESS != insert_status)
	{
		return (FAIL);
	}
	
	return (SUCCESS);
}

void* SetRemove(set_t* set, const void* key)
{
	set_find_iter_result_t find_result;
	void* data = NULL;
		
	/* assert */
	assert(NULL != set);

	/* calculate the hash value of the key */	
	/* find the hash value to remove in the hash table */
	find_result = SetFindKeyInBucket(set, key);
	
	assert(SUCCESS == find_result.was_found);
	
	data = DListGetData(find_result.iter);
	
	/* remove the key from the hash table */
	DListRemove(find_result.iter);
	
	return (data);
}

size_t SetSize(const set_t* set)
{
	size_t size = 0;
	dll_t** curr = NULL;
	dll_t** end = NULL;	
	
	/* assert */
	assert(NULL != set);
	
	curr = set->table;
	end = set->table + set->capacity;
	
	/* sum the size of all the linked lists in the table */
	while (curr < end)
	{
		if (NULL != *curr)
		{
			size += DListSize(*curr);
		}
		
		++curr;
	}
	
	return (size);
}

void* SetFind(const set_t* set, const void* key)
{
	set_find_iter_result_t find_result;
		
	/* assert */
	assert(NULL != set);

	/* calculate the hash value of the key */
	/* go to the hash value in the hash table and get the data */
	find_result = SetFindKeyInBucket(set, key);
	
	if (SUCCESS != find_result.was_found)
	{
		return (NULL);
	}
	
	return (DListGetData(find_result.iter));
}

int SetForEach(set_t* set, set_action_func_t action_func , void* param)
{
	int action_func_status = SUCCESS;
	dll_t** curr = NULL;
	dll_t** end = NULL;
	
	/* assert */
	assert(NULL != set);
	assert(NULL != action_func);
	
	curr = set->table;
	end = set->table + set->capacity;
	
	/* iterate all the hash table and run the action_func on the data */
	while (curr != end)
	{
		if (NULL != *curr)
		{
			action_func_status = DListForEach(DListBegin(*curr), DListEnd(*curr), action_func, param);
		}
		
		/* on first fail return the status */
		if (SUCCESS != action_func_status)
		{
			return (action_func_status);
		}

		++curr;
	}
	
	return (action_func_status);
}

/************************************Private Functions************************************/
static set_find_iter_result_t SetFindKeyInBucket(const set_t* set, const void* key)
{
	size_t index = 0;
	dll_t* bucket = NULL;
	set_find_iter_result_t result;
	
	assert(NULL != set);
	
	index = set->hash_func(key, set->hash_func_param) % set->capacity;
	bucket = set->table[index];
	
	result.iter = DListFind(DListBegin(bucket), DListEnd(bucket), set->is_same_key, set->is_same_param, (void*)key);
	result.was_found = SUCCESS;
	
	if (DListIsSameIter(result.iter, DListEnd(set->table[index])))
	{
		result.was_found = FAIL;
	}
	
	return (result);
}
