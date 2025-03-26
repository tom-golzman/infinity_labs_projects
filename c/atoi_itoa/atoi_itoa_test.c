/*----------------------includes----------------------*/
#include <stdio.h> /* For printf */
#include <string.h> /* For strcmp */
#include <limits.h> /* For INT_MAX */
#include <stdlib.h> /* For built-in atoi */
#include "atoi_itoa.h"

/*---------------------Functions Forward Decleration---------------------*/
void TestIToA10();
void TestAToI10();
void TestIToABase();
void TestAToIBase();
void TestIsLittleEndian();
void TestPrintUniqueLetters();

/*---------------------define---------------------*/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"

/*---------------------main---------------------*/
int main()
{
	TestIToA10();
	TestAToI10();
	TestIToABase();
	TestAToIBase();
	TestIsLittleEndian();
	TestPrintUniqueLetters();

	return 0;
}

/*-------------------------functions-------------------------*/
void TestIToA10()
{
	char str[50];

	printf("\033[1mTest IToA10:\033[0m\n");

	if (strcmp(IToA10(1234, str), "1234") == 0)
	{
		printf(GREEN "Test 1 passed: 1234 → \"%s\"\n" RESET, str);
	}
	else
	{
		printf(RED "Test 1 FAILED: 1234 → \"%s\"\n" RESET, str);
	}
	
	if (strcmp(IToA10(-5678, str), "-5678") == 0)
	{
		printf(GREEN "Test 2 passed: -5678 → \"%s\"\n" RESET, str);	
	}
	else
	{
		printf(RED "Test 2 FAILED: -5678 → \"%s\"\n" RESET, str);
	}
	
	if (strcmp(IToA10(0, str), "0") == 0)
	{
		printf(GREEN "Test 3 passed: 0 → \"%s\"\n" RESET, str);
	}
	else
	{
		printf(RED "Test 3 FAILED: 0 → \"%s\"\n" RESET, str);
	}
	
	if (strcmp(IToA10(INT_MAX, str), "2147483647") == 0)
	{
		printf(GREEN "Test 4 passed: INT_MAX → \"%s\"\n" RESET, str);
	}
	else
	{
		printf(RED "Test 3 FAILED: INT_MAX → \"%s\"\n" RESET, str);
	}
	
	if (strcmp(IToA10(INT_MIN, str), "-2147483648") == 0)
	{
		printf(GREEN "Test 5 passed: INT_MIN → \"%s\"\n" RESET, str);
	}
	else
	{
		printf(RED "Test 5 FAILED: INT_MIN → \"%s\"\n" RESET, str);
	}
	
	if (strcmp(IToA10(-1, str), "-1") == 0)
	{
		printf(GREEN "Test 6 passed: -1 → \"%s\"\n" RESET, str);
	}
	else
	{
		printf(RED "Test 6 FAILED: -1 → \"%s\"\n" RESET, str);
	}

	if (strcmp(IToA10(99999999, str), "99999999") == 0)
	{
		printf(GREEN "Test 7 passed: 99999999 → \"%s\"\n" RESET, str);
	}
	else
	{
		printf(RED "Test 7 FAILED: 99999999 → \"%s\"\n" RESET, str);
	}
}

void TestAToI10()
{
	int num;
	
	printf("\n\033[1mTest AToI10:\033[0m\n");

	num = AToI10("1234");
	if (num == atoi("1234"))
	{
		printf(GREEN "Test 1 passed: \"1234\" → \"%d\"\n" RESET, num);
	}
	else
	{
		printf(RED "Test 1 failed: \"1234\" → \"%d\"\n" RESET, num);
	}
	
	num = AToI10("+234");
	if (num == atoi("+234"))
	{
		printf(GREEN "Test 2 passed: \"+234\" → \"%d\"\n" RESET, num);
	}
	else
	{
		printf(RED "Test 2 failed: \"+234\" → \"%d\"\n" RESET, num);
	}
	
	num = AToI10("  5678");
	if (num == atoi("  5678"))
	{
		printf(GREEN "Test 3 passed: \"  5678\" → \"%d\"\n" RESET, num);
	}
	else
	{
		printf(RED "Test 3 failed: \"  5678\" → \"%d\"\n" RESET, num);
	}
	
	num = AToI10("  -100blabla");
	if (num == atoi("  -100blabla"))
	{
		printf(GREEN "Test 4 passed: \"  -100blabla\" → \"%d\"\n" RESET, num);
	}
	else
	{
		printf(RED "Test 4 failed: \"  -100blabla\" → \"%d\"\n" RESET, num);
	}
	
	num = AToI10("abc1234");
	if (num == atoi("abc1234"))
	{
		printf(GREEN "Test 5 passed: \"abc1234\" → \"%d\"\n" RESET, num);
	}
	else
	{
		printf(RED "Test 5 failed: \"abc1234\" → \"%d\"\n" RESET, num);
	}
	
	num = AToI10("-2147483648");
	if (num == atoi("-2147483648"))
	{
		printf(GREEN "Test 6 passed: \"-2147483648\" → \"%d\"\n" RESET, num);
	}
	else
	{
		printf(RED "Test 6 failed: \"-2147483648\" → \"%d\"\n" RESET, num);
	}
	
	num = AToI10("2147483647");
	if (num == atoi("2147483647"))
	{
		printf(GREEN "Test 7 passed: \"2147483647\" → \"%d\"\n" RESET, num);
	}
	else
	{
		printf(RED "Test 7 failed: \"2147483647\" → \"%d\"\n" RESET, num);
	}
	
	num = AToI10("--5678");
	if (num == atoi("--5678"))
	{
		printf(GREEN "Test 8 passed: \"--5678\" → \"%d\"\n" RESET, num);
	}
	else
	{
		printf(RED "Test 8 failed: \"--5678\" → \"%d\"\n" RESET, num);
	}
}

void TestIToABase()
{
	char buff[50];
	
	printf("\n\033[1mTest IToABase:\033[0m\n");

	IToABase(15, buff, 2);
	if (strcmp(buff, "1111") == 0)
	{
		printf(GREEN "Test 1 PASSED: 15 in base 2 → \"%s\"\n" RESET, buff);
	}
	else
	{
		printf(RED "Test 1 FAILED: 15 in base 2 → \"%s\"\n" RESET, buff);
	}
	
	IToABase(255, buff, 16);
	if (strcmp(buff, "ff") == 0)
	{
		printf(GREEN "Test 2 PASSED: 255 in base 16 → \"%s\"\n" RESET, buff);
	}
	else
	{
		printf(RED "Test 2 FAILED: 255 in base 16 → \"%s\"\n" RESET, buff);
	}
	
	IToABase(64, buff, 8);
	if (strcmp(buff, "100") == 0)
	{
		printf(GREEN "Test 3 PASSED: 64 in base 8 → \"%s\"\n" RESET, buff);
	}
	else
	{
		printf(RED "Test 3 FAILED: 64 in base 8 → \"%s\"\n" RESET, buff);
	}
	
	IToABase(-5, buff, 2);
	if (strcmp(buff, "-101") == 0)
	{
		printf(GREEN "Test 4 PASSED: -5 in base 2 → \"%s\"\n" RESET, buff);
	}
	else
	{
		printf(RED "Test 4 FAILED: -5 in base 2 → \"%s\"\n" RESET, buff);
	}
}

void TestAToIBase()
{
	int num;
	
	printf("\n\033[1mTest AToIBase:\033[0m\n");

	num = AToIBase("1234", 10);
	if (num == 1234)
	{
		printf(GREEN "Test 1 passed: str = \"1234\" | base = 10 → \"%d\"\n" RESET, num);
	}
	else
	{
		printf(RED "Test 1 failed: str = \"1234\" | base = 10 → \"%d\"\n" RESET, num);
	}
	
	num = AToIBase("-234", 10);
	if (num == -234)
	{
		printf(GREEN "Test 2 passed: str = \"-234\" | base = 10 → \"%d\"\n" RESET, num);
	}
	else
	{
		printf(RED "Test 2 failed: str = \"-234\" | base = 10 → \"%d\"\n" RESET, num);
	}
	
	num = AToIBase("  1101", 2);
	if (num == 13)
	{
		printf(GREEN "Test 3 passed: str = \"  1101\" | base = 2 → \"%d\"\n" RESET, num);
	}
	else
	{
		printf(RED "Test 3 failed: str = \"  1101\" | base = 2 → \"%d\"\n" RESET, num);
	}
	
	num = AToIBase("1a", 16);
	if (num == 26)
	{
		printf(GREEN "Test 4 passed: str = \"1a\" | base = 16 → \"%d\"\n" RESET, num);
	}
	else
	{
		printf(RED "Test 4 failed: str = \"1a\" | base = 16 → \"%d\"\n" RESET, num);
	}
	
	num = AToIBase("12za34", 11);
	if (num == 13)
	{
		printf(GREEN "Test 5 passed: str = \"12za34\" | base = 11 → \"%d\"\n" RESET, num);
	}
	else
	{
		printf(RED "Test 5 failed: str = \"12za34\" | base = 1 → \"%d\"\n" RESET, num);
	}
	
	num = AToIBase("-2147483648", 10);
	if (num == -2147483648)
	{
		printf(GREEN "Test 6 passed: str = \"-2147483648\" | base = 10 → \"%d\"\n" RESET, num);
	}
	else
	{
		printf(RED "Test 6 failed: str = \"-2147483648\" | base = 10 → \"%d\"\n" RESET, num);
	}
}

void TestIsLittleEndian()
{
	printf("\n\033[1mTest AToIBase:\033[0m\n");
	
	if (IsLittleEndian() == 0)
	{
		printf(GREEN "System is LITTLE Endian!\n" RESET);
	}
	
	else 
	{
		printf(RED "System is BIG Endian!\n" RESET);
	}
}

void TestPrintUniqueLetters()
{
	const char a[] = { 'a', 'b', 'c', 'd'};
	const char b[] = { 'h', 'a', 'b', 'c' };
	const char c[] = { 'a', 'x', 'y' };

	printf("\n\033[1mTest PrintUniqueLetters:\033[0m\n");

	printf("a = {'a', 'b', 'c', 'd'} | b = {'h', 'a', 'b', 'c'} | c = {'a', 'x', 'y'}\n"); 
	printf("Expected output → b c\n");
	
	PrintUniqueLetters(a, sizeof(a), b, sizeof(b), c, sizeof(c));
	


}
