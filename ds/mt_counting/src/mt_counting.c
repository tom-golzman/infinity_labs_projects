/**
	Written By: Tom Golzman
	Date: 20/07/2025
	Reviewed By: 
**/

/************************************includes************************************/
#include <assert.h> /* assert() */
#include <stdio.h>	/* printf() */
#include <pthread.h>	/* pthread_t, pthread_create(), pthread_join() */
#include <fcntl.h>		/* open(), close(), lseek() O_RDONLY */
#include <sys/mman.h>	/* mmap(), munmap(), PROT_READ, MAP_PRIVATE */

#include "utils.h"	/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */
#include "mt_counting.h"

/************************************define************************************/
enum
{
	CHAR_SIZE = 256,
	MAX_THREADS = 30
};

typedef struct
{
	const char* start;
	const char* end;
	size_t* counting_array;
} thread_data_t;

/************************************static Functions************************************/
static void* ThreadFunc(void* arg);
static void MergeResults(size_t* counting_array, size_t result_array[CHAR_SIZE], int num_threads);
static void PrintResult(size_t result_array[CHAR_SIZE]);

/************************************Functions************************************/
int MTCounting(const char* file_path, int num_threads)
{
	int fd = 1;
	int i = 0;
	int status = -1;
	int file_size = 0;
	size_t section_size = 0;
	const char* file_data = NULL;
	
	pthread_t threads[MAX_THREADS] = {0};
	thread_data_t thread_data[MAX_THREADS] = {0};
	size_t result_array[CHAR_SIZE] = {0};
	size_t counting_array[MAX_THREADS][CHAR_SIZE] = {0};
	size_t* counting_array_ptrs[MAX_THREADS] = {0};
	
	/* assert */
	assert(NULL != file_path);
	assert(num_threads < MAX_THREADS);
	
	/* open the file & handle failure */	
	fd = open(file_path, O_RDONLY);
	RET_IF_BAD(-1 != fd, FAIL, "MTCounting(): open() FAILED!\n");
	
	/* calculate the file size */
	file_size = lseek(fd, 0, SEEK_END);
	RET_IF_BAD(-1 != file_size, FAIL, "MTCounting(): lseek() FAILED!\n");
	
	/* assign to a local variable the size of a section (file size / num of threads) */
	section_size = file_size / num_threads;
	
	/* call mmap() */
	file_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	RET_IF_BAD((void*)-1 != file_data, FAIL, "MTCounting(): mmap() FAILED!\n");
	
	/* close the file */
	close(fd);
	
	/* memset() 0 in the arrays */
	memset(counting_array, 0, sizeof(counting_array));
	memset(result_array, 0, sizeof(result_array));
	memset(counting_array_ptrs, 0, sizeof(counting_array_ptrs));
	
	/* for each num of threads */
	for (i = 0; i < num_threads; ++i)
	{
		/* assign in the pointers array a pointer to a counting array */
		counting_array_ptrs[i] = counting_array[i];
		
		/* in the array of thread data, assign in current index the start and end pointers and pointer to
			a counting array */
		thread_data[i].start = file_data + (i * section_size);
		thread_data[i].end = (i == num_threads - 1) ? (file_data + file_size) : (file_data + ((i + 1) * section_size)); /* if its the last thread, assign the end of the file */
		thread_data[i].counting_array = counting_array_ptrs[i];
		
		/* create a thread */
		status = pthread_create(&threads[i], NULL, ThreadFunc, &thread_data[i]);
		RET_IF_BAD_SC(status, FAIL, "MTCounting(): pthread_create() FAILED!\n");
	}

	/* for each num of threads */
	for (i = 0; i < num_threads; ++i)
	{
		/* join the thread */
		status = pthread_join(threads[i], NULL);
		RET_IF_BAD_SC(status, FAIL, "MTCounting(): pthread_join() FAILED!\n");
	}
	
	/* merge the couting arrays into result array */
	MergeResults(&counting_array[0][0], result_array, num_threads);
	
	/* print result array */
	PrintResult(result_array);
	
	/* free the mmap (call munmap()) ?? */
	munmap((void*)file_data, file_size);
		
	/* return SUCCESS */
	return SUCCESS;
}

static void* ThreadFunc(void* arg)
{
	thread_data_t* thread_data = (assert(NULL != arg), (thread_data_t*)arg);
	const char* curr_pos = thread_data->start;
	unsigned char curr_char = '\0';

	/* while current position is less than the end of the section */
	while (curr_pos < thread_data->end)
	{
		/* assign the current char to a local variable */
		curr_char = (unsigned char)(*curr_pos);
			
		/* increase the array in index of the current char */
		++(thread_data->counting_array[curr_char]);
		
		/* increase current position pointer */
		++curr_pos;
	}
		
	/* return NULL */
	return NULL;
}

static void MergeResults(size_t* counting_array, size_t result_array[CHAR_SIZE], int num_threads)
{
	int i = 0, j = 0;
	
	/* assert */
	assert(NULL != counting_array);
	assert(NULL != result_array);

	/* for each number of threads */
	for (i = 0; i < num_threads; ++i)
	{
		/* for each number of size of char */
		for (j = 0; j < CHAR_SIZE; ++j)
		{
			/* add the value from counting array to result array */
			result_array[j] += counting_array[i * CHAR_SIZE + j];
		}
	}
}

static void PrintResult(size_t result_array[CHAR_SIZE])
{
	int i = 0;
	
	/* assert */
	assert(NULL != result_array);
	
	/* for each number of size of char */
	for (i = 0; i < CHAR_SIZE; ++i)
	{
		/* if the current char appeared at least once */
		if (result_array[i] > 0)
		{
			/* print it */
			printf("char %c -> %lu times\n", i, (size_t)result_array[i]);
		}
	}
}
