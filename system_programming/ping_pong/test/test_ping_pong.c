/************************************includes************************************/
#include <stdio.h> /* printf */

#include "test_utils.h" /* colors, titles, status, boolean */
#include "ping_pong.h"

/************************************Functions Forward Decleration************************************/
void TestPingPong1();

/************************************main************************************/
int main(void)
{
	TestPingPong1();
	
	printf("\n");
	
	return (0);
}

/************************************Functions************************************/
void TestPingPong1()
{
	printf(BOLD_TITLE "PingPong1() TEST: \n" RESET);
	
	PingPong1();	
}
