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
static volatile sig_atomic_t got_sigusr1 = FALSE;

static pid_t parent_pid = 0;

/********************************Private Functions********************************/
static void ChildHandler(int sig);

/************************************Functions************************************/
int main()
{
	int status = 0;
	
	/* create sigaction variable */
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	/* intialize parent handler in the struct */
	sa.sa_handler = ChildHandler;

	/* determine signal action */
	ExitIfBad(0 == sigaction(SIGUSR1, &sa, NULL), FAIL, "sigaction() failed!\n");
	
	/* get parent pid */
	parent_pid = getppid();

	while (1)
	{
		/* send signal SIGUSR2 to the parent */
		DEBUG_ONLY(printf("Child (%d): sending SIGUSR2 to parent (%d)\n", getpid(), parent_pid););
		status = kill(parent_pid, SIGUSR2);
		ExitIfBad(-1 != status, FAIL, "kill() FAILED!\n");

		if (!got_sigusr1)
		{
			status = pause();
			ExitIfBad(-1 == status, FAIL, "pause() FAILED!\n");
		}
		
		DEBUG_ONLY(printf("Child (%d): received SIGUSR1 from parent (%d)\n", getpid(), parent_pid););
		got_sigusr1 = FALSE;
		
		/* send signal SIGUSR2 to the parent 
		DEBUG_ONLY(printf("Child: sending SIGUSR2 to parent\n"););
		DEBUG_ONLY(-1 == kill(parent_pid, SIGUSR2), FAIL, "kill() FAILED!\n");*/
	}

	return SUCCESS;
}

static void ChildHandler(int sig)
{
	/* assert that the received signal is SIGUSR1 */
	assert(SIGUSR1 == sig);

	got_sigusr1 = TRUE;
}
