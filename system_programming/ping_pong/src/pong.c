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
static volatile sig_atomic_t got_sigusr1 = FALSE;

static pid_t parent_pid = 0;

/********************************Private Functions********************************/
static void ChildHandler(int sig);

/************************************Functions************************************/
int main()
{
	/* create sigaction variable */
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	/* intialize parent handler in the struct */
	sa.sa_handler = ChildHandler;

	/* determine signal action */
	sigaction(SIGUSR2, &sa, NULL);
	
	/* get parent pid */
	parent_pid = getppid();
	
	printf("Child (pong): my pid = %d, my parent pid = %d\n", getpid(), parent_pid);

	while (1)
	{
		if (got_sigusr1)
		{
			printf("Child: received SIGUSR1 from parent\n");
			got_sigusr1 = FALSE;

			sleep(1);
			
			/* send signal SIGUSR2 to the child */
			printf("Child: sending SIGUSR2 to parent\n");
			kill(parent_pid, SIGUSR2);
		}
	}

	return SUCCESS;
}

static void ChildHandler(int sig)
{
	/* if the received signal is SIGUSR1 */
	if (SIGUSR1 == sig)
	{
		printf("Child: received SIGUSR1 from parent\n");
		
		/* send signal SIGUSR2 to the parent */
		got_sigusr1 = TRUE;
	}
}
