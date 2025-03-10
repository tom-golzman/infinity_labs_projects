#include "arr_ptr2ptr_ex.h"
#include <stdio.h> /* printf */

void TestSumRows();
void TestStayAlive();
void TestPrintDataTypes();
void TestPrintEnvVars();

int main()
{
	TestSumRows();
	TestStayAlive();
	TestPrintDataTypes();
	TestPrintEnvVars();
	
	return (0);
}

void TestSumRows()
{
	int row1[] = {1,1,1};
	int row2[] = {2,2,2};
	int row3[] = {3,3,3};
	const int* table[3];
	size_t rows = 3, columns = 3;
	long sums[3];
	size_t i = 0, j = 0;
	long expected_result[] = {3,6,9};
	
	printf("\033[1mTest SumRows:\033[0m\n");
	
	table[0] = row1;
	table[1] = row2;
	table[2] = row3;
	
	printf("Table:\n");
	
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < columns; j++)
		{
			printf("%d ", table[i][j]);
		}
		printf("\n");
	}
	
	SumRows(table, sums, rows, columns);
	
	for (i = 0; i < rows; i++)
	{
		printf("row %lu sum = %ld (expected: %ld)\n", i, sums[i], expected_result[i]);
	}
}

void TestStayAlive()
{
	printf("\n4: %lu\n", StayAlive(4));
	printf("\n5: %lu\n", StayAlive(5));
}

void TestPrintDataTypes()
{
	printf("\n\033[1mTest PrintDataTypes:\033[0m\n");
	
	PrintDataTypes();
}

void TestPrintEnvVars()
{
	printf("\n\033[1mTest PrintEnvVArs:\033[0m\n");
	
	PrintEnvVars();
}





