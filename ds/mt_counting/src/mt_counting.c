/**
	Written By: Tom Golzman
	Date: 20/07/2025
	Reviewed By: 
**/

/************************************includes************************************/
#include <assert.h> /* assert() */

#include "utils.h"	/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */
#include "mt_counting.h"

/************************************define************************************/
/* struct to pass data to the thread */
{
	const char* start;
	const char* end;
	size_t* counting_arr;
}

/************************************Functions************************************/
int main(int argc, char* argv[])
{
	/* local variables: */
		/* file descriptor, pointer to the file, file size, array of couting arrays, result array,
			array of threads, counting array, array of thread data */
	
	/* assert */
	
	/* open the file & handle failure */	
	
	/* calculate the file size */
	
	/* assign to a local variable the size of a section (file size / num of threads) */
	
	/* call mmap() */
	/* handle failure */
	
	/* close the file */
	
	/* memset() 0 in the result array */

	/* memset() 0 in the counting arrays */
	
	/* for each num of threads */
	{
		/* assign in the pointers array a pointer to a counting array */
	}
	
	/* for each num of threads */
	{
		/* in the array of thread data, assign in current index the start and end pointers and pointer to
			a counting array */
		
		/* create a thread */
	}

	/* for each num of threads */
	{	
		/* join the thread */
		
		/* for each index in range 0-255 */
		{
			/* add the value from result array, to counting array */
		}
	}
	
	/* free the mmap (call munmap()) ?? */
	
	/* for each index in range 0-255 */
	{
		/* print the value */
	}
	
	/* return SUCCESS */
}

static void* ThreadFunc(void* arg)
{
	/* assert */
	
	/* while curr is less than the end of the section */
	
		/* assign the current char to a local variable */
		
		/* increase the array in index of the char */
		
		/* increase curr pointer */
		
	/* return NULL */
}
