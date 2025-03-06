#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

/** 
** Checks if a string is a palindrome
**/

int IsPalindrome(const char* str)
{
	size_t str_size = strlen(str);
	char* end = (char*)str + str_size - 1; /* -1 to skip '\0' */
	
	while(str < end)
	{
		if(*str != *end)
		{
			return (0);
		}
		
		str++;
		end--;
	}
	
	return (1);
}

/**
** A function that recives a range (from, to) and prints all numbers in the range,
** except for numbers that contain the digit 7 or are divisible by 7 with no remainder,
** and instead of those numbers, it will print 'BOOM'.
**/

void SevenBoom(long from, long to)
{	
	long curr_num = from, tmp = 0;
	int is_seven = 0;
	
	for(curr_num = from; curr_num <= to; curr_num++)
	{
		tmp = curr_num;
		is_seven = 0;
		
		while(tmp > 0)
		{
			if(tmp % 10 == 7)
			{
				is_seven = 1;
				break;
			}
			tmp /= 10;
		}
		
		if(curr_num % 7 == 0 || is_seven == 1)
		{
			printf("BOOM ");
		}
		else
		{
			printf("%ld ", curr_num);
		}
	}
	printf("\n");
}

/**
** A function that receives a string and deletes white spaces from the beginning and end of the string,
** and deletes white spaces within the string. Returns new string
**/

char* RmSpaces(const char* src, char* new_str)
{
	int new_str_index = 0;
	int in_space_seq = 0; /* Flag that indicates if we are in a space sequence */
	
	assert(src != NULL);
	assert(new_str != NULL);
	
	/* Skip spaces in the beginning */
	while(*src != '\0' && isspace((char)*src))
	{
		src++;
	}
	
	while(*src != '\0')
	{
		if(isspace(*src) != 0)
		{
			if(in_space_seq != 0)
			{
				new_str[new_str_index] = ' ';
				new_str_index++;
				in_space_seq = 1;
			}
		}
		else
		{
			new_str[new_str_index] = *src;
			new_str_index++;
			in_space_seq = 0;
		}
		src++;
	}
	
	/* Remove spaces in the end */
	if(new_str_index > 0 && new_str[new_str_index - 1] == ' ')
	{
		new_str_index--;
	}
	
	new_str[new_str_index] = '\0';
	
	return (new_str);
}



































