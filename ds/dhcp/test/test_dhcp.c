/************************************includes************************************/
#include <stdio.h> 		/* printf */

#include "test_utils.h" /* colors, titles, status, boolean */
#include "dhcp.h"

/************************************Functions Forward Decleration************************************/
void TestCreateAndDestroy();
void TestAllocateIP();
void TestFreeIP();
void TestCountFree();

/************************************main************************************/
int main(void)
{
	TestCreateAndDestroy();
	TestAllocateIP();
	TestFreeIP();
	TestCountFree();
	
	printf("\n");
	
	return (0);
}

/************************************Functions************************************/
void TestCreateAndDestroy()
{
	dhcp_t* dhcp = DHCPCreate(0xC0A86700, 0xFFFFFF00); /* 192.168.103.0/24 */

	printf(BOLD_TITLE "\nTest: DHCPCreate & DHCPDestroy\n" RESET);

	if (NULL != dhcp)
	{
		printf(GREEN "TEST 1 PASSED\n" RESET);
	}
	else
	{
		printf(RED "TEST 1 FAILED: " RESET "Failed to create DHCP structure\n");
	}

	DHCPDestroy(dhcp);

	printf(GREEN "TEST 2 PASSED: Destroy completed (no crash)\n" RESET);
}

void TestAllocateIP()
{
	dhcp_t* dhcp = DHCPCreate(0xC0A86700, 0xFFFFFF00); /* 192.168.103.0/24 */
	unsigned long ip = 0;

	printf(BOLD_TITLE "\nTest: DHCPAllocateIp\n" RESET);

	ip = DHCPAllocateIp(dhcp, 0xC0A86742); /* Try allocate 192.168.103.66 */
	if (ip == 0xC0A86742)
	{
		printf(GREEN "TEST 1 PASSED\n" RESET);
	}
	else
	{
		printf(RED "TEST 1 FAILED: " RESET "Expected 192.168.103.66, got %lu\n", ip);
	}

	DHCPDestroy(dhcp);
}

void TestFreeIP()
{
	dhcp_t* dhcp = DHCPCreate(0xC0A86700, 0xFFFFFF00); /* 192.168.103.0/24 */
	unsigned long ip1 = 0, ip2 = 0;

	printf(BOLD_TITLE "\nTest: DHCPFreeIp\n" RESET);

	ip1 = DHCPAllocateIp(dhcp, 0xC0A86720); /* 192.168.103.32 */
	DHCPFreeIp(dhcp, ip1); /* free it */

	ip2 = DHCPAllocateIp(dhcp, 0xC0A86720); /* try again */

	if (ip2 == ip1)
	{
		printf(GREEN "TEST 1 PASSED\n" RESET);
	}
	else
	{
		printf(RED "TEST 1 FAILED: " RESET "Expected to reallocate same IP, got %lu\n", ip2);
	}

	DHCPDestroy(dhcp);
}

void TestCountFree()
{
	dhcp_t* dhcp = DHCPCreate(0xC0A86700, 0xFFFFFF00); /* 192.168.103.0/24 */
	size_t count = 0;

	printf(BOLD_TITLE "\nTest: DHCPCountFree\n" RESET);

	/* 256 total - 3 reserved = 253 */
	count = DHCPCountFree(dhcp);
	if (count == 253)
	{
		printf(GREEN "TEST 1 PASSED\n" RESET);
	}
	else
	{
		printf(RED "TEST 1 FAILED: " RESET "Expected 253 free, got %lu\n", count);
	}

	DHCPAllocateIp(dhcp, 0xC0A86710);
	count = DHCPCountFree(dhcp);

	if (count == 252)
	{
		printf(GREEN "TEST 2 PASSED\n" RESET);
	}
	else
	{
		printf(RED "TEST 2 FAILED: " RESET "Expected 252 free after allocation, got %lu\n", count);
	}

	DHCPDestroy(dhcp);
}
