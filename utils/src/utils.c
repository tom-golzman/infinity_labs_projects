#include <unistd.h>	/* _exit */
#include <stdio.h>	/* fputs */

void ExitIfBad(int is_good, int exit_status, const char* message)
{
	if(!is_good)
	{
		fputs(stderr, message);
		
		_exit(exit_status);
	}
}

