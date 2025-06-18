/************************************includes************************************/
#include <stdio.h> /* printf */

#include "test_utils.h" /* colors, titles, status, boolean */
#include "knight_tour.h"

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
	int x = 0, y = 0;
	int failed_count = 0;
	int result = FALSE;

	printf(BOLD_TITLE "\nTest: IsKnightTourPathFound() - Full 8x8 Board\n" RESET);

	for (x = 0; x < BOARD_SIZE; ++x)
	{
		for (y = 0; y < BOARD_SIZE; ++y)
		{
			result = IsKnightTourPathFound(x, y);

			if (result != TRUE)
			{
				printf(RED "FAILED at position (%d, %d)\n" RESET, x, y);
				++failed_count;
			}
		}
	}

	if (0 == failed_count)
	{
		printf(GREEN "ALL TESTS PASSED: Knight’s Tour is possible from every square.\n" RESET);
	}
	else
	{
		printf(RED "TEST FAILED: %d positions failed.\n" RESET, failed_count);
	}
}
