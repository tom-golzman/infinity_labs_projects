#include <stdio.h>
#include "swap.h" // Include the function prototype

void test_swap ()
{
	int a = 5, b = 10;
	
	printf("Before swap: a = %d, b = %d\n", a, b);

	swap (&a, &b);

	printf("After swap: a = %d, b = %d\n", a, b);

	if (a == 10 && b == 5)
	{
		printf("Test Passed!\n");
	}
	else
	{
		printf("Test failed!\n");
	}
}

int main ()
{
	test_swap();
	return 0;
}
