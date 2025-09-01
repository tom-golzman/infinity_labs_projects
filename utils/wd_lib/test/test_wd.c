#include <stdio.h>

#include "test_utils.h"
#include "wd.h"

enum
{ 
	MAX_MISSES = 6,
	INTERVAL = 2 
};

int main(int argc, char* argv[])
{
	MakeMeImmortal(argc, argv, MAX_MISSES, INTERVAL ,"/home/tom/git/utils/wd_exe/wd_launch.out");
	
	sleep(2);
	
	DNR();
	
	return SUCCESS;
}
