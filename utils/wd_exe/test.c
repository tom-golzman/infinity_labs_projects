#include <stdio.h>

int main(int argc, const char* argv[])
{
	int pid = -1;
	
	printf("pid: %d\n", getpid());
	
	pid = fork();
	
	if (0 == pid)
	{
		execvp("./wd_launch.out", argv);
	}
	
	while(1)
	{}
	
	return 0;
}
