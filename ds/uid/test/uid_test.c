/************************************includes************************************/
#include <stdio.h> /* printf */

#include "uid.h"

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define TITLE "\033[1;4m"

/************************************Functions Forward Decleration************************************/
void TestUIDCreate();
void TestUIDIsSame();

/************************************main************************************/
int main(void)
{
	TestUIDCreate();
	TestUIDIsSame();	
	
	printf("\n");
	
	return (0);
}

/************************************Functions************************************/
void TestUIDCreate()
{
	ilrd_uid_t uid1 = UIDCreate();
	ilrd_uid_t uid2 = UIDCreate();
	
    printf(TITLE "Test: UIDCreate()\n" RESET);

	printf("id1: ip = %u, pid = %u, id = %lu, time = %lu\n", uid1.ip, uid1.pid, uid1.id, uid1.time);
	printf("id2: ip = %u, pid = %u, id = %lu, time = %lu\n", uid2.ip, uid2.pid, uid2.id, uid2.time);		
}

void TestUIDIsSame()
{
	ilrd_uid_t uid1 = UIDCreate();
	ilrd_uid_t uid2 = UIDCreate();
	ilrd_uid_t uid3 = uid1;
			
    printf(TITLE "\nTest: UIDCreate()\n" RESET);

	if (!UIDIsSame(uid1, uid2))
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
	}

	if (UIDIsSame(uid1, uid3))
	{
		printf(GREEN "Test 2 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 2 Failed!\n" RESET);
	}
}
