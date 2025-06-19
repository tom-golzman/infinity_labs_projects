#include <unistd.h>	/* _exit */
#include <stdio.h>	/* fputs */

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

