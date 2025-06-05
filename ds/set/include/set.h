#ifndef __ILRD_SET_H__
#define __ILRD_SET_H__

/************************************includes************************************/

/************************************typedef************************************/
typedef struct set set_t;

typedef int (*set_is_same_key_t)(const void* key1, const void* key2, const void* param);

/**
	@RETURN: number in the range 0 to at least the requested size of the set (can be bigger).
**/
typedef size_t (*hash_func_t)(const void* key, const void* param);


/************************************Functions************************************/
set_t* SetCreate(size_t capacity, set_is_same_key_t is_same_key, const void* is_same_param, hash_func_t hash_func, const void* hash_func_param);

/**
	@NOTES: the set can be NULL.
**/
void SetDestroy(set_t*);

/**
	@RETURN: returns the data that has been removed.
	@LIMITATIONS: undefined to remove a key that does not exist.
**/
void* SetRemove(set_t*, const void* key);

/**
	@RETURN: status. allocation can fail.
**/
int SetInsert(set_t*, const void* key);

/**
	@COMPLEXITY: ?
**/
size_t SetSize(const set_t*);

/**
	@RETURN: NULL if wasn't found
**/
void* SetFind(const set_t*, const void* key);

/**
	@RETURN: status
	@NOTES: undefined if it changes the key part of the object
**/
typedef int (*set_action_func_t)(void* data, const void* param);

/**
	@RETURN: last status returned by set_action_func_t.
	@NOTES: stops and returnes if set_action_func_t fails.
**/
int SetForEach(set_t*, set_action_func_t, void* param);

#endif /*__ILRD_SET_H__*/
