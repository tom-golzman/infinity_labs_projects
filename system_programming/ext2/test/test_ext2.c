/************************************ Includes *************************************/
#include <stdio.h> /* printf */

#include "test_utils.h" /* colors, titles, status, boolean */
#include "ext2.h"

/************************************* Defines *************************************/

/************************** Functions Forward Decleration **************************/

/************************************** main ***************************************/
int main(void)
{
	int status = FAIL;
	const char* device_path = "/dev/ram0";
	const char* file_name = "a.txt";

	status = EXT2PrintSuperblock(device_path);
	
	if (SUCCESS == status)
	{
		printf(GREEN "Test print super block PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test print super block FAILED!\n" RESET);
	}

	printf("\n");

	status = EXT2PrintGroupDesc(device_path);
	
	if (SUCCESS == status)
	{
		printf(GREEN "Test print group descriptor PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test print group descriptor FAILED!\n" RESET);
	}

	printf("\n");
	
	status = Ex2PrintFileContentInRoot(device_path, file_name);
	
	if (SUCCESS == status)
	{
		printf(GREEN "Test print file \"a.txt\" content in root PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test print file content in root FAILED!\n" RESET);
	}

	printf("\n");

	status = Ex2PrintFileContentInRoot(device_path, "b.txt");
	
	if (SUCCESS == status)
	{
		printf(GREEN "Test print file \"b.txt\" content in root PASSED!\n" RESET);
	}
	else
	{
		printf(RED "Test print file content in root FAILED!\n" RESET);
	}

	printf("\n");

	return SUCCESS;
}

/************************************ Functions ************************************/


