/************************************ Includes ************************************/
#include <utils.h>

/************************************ Functions ***********************************/
void ExitIfBad(int is_good, int exit_status, const char* message)
{
	if(!is_good)
	{
		/* print message to std error */
		fputs(message, stderr);
		
		/* exit from the function with exit status */
		_exit(exit_status);
	}
}

void Log(const char* message)
{
    /* print message to stderr */
    perror(message);
}

void LogIfBad(int is_good, const char* message)
{
	if(!is_good)
	{
		/* print message to std error */
		fputs(message, stderr);
	}
}

/** My strdup function **/
char* StrDup(const char* str)
{
	size_t len = strlen(str) + 1; /* +1 for '\0' */
	char* dup = (char*)malloc(len * sizeof(char)); 
	
	assert(str != NULL);
	assert(dup != NULL);
	
	strncpy(dup, str, len);
	
	return (dup);
}
