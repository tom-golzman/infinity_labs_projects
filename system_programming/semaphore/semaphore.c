/**
	Written By: Tom Golzman
	Date: 30/06/2025
	Reviewed By: 
**/

/************************************includes************************************/
#include <assert.h> /* assert() */

#include "utils.h"	/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */
#include "semaphore.h"

/************************************define************************************/
enum { MAX_FILE_NAME = 10, NUM_ARGS = 3 };

typedef union semnum
{
	int val;
	struct semid_ds* buf;
	unsigned short* array;
	struct seminfo* __buf;
} semnum_t;

/************************************Functions************************************/
int main (int argc, char* argv[])
{
	key_t key = -1;
	char file_path[6 + MAX_FILE_NAME] = "/tmp/";
	int fd = -1;
	int semid = -1;
	int status = FAIL;
	
	assert(argc > 1);
	
	/* create new file in /tmp */
	strcat(file_path, argv[1]);
	fd = open(file_path, 1);
	/* handle failure */
	ExitIfBad(-1 != fd, FAIL, "open() FAILED!");
	
	/* create key with ftok() */
	key = ftok(file_path, 1);
	/* handle failure */
	ExitIfBad(-1 != key, FAIL, "ftok() FAILED!");

	/* create semaphore with semget() */
	semid = semget(key, 1, 0666 | IPC_CREAT);
	/* handle failure */
	ExitIfBad(-1 != semid, FAIL, "semget() FAILED!");

	/* initialize semaphore value */
	status = InitSemaphore(semid, 1);
	/* handle failure */
	ExitIfBad(-1 != status, FAIL, "InitSemaphore() FAILED!");
}

static int InitSemaphore(int semaphore, int value)
{
	int status = -1;
	
	/* create semnum variable */
	semnum_t sem_value;
	
	/* initialize semnum value */
	sem_value.val = value;

	/* initialize semaphore value with semctl() */
	return semctl(semid, 0 SETVAL, sem_value);
}

static void ParseInput(char* input, char** args)
{
	char* tok = NULL;
	char** curr = NULL;
	char** end = NULL;
	
	assert(NULL != input);
	assert(NULL != args);
	
	curr = args;
	end = args + NUM_ARGS - 1;
	tok = strtok(input, " ");
	
	while (curr != end && NULL != tok)
	{
		*curr = tok;
		tok = strtok(NULL, " ");
		++curr;
	}
}

static int SemManipultaion(int semid)
{

	/* loop */
		/* get input and parse it */
		
		/* switch input */
			/* D: */
				/* decrement the value of semaphore */
			/* I: */
				/* increment the value of semaphore */
			/* V: */
				/* view the value of semaphore */
			/* X: */
				/* exit the program */
	
}

