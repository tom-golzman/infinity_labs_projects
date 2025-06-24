/**
	Written By: Tom Golzman
	Date: 24/06/2025
	Reviewed By: 
**/

/************************************includes************************************/
#include <assert.h>		/* assert() */
#include <stddef.h>		/* size_t */
#include <stdlib.h>		/* malloc, free */
#include <pthread.h>	/* pthread_create, pthread_join */

#include "utils.h"		/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */
#include "100k_threads.h"

/************************************define************************************/

/********************************Global Variables*********************************/
static int* g_arr = NULL;

/********************************Private Functions********************************/
static void* WriteIndex(void* arg);

/************************************Functions************************************/
void InitArray(int* arr, size_t size)
{
	pthread_t* threads = NULL;
	size_t i = 0;
	
	assert(NULL != arr);
	
	g_arr = arr;
	
	threads = (pthread_t*)calloc(1, size * sizeof(pthread_t));
	ExitIfBad(NULL != threads, FAIL, "malloc() FAILED!\n");
	
	for (i = 0; i < size; ++i)
	{
		pthread_create(&threads[i], NULL, WriteIndex, (void*)i);
	}
	
	free(threads);
	threads = NULL;
}

static void* WriteIndex(void* arg)
{
	size_t idx = (size_t)arg;
	
	g_arr[idx] = idx;
	
	return NULL;
}
