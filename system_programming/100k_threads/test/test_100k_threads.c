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
void TestEx3();
void TestSimpleWorkLoad();

static int IsArrValid(int* arr, int size);

/************************************main************************************/
int main(void)
{
	TestEx1();
	TestEx3();
	TestSimpleWorkLoad();
	
	printf("\n");
	
	return SUCCESS;
}

/************************************Functions************************************/
void TestEx1()
{
	int arr[NUM_THREADS] = {0};
	time_t end, start;

    printf(BOLD_TITLE "\nTest InitArr\n" RESET);

	start = time(NULL);
	InitArray(arr, NUM_THREADS);
	sleep(1);
	end = time(NULL);	

	printf("time = %.0f seconds\n", difftime(end, start));
	
	if (IsArrValid(arr, NUM_PRINTS))
	{
		printf(GREEN "Test passed!\n" RESET);
	}
	else
	{
		printf(RED "Test FAILED!\n" RESET);
	}
}

void TestEx3()
{
	int arr[NUM_THREADS] = {0};
	size_t result = 0;
	time_t end, start;

    printf(BOLD_TITLE "\nTest WorkLoad\n" RESET);

	start = time(NULL);
	result = WorkLoad(arr, NUM_THREADS);
	sleep(1);
	end = time(NULL);	

	printf("time = %.0f seconds\n", difftime(end, start));
	
	printf("result = %lu\n", result);
}

void TestSimpleWorkLoad()
{
	size_t result = 0;
	size_t expected_result = 3211610688;
	time_t end, start;
		
	printf(BOLD_TITLE "\nTest Simple WorkLoad\n" RESET);
	
	start = time(NULL);
	result = SimpleWorkLoad();
	sleep(1);
	end = time(NULL);	

	printf("time = %.0f seconds\n", difftime(end, start));   
	
	if (result == expected_result)
	{
		printf(GREEN "Test passed!\n" RESET);
	}
	else
	{
		printf(RED "Test FAILED! result = %lu | expected = %lu\n" RESET, result, expected_result);
	}
}

static int IsArrValid(int* arr, int size)
{
	int i = 0;
	
	assert(NULL != arr);
	
	for (i = 0; i < size; ++i)
	{
		if (arr[i] != i)
		{
			printf("arr[%d] = %d\n", i, arr[i]);
			return FALSE;
		}
	}
	
	
	
	return TRUE;
}

