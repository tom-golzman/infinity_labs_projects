#ifndef __ILRD_SET_H__
#define __ILRD_SET_H__

/************************************includes************************************/

/************************************typedef************************************/
typedef struct set set_t;

/**
	@RETURN: status. TRUE or FALSE.
**/
typedef int (*set_is_match_t)(const void* current_data, const void* data_to_compare, void* param);

/**
	@RETURN: index in the hash table.
**/
typedef size_t (*hash_func_t)(const void* data);

/**
	@RETURN: status. SUCCESS or FAIL.
**/
typedef int (*set_action_func_t)(void* data, void* param);

/************************************Functions************************************/
set_t* SetCreate(const set_is_match_t is_match, void* param, const hash_func_t hash_func, size_t table_size);

/**
	@NOTES: the set can be NULL.
**/
void SetDestroy(set_t*);

/**
	@RETURN: returns the data that has been removed.
**/
void* SetRemove(set_t*, const void* data, const set_is_match_t is_match, void* param);

/**
	@RETURN: status. allocation can fail.
**/
int SetInsert(set_t*, const void* data, const set_is_match_t is_match, void* param);

/**
	@COMPLEXITY: O(size).
**/
size_t SetSize(const set_t*);

/**
	@RETURN: status. TRUE or FALSE.
**/
int SetIsEmpty(const set_t*);

/**
	@RETURN: the data that was found. NULL if wasn't found
**/
void* SetFind(const set_t*, const void* data);

/**
	@RETURN: status. SUCCESS or FAIL.
**/
int SetForEach(set_t*, set_action_func_t, void* param);

#endif /*__ILRD_SET_H__*/
