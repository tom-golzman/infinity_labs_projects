/**
	Written By: Tom Golzman
	Date: 19/06/2025
	Reviewed By: Ola Kaz
**/

/************************************includes************************************/
#define _POSIX_C_SOURCE 200809L

#include <assert.h>	/* assert() */
#include <unistd.h>	/* pid_t, fork(), getpid(), getppid() */
#include <signal.h>	/* struct sigaction, SIGUSR1, SIGUSR2 */
#include <stdio.h>	/* printf() */
#include <stdlib.h>	/* exit() */
#include <string.h>	/* memset() */

#include "utils.h"	/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */
#include "ping_pong.h"

/************************************define************************************/
enum { NUM_ROUNDS = 3 };

/********************************Global Variables*********************************/
static volatile sig_atomic_t parent_running = TRUE;
static volatile sig_atomic_t child_running = TRUE;

static volatile sig_atomic_t got_sigusr1 = FALSE;
static volatile sig_atomic_t got_sigusr2 = FALSE;

static pid_t parent_pid = 0;
static pid_t child_pid = 0;
static int rounds_counter = 0;

/********************************Private Functions********************************/
static void RunParent(void);
static void RunChild(void);
static void ChildHandler(int sig);
static void ParentHandler(int sig);

/************************************Functions************************************/
void PingPong1()
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	
	/* save current pid as parent pid */
	parent_pid = getpid();
	
	/* create a child process */
	child_pid = fork();
	
	/* handle failure */
	ExitIfBad(child_pid >= 0, 1, "fork() FAILED!");
	
	/* child process */
	if (0 == child_pid)
	{
		RunChild();
	}
	
	/* parent process */
	else
	{
		sleep(1);
	
		RunParent();
		
		wait(NULL);
	}
}

/********************************Private Functions********************************/
static void RunChild(void)
{
	int status = 0;
	
	/* create sigaction variable */
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	/* save praent id */
	parent_pid = getppid();

	/* intialize child handler in the struct */
	sa.sa_handler = ChildHandler;

	/* determine signals action */
	ExitIfBad(0 == sigaction(SIGUSR1, &sa, NULL), FAIL, "sigaction() failed!\n");

	/* while the child process is running, pause until it gets signal */
	while (child_running)
	{
		status = pause();
		ExitIfBad(-1 == status, FAIL, "pause() FAILED!\n");
		
		if (got_sigusr1)
		{
			DEBUG_ONLY(printf("child: received SIGUSR1 from parent\n"););
			got_sigusr1 = FALSE;

			/* send signal SIGUSR2 to the parent */
			status = kill(parent_pid, SIGUSR2);
			ExitIfBad(-1 != status, FAIL, "kill() FAILED!\n");	
			
			++rounds_counter;

			if (rounds_counter == NUM_ROUNDS)
			{
				DEBUG_ONLY(printf("child: done after %d rounds\n", rounds_counter););
				child_running = FALSE;
			}
		}
	}

	exit(SUCCESS);
}

static void RunParent(void)
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
	
	/* send a signal to the child */
	DEBUG_ONLY(printf("parent: sending SIGUSR1 to child\n"););
	status = kill(child_pid, SIGUSR1);
	ExitIfBad(-1 != status, FAIL, "kill() FAILED!\n");	
	
	/* while the parent process is running, pause until it gets signal */
	while (parent_running)
	{
		status = pause();
		ExitIfBad(-1 == status, FAIL, "pause() FAILED!\n");
		
		if (got_sigusr2)
		{
			DEBUG_ONLY(printf("parent: received SIGUSR2 from child\n"););
			++rounds_counter;
			got_sigusr2 = FALSE;

			if (rounds_counter == NUM_ROUNDS)
			{
				DEBUG_ONLY(printf("parent: done after %d rounds\n", rounds_counter););
				parent_running = FALSE;
			}
			else
			{
				status = kill(child_pid, SIGUSR1);
				ExitIfBad(-1 != status, FAIL, "kill() FAILED!\n");
			}
		}
	}
}

static void ChildHandler(int sig)
{
	/* assert that the received signal is SIGUSR1 */
	assert(SIGUSR1 == sig);

	got_sigusr1 = TRUE;
}

static void ParentHandler(int sig)
{
	/* assert that the received signal is SIGUSR2 */
	assert(SIGUSR2 == sig);

	got_sigusr2 = TRUE;
}
