/**
	Written By: Tom Golzman
	Date: 18/06/2025
	Reviewed By: 
**/

/************************************includes************************************/
#include <assert.h> 	/* assert() */
#include <stdio.h>		/* printf() */
#include <string.h>		/* strcspn(), strcmp(), strncpy(), strtok() */
#include <stdlib.h>		/* free() */
#include <unistd.h>     /* fork(), execvp() */
#include <sys/types.h>  /* pid_t */
#include <sys/wait.h>   /* waitpid() */

#include "utils.h"		/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM() */

#include "simple_shell.h"

/************************************define************************************/
enum { MAX_COMMAND_LEN = 1024 , MAX_ARGS = 64 };

/********************************Private Functions********************************/
static void ExecCommand(const char* input);
static int ParseInput(const char* input, char** arguments);
static void FreeArguments(char** arguments, int num_args);
static char* StrDup(const char *str);

/************************************Functions************************************/
void RunShell()
{
	char input[MAX_COMMAND_LEN];
	
	/* while */
	while (1)
	{
		printf("simple shell -> ");
		/* send immediately the text to stdout */		
		fflush(stdout);
		
		/* get input from the user and handle fail */
		if (NULL == fgets(input, MAX_COMMAND_LEN, stdin))
		{
			return;
		}
		
		/* change the '\n' in the string to '\0' */
		input[strcspn(input, "\n")] = '\0';
		
		/* if the input is 'exit' */
		if (0 == strcmp(input, "exit"))
		{
			/* handle exit */
			return;
		}
		
		/* execute the command */
		ExecCommand(input);
	}
}

/********************************Private Functions********************************/
static void ExecCommand(const char* input)
{
	char* arguments[MAX_ARGS];
	int num_args = 0;
	int status = FAIL;
	pid_t pid = -1;

	/* assert */
	assert(NULL != input);
	
	/* parse the input */
	num_args = ParseInput(input, arguments);
	
	/* if there isnt a command */
	if (0 == num_args)
	{
		/* do nothing */
		return;	
	}
	
	/* create a new process */
	pid = fork();
	/* handle fail */
	if (pid < 0)
	{
		FreeArguments(arguments, num_args);
		
		return;
	}
	
	/* if son */
	if (0 == pid)
	{
		/* put NULL at the end of the arguments array (for execvp) */
		arguments[num_args] = NULL;
		 
		/* exec the given executable */
		execvp(arguments[0], arguments);
		
		/* handle fail */
	
	}
		
	/* if father */
	else
	{
		/* wait for the son */
		waitpid(pid, &status, 0);
		
		/* free the arguments array */
		FreeArguments(arguments, num_args);
	}
}

/* @RETURN: number of arguments , -1 on error */
static int ParseInput(const char* input, char** arguments)
{
	char buffer [MAX_COMMAND_LEN];
	char* token = NULL;
	int curr_idx = 0;
	
	/* assert */
	assert(NULL != input);
	assert(NULL != arguments);
	
	/* copy the input to a temp buffer */
	strncpy(buffer, input, MAX_COMMAND_LEN);
	/* put null terminator at the end of the buffer */
	buffer[MAX_COMMAND_LEN - 1] = '\0';
	
	/* split the buffer string by spaces */
	token = strtok(buffer, " ");
	
	/* while the token isn't NULL and not reached end of input string */
	while (NULL != token && curr_idx < MAX_ARGS - 1)
	{
		/* duplicate each token and put it in the arguments array */
		arguments[curr_idx] = StrDup(token);
		/* handle fail */
		if (NULL == arguments[curr_idx])
		{
			/* free what was allocated so far */
			FreeArguments(arguments, curr_idx);
		
			return -1;
		}
		
		/* increase index and get the next token */
		++curr_idx;
		token = strtok(NULL, " ");
	}
		
	/* add null at the end of the arguments array (required by execvp) */
	arguments[curr_idx] = NULL;
	
	/* return number of arguments */
	return curr_idx;
}

static void FreeArguments(char** arguments, int num_args)
{
	int i = 0;

	for (i = 0; i < num_args; ++i)
	{
		free(arguments[i]);
	}
	
}

static char* StrDup(const char *str)
{
	char* dup = NULL;

	assert(NULL != str);

	dup = malloc(strlen(str) + 1);

	if (NULL != dup)
	{
		strcpy(dup, str);
	}
	
	return dup;
}
