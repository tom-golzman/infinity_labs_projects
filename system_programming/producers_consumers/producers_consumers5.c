/**
	Written By: Tom Golzman
	Date: 06/07/2025
	Reviewed By: 
**/

/************************************includes*************************************/
#include <assert.h>		/* assert() */
#include <pthread.h>	/* pthread_t */
#include <stdlib.h>		/* calloc(), free() */
#include <semaphore.h>	/* sem_t */
#include <stdio.h>		/* printf() */

#include "utils.h"		/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

/**************************************define*************************************/
enum { NUM_ROUNDS = 1, NUM_PRODUCERS = 5, NUM_CONSUMERS = 5 };

static dll_t* shared_list = NULL;
static pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;
static sem_t items_available;

/********************************Private Functions********************************/
static void* ProduceThreadFunc(void* arg);
static void* ConsumeThreadFunc(void* arg);
static int* Produce(int val);
static void Consume(int* data);

/************************************Functions************************************/
int main()
{
