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
typedef struct args {
	size_t start;
	size_t end;
	size_t num;
} args_t;

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

size_t WorkLoad(int* arr, size_t size)
{
	pthread_t* threads = NULL;
	size_t i = 0;
	size_t num = 1234567890;
	size_t range = num / size;
	size_t start = 1;
	size_t end = range;
	size_t sum = 0;
	size_t status = 0;
	void* result = 0;
	args_t* args = NULL;
	
	assert(NULL != arr);
	
	g_arr = arr;
	
	threads = (pthread_t*)calloc(1, size * sizeof(pthread_t));
	ExitIfBad(NULL != threads, FAIL, "threads calloc() FAILED!\n");
	
	for (i = 0; i <= size; ++i)
	{
		args = (args_t*)calloc(1, sizeof(args_t));
		ExitIfBad(NULL != args, FAIL, "args calloc() FAILED!\n");
		
		args->start = start;
		args->end = end;
		args->num = num;
		
		if (i == size - 1)
		{
			args->end = num;
		}
		
		status = pthread_create(&threads[i], NULL, SumOfDivisors, args);
		ExitIfBad(0 == status, FAIL, "pthread_create() FAILED!\n");

		start += range;
		end += range;
	}

	for (i = 0; i < size; ++i)
	{
		pthread_join(threads[i], &result);
		sum += (size_t)result;
	}
	
	free(threads);
	threads = NULL;
	
	return sum;
}

static void* SumOfDivisors(void* args)
{
	size_t start = ((args_t*)args)->start;
	size_t end = ((args_t*)args)->end;
	size_t num = ((args_t*)args)->num;
	size_t sum = 0;
	size_t i = 0;
	size_t* result = NULL;
	
	for (i = start; i <= end; ++i)
	{
		if (0 == num % i)
		{
			sum += i;
		}
	}
	
	free(args);
	args = NULL;
	
	return (void*)sum;
}

size_t SimpleWorkLoad()
{
	size_t sum = 0;
	size_t i = 0;
	
	for (i = 1; i <= 1234567890; ++i)
	{
		if (0 == 1234567890 % i)
		{
			sum += i;
		}
	}
	
	return sum;
}












