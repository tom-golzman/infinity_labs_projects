/************************************ Includes *************************************/
#include <stdio.h>		/* printf */
#include <sys/time.h>	/* struct timeva, gettimeofday() */
#include <string.h>		/* memcpy() */
#include <time.h>		/* time() */
#include <stdlib.h>		/* qsort() */

#include "test_utils.h" /* colors, titles, status, boolean */
#include "shuffle_sort.h"

/************************************* Defines *************************************/
enum
{
	NUM_THREADS = 10,
	WORDS_ARRAY_COPIES = 5,
	NUM_WORDS = 521670
};

/************************************** main ***************************************/
int main(void)
{
	char** result = NULL;
	int status = 0;
	size_t i = 0, j = 0;
	struct timeval start, end;
	double elapsed_sec = 0;
	double min_time = 10000.0;
	size_t best_num_threads = 0;
	const char* path = "/home/tom/git/system_programming/shuffle_sort/linux_dict_5x.txt";
	
	printf(BOLD_TITLE "\nTest: ShuffleSort() with multiple threads\n" RESET);
	
	for (i = 1; i <= NUM_THREADS; ++i)
	{
		status = gettimeofday(&start, NULL);
		if (-1 == status)
		{
			printf("gettimeofday(&start) FAILED!\n");
			return FAIL;
		}

		result = ShuffleSort(path, i);
		if (NULL == result)
		{
			printf(RED "ShuffleSort() returned NULL\n" RESET);
			return FAIL;
		}

		status = gettimeofday(&end, NULL);
		if (-1 == status)
		{
			printf("gettimeofday(&end) FAILED!\n");
			return FAIL;
		}

		for (j = 0; j < NUM_WORDS * WORDS_ARRAY_COPIES - 1; ++j)
		{
			if (strcmp(result[j], result[j + 1]) > 0)
			{
				printf(RED "FAILED at index %lu: \"%s\" > \"%s\"\n" RESET, j, result[j], result[j + 1]);
				free(result);
				return FAIL;
			}
		}

		elapsed_sec = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
		if (elapsed_sec < min_time)
		{
			min_time = elapsed_sec;
			best_num_threads = i;
		}

		printf(GREEN "%lu threads → %.5f sec\n" RESET, i, elapsed_sec);
	}

	printf(BOLD "Ideal number of threads: %lu (%.5f sec)\n\n" RESET, best_num_threads, min_time);
	
	return SUCCESS;
}
