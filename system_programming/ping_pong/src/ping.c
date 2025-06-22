/**
	Written By: Tom Golzman
	Date: 21/06/2025
	Reviewed By: 
**/

/************************************includes************************************/
#define _POSIX_C_SOURCE 200809L

#include <unistd.h>	/* pid_t, fork() */
#include <signal.h>	/* struct sigaction, SIGUSR1, SIGUSR2 */
#include <stdio.h>	/* printf() */
#include <stdlib.h>	/* exit() */
#include <string.h>	/* memset() */

#include "utils.h"	/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

/************************************define************************************/
enum { NUM_ROUNDS = 3 };

/********************************Global Variables*********************************/
static volatile sig_atomic_t got_sigusr2 = FALSE;

static pid_t child_pid = 0;

static int rounds_counter = 0;

/********************************Private Functions********************************/
static void ParentHandler(int sig);

/************************************Functions************************************/
int main()
{
	/* create sigaction variable */
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	/* intialize child handler in the struct */
	sa.sa_handler = ParentHandler;

	/* determine signal action */
	ExitIfBad(0 == sigaction(SIGUSR2, &sa, NULL), FAIL, "sigaction() failed!\n");
	
	/* create child process */
	child_pid = fork();
	
	/* handle failure */
	ExitIfBad(child_pid >= 0, FAIL, "fork() FAILED!");
	
	/* child process */
	if (0 == child_pid)
	{
		execl("./pong", "pong", NULL);
		
		/* handle failure */
		ExitIfBad(child_pid == 0, FAIL, "execl() FAILED!");
	}
	
	/* wait for a signal from the child */

		pause();

	printf("Parent: received first signal from the child\n");

	while (rounds_counter < NUM_ROUNDS)
	{
		/* send signal to the child process */
		printf("Parent: sending SIGUSR1 to child\n");		
		kill(child_pid, SIGUSR1);

		/* wait for a signal from the child */
		if (!got_sigusr2)
		{
			pause();
		}
			
		printf("Parent: received SIGUSR2 from child\n");
		got_sigusr2 = FALSE;

		++rounds_counter;
	}

	DEBUG_ONLY(printf("Parent: done after %d rounds\n", rounds_counter););
		
	return SUCCESS;
}

static void ParentHandler(int sig)
{
	/* if the received signal is SIGUSR2 */
	if (SIGUSR2 == sig)
	{
		/* send signal SIGUSR1 to the child */
		got_sigusr2 = TRUE;
	}
}
