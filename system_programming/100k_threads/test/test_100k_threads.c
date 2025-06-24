/************************************includes************************************/
#include <stdio.h>		/* printf */
#include <assert.h>		/* assert() */
#include <time.h>		/* time_t */

#include "test_utils.h" /* colors, titles, status, boolean */
#include "100k_threads.h"

/************************************define************************************/
enum { NUM_PRINTS = 10 };

/************************************Functions Forward Decleration************************************/
void TestEx1();

static int IsArrValid(int* arr, size_t size);

/************************************main************************************/
int main(void)
{
	TestEx1();
	
	printf("\n");
	
	return SUCCESS;	
}

/************************************Functions************************************/
void TestEx1()
{
	int arr[NUM_THREADS] = {0};
	time_t end, start;

    printf(BOLD_TITLE "\nTest: ()\n" RESET);

	start = time(NULL);
	InitArray(arr, NUM_THREADS);
	end = time(NULL);	

	printf("time = %f\n", difftime(end, start));
	
	if (IsArrValid(arr, NUM_PRINTS))
	{
		printf(GREEN "Test passed!\n" RESET);
	}
	else
	{
		printf(RED "Test FAILED!\n" RESET);
	}
}

static int IsArrValid(int* arr, size_t size)
{
	size_t i = 0;
	
	assert(NULL != arr);
	
	for (i = 0; i < size; ++i)
	{
		if (arr[i] != i)
		{
			printf("arr[%lu] = %d\n", i, arr[i]);
			return FALSE;
		}
	}
	
	return TRUE;
}

