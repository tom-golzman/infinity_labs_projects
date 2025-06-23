/**
	Written By: Tom Golzman
	Date: 21/06/2025
	Reviewed By: Sami
**/

/************************************includes************************************/
#define _POSIX_C_SOURCE 200809L

#include <assert.h>	/* assert() */
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
	int status = 0;
	
	/* create sigaction variable */
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	/* intialize child handler in the struct */
	sa.sa_handler = ParentHandler;

	/* determine signal action */
	status = sigaction(SIGUSR2, &sa, NULL);
	ExitIfBad(0 == status, FAIL, "sigaction() failed!\n");
	
	/* create child process */
	child_pid = fork();
	
	/* handle failure */
	ExitIfBad(child_pid >= 0, FAIL, "fork() FAILED!");
	
	/* child process */
	if (0 == child_pid)
	{
		execl("./pong", "pong", NULL);
		
		/* handle failure */
		ExitIfBad(child_pid == 0, FAIL, "execl() FAILED!\n");
	}
	
	/* wait for a signal from the child */
	status = pause();
	ExitIfBad(-1 == status, FAIL, "pause() FAILED!\n");

	DEBUG_ONLY(printf("Parent: received first signal from the child\n"););

	while (rounds_counter < NUM_ROUNDS)
	{
		/* send signal to the child process */
		DEBUG_ONLY(printf("Parent: sending SIGUSR1 to child\n"););
		status = kill(child_pid, SIGUSR1);
		ExitIfBad(-1 != status, FAIL, "kill() FAILED!\n");

		/* wait for a signal from the child */
		if (!got_sigusr2)
		{
			status = pause();
			ExitIfBad(-1 == status, FAIL, "pause() FAILED!\n");
		}
			
		DEBUG_ONLY(printf("Parent: received SIGUSR2 from child\n"););
		got_sigusr2 = FALSE;

		++rounds_counter;
	}

	DEBUG_ONLY(printf("Parent: done after %d rounds\n", rounds_counter););
		
	return SUCCESS;
}

static void ParentHandler(int sig)
{
	/* assert that the received signal is SIGUSR2 */
	assert(SIGUSR2 == sig);

	got_sigusr2 = TRUE;
}
