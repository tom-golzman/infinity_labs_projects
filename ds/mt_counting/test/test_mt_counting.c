/************************************includes************************************/
#include <stdio.h>		/* printf */
#include <sys/time.h>	/* struct timeva, gettimeofday() */

#include "test_utils.h" /* colors, titles, status, boolean */
#include "mt_counting.h"

/************************************define************************************/
enum
{
	NUM_THREADS = 20
};

/************************************main************************************/
int main(void)
{
	int result = 0;
	int i = 1;
	struct timeval start, end;
	double elapsed_sec = 0;
	double min_time = 10000;
	int best_num_threads = -1;
	
	printf(BOLD_TITLE "\nTest: ()\n" RESET);
	
	for (i = 1; i <= NUM_THREADS; ++i)
	{
		gettimeofday(&start, NULL);
		
		result = MTCounting("/home/tom/git/ds/mt_counting/linux_dict_10x.txt", i);	
		
		gettimeofday(&end, NULL);
		
		elapsed_sec = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
				
		if (SUCCESS != result)
		{
			printf(RED "Test with %d threads FAILED!\n" RESET, i);
		}
		
		if (elapsed_sec < min_time)
		{
			min_time = elapsed_sec;
			best_num_threads = i;
		}
		
		printf(GREEN "%d threads -> %.5f sec\n\n" RESET, i, elapsed_sec);
	}
	
	printf(BOLD "ideal num threads: %d (time: %.5f)\n" RESET, best_num_threads, min_time);
	
	printf("\n");
	
	return (0);
}
