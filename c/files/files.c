/**
	Written by: Tom Golzman
	Date: 12/02/2025
	Reviewed by: Hagai Levi
**/

#include <stdio.h> /* For printf */

void PrintInt(int num);
typedef void (*print_t)(int);

typedef struct
{
	int value;
	print_t print;
} print_me;

int main()
{
	print_me printers[10];
	int i = 0;
	
	for (i = 0; i < 10; i++)
	{
		printers[i].value = i;
		printers[i].print = PrintInt;
	}
	
	for (i = 0; i < 10; i++)
	{
		printers[i].print(printers[i].value);
	}

	return 0;
}

void PrintInt(int num)
{
	printf("Value: %d\n", num);
}
