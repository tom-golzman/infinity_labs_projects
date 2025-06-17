/************************************includes************************************/
#include <stdio.h> /* printf */

#include "test_utils.h" /* colors, titles, status, boolean */
#include "knight_tour.h"

/************************************define************************************/

/************************************Functions Forward Decleration************************************/
void TestKnightTour();

/************************************main************************************/
int main(void)
{
	TestKnightTour();
		
	printf("\n");
	
	return (0);
}

/************************************Functions************************************/
void TestKnightTour()
{
	int result = 0;

	printf(BOLD_TITLE "\nTest: IsKnightTourPathFound()\n" RESET);

	/* TEST 1: left up edge (0,0) */
	result = IsKnightTourPathFound(0, 0);
	if (TRUE == result)
	{
		printf(GREEN "TEST 1 PASSED\n" RESET);
	}
	else
	{
		printf(RED "TEST 1 FAILED: " RESET "Expected TRUE at (0,0)\n");
	}

	/* TEST 2: board center (3,3) */
	result = IsKnightTourPathFound(3, 3);
	if (TRUE == result)
	{
		printf(GREEN "TEST 2 PASSED\n" RESET);
	}
	else
	{
		printf(RED "TEST 2 FAILED: " RESET "Expected TRUE at (3,3)\n");
	}

	/* TEST 3: right down edge (7,7) */
	result = IsKnightTourPathFound(7, 7);
	if (TRUE == result)
	{
		printf(GREEN "TEST 3 PASSED\n" RESET);
	}
	else
	{
		printf(RED "TEST 3 FAILED: " RESET "Expected TRUE at (7,7)\n");
	}
}
