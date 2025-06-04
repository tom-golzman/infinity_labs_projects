/**
	Written By: Tom Golzman
	Date: 04/06/2025
	Reviewed By: 
**/

/************************************includes************************************/
#include <assert.h> /* assert() */

#include "set.h"

/************************************define************************************/
#define TRUE 	(1)
#define FALSE 	(0)
#define SUCCESS (0)
#define FAIL 	(1)

/************************************typedef************************************/
/* struct set */
	/* capacity */
	/* dll_t* table */
	/* size */
		
/************************************Functions************************************/
set_t* SetCreate(size_t capacity, set_is_same_key_t is_same_key, const void* is_same_param, hash_func_t hash_func, const void* hash_func_param)
{
	/* allocate memory for the set */
	/* initialize array elements with an empty dll */
	/* initialize the functions (is_same_key & hash_func) and the params */
}

void SetDestroy(set_t*)
{
	/* destroy all the keys in the set */
	/* free the memory of the set */
}

void* SetRemove(set_t*, const void* key)
{
	/* calculate the hash value of the key */
	/* find the hash value to remove in the hash table */
	/* remove the key from the hash table and free its memory */
}

int SetInsert(set_t*, const void* key)
{
	/* calculate the hash value of the key */
	/* push back the hash value in the place and increase the size */
}

size_t SetSize(const set_t*)
{
	/* returns the size field from the struct */
}

int SetIsEmpty(const set_t*)
{
	/* check if the size field in the struct is zero or not */
}

void* SetFind(const set_t*, const void* data)
{
	/* calculate the hash value of the key */
	/* go to the hash value in the hash table and get the data */
}

int SetForEach(set_t*, set_action_func_t, void* param)
{
	/* iterate all the hash table and run the action_func on the data */
	/* on first fail return the status */
}













