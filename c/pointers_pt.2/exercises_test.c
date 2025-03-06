#include "exercises.h"

void TestIsPalindrome();
void TestSevenBoom();
void TestRmSpaces();

int main()
{
	TestIsPalindrome();
	TestSevenBoom();
	TestRmSpaces();

	return (0);
}

void TestIsPalindrome()
{
	char str[] = "asbfbsa";
	
	if(IsPalindrome(str) == 1)
	{
		printf("%s is a palindrome!\n", str);
	}
	else
	{
		printf("%s is NOT a palindrome!\n", str);
	}
}

void TestSevenBoom()
{
	SevenBoom(1,22);
}

void TestRmSpaces()
{
	char str[] = "    t   o   m   ";
	char new_str[20];
	
	char* result = RmSpaces(str, new_str);
	
	printf("\n\033[1mTest RmSpaces:\033[0m\n");
	printf("the new: %s\n", result);
}












