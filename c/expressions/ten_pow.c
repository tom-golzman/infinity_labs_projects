#include <stdio.h>

int ten_pow (int n)
{
	float result = 10;
	int i;

	if (n == 0)
		return 1;

	if (n < 0)
	{
		result = 1/10;
		n = -n;
	}

	for (i = 1; i < n; i++)
		result *= result;

	return result;
}

int main ()
{
	return 0;
}
