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

void Log(const char* format, ...)
{
	va_list args;
	
	va_start(args, format);
	
	/* print message to stderr */
	vfprintf(stderr, format, args);
	
	va_end(args);
}

void LogIfBad(int is_good, const char* message)
{
	if(!is_good)
	{
		/* bold ON */
		fprintf(stderr, "\033[1m");
	
		/* print message to std error */
		fputs(message, stderr);

		/* bold OFF */
		fprintf(stderr, "\033[0m");
	}
}

/** My strdup function **/
char* StrDup(const char* str)
{
	size_t len = strlen(str) + 1; /* +1 for '\0' */
	char* dup = (char*)malloc(len * sizeof(char)); 
	
	RET_IF_BAD(NULL != dup, NULL, "StrDup(): malloc() FAILED!\n");
	
	assert(str != NULL);
	assert(dup != NULL);
	
	strncpy(dup, str, len);
	
	return (dup);
}
