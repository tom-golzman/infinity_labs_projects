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
static void* SumOfDivisors(void* num);

/************************************Functions************************************/
void InitArray(int* arr, int size)
{
	pthread_t* threads = NULL;
	int i = 0;
	int* arg = NULL;
	int status = -1;
	
	assert(NULL != arr);
	
	g_arr = arr;
	
	threads = (pthread_t*)calloc(1, size * sizeof(pthread_t));
	ExitIfBad(NULL != threads, FAIL, "malloc() FAILED!\n");
	
	for (i = 0; i < size; ++i)
	{
		status = -1;
		arg = (int*)malloc(sizeof(int));
		ExitIfBad(NULL != arg, FAIL, "malloc() FAILED!\n");
		*arg = i;
		
		status = pthread_create(&threads[i], NULL, WriteIndex, arg);
		ExitIfBad(0 == status, FAIL, "pthread_create() FAILED!\n");
	}

	for (i = 0; i < size; ++i)
	{
		pthread_join(threads[i], NULL);
	}
	
	free(threads);
	threads = NULL;
}

static void* WriteIndex(void* arg)
{
	int idx = *(int*)arg;
	
	g_arr[idx] = idx;
	
	free(arg);
	
	return NULL;
}

void WorkLoad(int* arr, int size)
{
	pthread_t* threads = NULL;
	int i = 0;
	int* arg = NULL;
	int status = -1;
	
	assert(NULL != arr);
	
	g_arr = arr;
	
	threads = (pthread_t*)calloc(1, size * sizeof(pthread_t));
	ExitIfBad(NULL != threads, FAIL, "malloc() FAILED!\n");
	
	for (i = 0; i < size; ++i)
	{
		status = -1;
		arg = (int*)malloc(sizeof(int));
		ExitIfBad(NULL != arg, FAIL, "malloc() FAILED!\n");
		*arg = i;
		
		status = pthread_create(&threads[i], NULL, SumOfDivisors, arg);
		ExitIfBad(0 == status, FAIL, "pthread_create() FAILED!\n");
	}

	for (i = 0; i < size; ++i)
	{
		pthread_join(threads[i], NULL);
	}
	
	free(threads);
	threads = NULL;
}

static void* SumOfDivisors(void* num)
{
	int i = 0;
	int sum = 0;
	
	for (i = 1; i < *(int*)num; ++i)
	{
		if (0 == *(int*)num % i)
		{
			sum += i;
		}
	}
	
	return NULL;
}














