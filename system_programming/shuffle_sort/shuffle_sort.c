/**
	Written By: Tom Golzman
	Date: 22/07/2025
	Reviewed By: 
**/

/************************************ Includes *************************************/
#include <assert.h> /* assert() */

#include "utils.h"	/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

/************************************* Defines *************************************/
enum
{
	NUM_WORDS = 2086680,
	WORDS_ARRAY_COPIES = 5
};

/* struct to pass argument to the thread */
{
	char** array;
	size_t size;
}

/******************************** Static Functions *********************************/

/************************************ Functions ************************************/
int main()
{
	/* load file into buffer and get file size */
	
	/* fill words array from the buffer */
	
	/* duplicate words array into a big array */
	
	/* for each number of threads */
	{
		/* assign in arguments array pointer to section's array and the size */
		
		/* create a thread */
	}
	
	/* for each number of threads */
	{
		/* join the thread */
	}
	
	/* merge section's arrays from all the threads into merged array */

	/* return SUCCESS */
}

static void* ThreadFunc(void* arg_)
{
	/* assert */
	
	/* qsort() with CompareWords() */
	
	/* return NULL */
}

static char* LoadFile(const char* file_path_, size_t* file_size_out_)
{
	/* assert */
	
	/* open the file & handle failure */
	
	/* calculate file size */
	
	/* allocate memory for the buffer */
	
	/* read from the file into the buffer */
	
	/* add null terminator at the end */
	
	/* close the file */
	
	/* return the buffer */
}

static void FillWordsArray(char** words_, char* buffer_)
{
	/* assert */
	
	/* while current char is not the end of the file */
	{
		/* if the current char is new line */
		{
			/* replace it with 0 */
			
			/* assign in words array a pointer to the current word */
			
			/* increment words index */
		}
		
		/* increment current char */
	}
}

static void DuplicateArray(char** dest_, char** src_, size_t num_words_, size_t num_copies_)
{
	/* assert */
	
	/* for each num of copies */
	{
		/* memcpy() number of words */
	}
}

static void ShuffleArray(char** arr_, size_t size_)
{
	/* assert */
	
	/* qsort() with RandomCompare() */
}

static int RandomCompare(const void* word1_, const void* word2_)
{
	/* assert */
	
	/* return -1 or 0 or 1 */
	return (rand() % 3) - 1;
}

static void MergeArrays(char** dest_, char** left_, char** right_, size_t left_size_, size_t right_size_)
{
	/* assert */

	/* lock mutex */
	
	/* while current left is smaller than left_size and current right is smaller than right_size */
	{
		/* if left word is before the right word */
		{
			/* copy left word into dest */
			
			/* increment right and left indexes */
		}
		
		/* else */
		{
			/* copy right word into dest */
			
			/* increment right and left indexes */
		}
	}

	/* copy the remaining words: */
	
	/* while current left is smaller than left_size */
	{
		/* copy left word into dest */
			
		/* increment left index */
	}
	
	/* while current right is smaller than right_size */
	{
		/* copy right word into dest */
			
		/* increment right indexe */
	}
	
	/* unlock mutex */	
}
