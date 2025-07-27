/************************************ Includes ************************************/
#include <stdio.h>		/* sigaction, sig_atomic_t */

#include "utils.h"		/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

#include "wd.h"

enum { MAX_MISSES = 6, INTERVAL = 2 };

/******************************** main ********************************/
int main(int argc, char* argv[])
{
	/* call MakeMeImmortal() */
	MakeMeImmortal(argc, argv, MAX_MISSES, INTERVAL ,"/home/tom/git/utils/wd_exe/wd_launch.out");
	
	sleep(2);
	
	/* call DNR() */
	DNR();
	
	/* return SUCCESS */
	return SUCCESS;
}
