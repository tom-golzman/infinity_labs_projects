/**
	Written By: Tom Golzman
	Date: 25/02/2025
	Reviewed By: Amir Granot
**/

/*----------------------includes----------------------*/
#include <stddef.h> /* For NULL */
#include <limits.h> /* For INT_MIN */
#include <ctype.h> /* For isspace */
#include <stdio.h> /* For printf */
#include <assert.h> /* For assert */

/*---------------------Functions Forward Decleration---------------------*/
char* IToA10(int n, char* str);
int AToI10(char* str);
char* IToABase(int n, char* str, int base);
int AToIBase(char* str, int base);
int IsLittleEndian();
void PrintUniqueLetters(const char* arr1, size_t size1, const char* arr2, size_t size2 , const char* arr3, size_t size3);
static void FlipString(char* start, char* end);
static int IsInBase(char ch, int base);

/*-------------------------functions-------------------------*/
char* IToA10(int n, char* str)
{
	int i = 0;
	
	assert(str);
	
	if (n == 0)
	{
		str[i] = '0';
		i++;
		str[i] = '\0';
		
		return (str);	
	}
	
	if (n < 0)
	{
		while (n != 0)
		{
			str[i] = (-(n % 10)) + '0';
			n = n / 10;
			i++;
		}
		
		str[i] = '-';
		i++;
	}
	
	else
	{
		while (n != 0)
		{
			str[i] = (n % 10) + '0';
			n = n / 10;
			i++;
		}
	}
	
	str[i] = '\0';
	
	FlipString(str, str + (unsigned char)(i-1));
	
	return (str);
}

int AToI10(char* str)
{
	int i = 0;
	int sign = 1;
	int result = 0;
	
	assert(str);
	
	while (isspace(str[i]) != 0)
	{
		i++;
	}
	
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
	{
		i++;
	}
		
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	
	return (result * sign);
}

char* IToABase(int n, char* str, int base)
{
	int i = 0;
	int curr_digit = 0;
	
	assert(str);
	
	if (base < 2 || base > 36)
	{
		printf("ILLEGAL BASE!\n");
		
		return (str);
	}
	
	if (n == 0)
	{
		str[i] = '0';
		i++;
		str[i] = '\0';
		
		return (str);	
	}
	
	if (n < 0)
	{
		while (n != 0)
		{ 
			curr_digit = -(n % base);
			if (curr_digit > 9)
			{
				str[i] = curr_digit - 10 + 'a';
			}
			else
			{
				str[i] = curr_digit + '0';
			}
			
			i++;
			n = n / base;
		}
		
		str[i] = '-';
		i++;
	}

	else
	{
		while (n != 0)
		{	
			curr_digit = n % base;
			if (curr_digit > 9)
			{
				str[i] = curr_digit - 10 + 'a';
			}
			else
			{
				str[i] = curr_digit + '0';
			}
			
			i++;
			n = n / base;
		}
	}
	
	str[i] = '\0';
	
	FlipString(str, str + (unsigned char)(i-1));
	
	return (str);
}

int AToIBase(char* str, int base)
{
	int i = 0;
	int sign = 1;
	int result = 0;
	
	assert(str);
	
	if (base < 2 || base > 36)
	{
		printf("ILLEGAL BASE!\n");
		
		return (0);
	}
	
	while (isspace(str[i]) != 0)
	{
		i++;
	}
	
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
	{
		i++;
	}
	
	while (str[i] != '\0' && IsInBase(str[i], base) == 1)
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			result = result * base + (str[i] - '0');
			i++;
		}
		
		else
		{
			result = result * base + (str[i] - 'a' + 10);
			i++;
		}
	}
		
	return (result * sign);
}

void PrintUniqueLetters(const char* arr1, size_t size1, const char* arr2, size_t size2 , const char* arr3, size_t size3)
{
	unsigned char in1[256] = {0};
	unsigned char in2[256] = {0};
	unsigned char in3[256] = {0};
	size_t i = 0;
	int j = 0;
	
	assert(arr1);
	assert(arr2);
	assert(arr3);
	
	for (i = 0; i < size1; i++)
	{
		in1[(unsigned char)arr1[i]] = 1;
	}
	
	for (i = 0; i < size2; i++)
	{
		in2[(unsigned char)arr2[i]] = 1;
	}
	
	for (i = 0; i < size3; i++)
	{
		in3[(unsigned char)arr3[i]] = 1;
	}
	
	for (j = 0; j < 256; j++)
	{
		if (1 == in1[j] && 1 == in2[j] && 0 == in3[j])
		{
			printf("%c ", j);
		}
	}
	
	printf("\n");
	
}

int IsLittleEndian()
{
	int num = 1;
	char* byte = NULL;
	
	byte = (char*)&num;
	
	if (1 == byte[0])
	{
		return (0);
	}
	
	return (1);
}

static int IsInBase(char ch, int base)
{
	if (ch >= '0' && ch <= '9' && ch - '0' < base)
	{
		return (1);
	}
	
	else if (ch - 'a' + 10 <= base)
	{
		return (1);
	}
	
	return (0);
}

static void FlipString(char* start, char* end)
{
	char temp = 0;
	
	assert(start);
	assert(end);
	
	while (start < end)
	{
		temp = *start;
		*start = *end;
		*end = temp;
		start++;
		end--;
	}
}

