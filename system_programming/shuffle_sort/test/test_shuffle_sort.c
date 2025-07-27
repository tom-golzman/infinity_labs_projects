/************************************ Includes *************************************/
#include <stdio.h> /* printf */
#include <sys/time.h>	/* struct timeva, gettimeofday() */

#include "test_utils.h" /* colors, titles, status, boolean */
#include "shuffle_sort.h"

/************************************* Defines *************************************/
enum
{
	NUM_THREADS = 10
};

/************************************** main ***************************************/
int main(void)
{
	int result = 0;
	int status = 0;
	size_t i = 1;
	struct timeval start, end;
	double elapsed_sec = 0;
	double min_time = 10000;
	size_t best_num_threads = -1;
	const char* path = "/home/tom/git/system_programming/shuffle_sort/linux_dict_5x.txt";
	
	printf(BOLD_TITLE "\nTest: ()\n" RESET);
	
	for (i = 1; i <= NUM_THREADS; ++i)
	{
		status = gettimeofday(&start, NULL);
		if (-1 == status)
		{
			printf("gettimeofday(&start) FAILED!\n");
			
			return FAIL;
		}

		result = ShuffleSort(path, i);

		status = gettimeofday(&end, NULL);
		if (-1 == status)
		{
			printf("gettimeofday(&end) FAILED!\n");
			
			return FAIL;
		}
		
		elapsed_sec = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
				
		if (SUCCESS != result)
		{
			printf(RED "Test with %ld threads FAILED!\n" RESET, i);
		}
		
		if (elapsed_sec < min_time)
		{
			min_time = elapsed_sec;
			best_num_threads = i;
		}
		
		printf(GREEN "%ld threads -> %.5f sec\n\n" RESET, i, elapsed_sec);
	}
	
	printf(BOLD "ideal num threads: %ld (time: %.5f)\n" RESET, best_num_threads, min_time);
	
	printf("\n");
	
	return (0);
}
