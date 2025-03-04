#include <stdio.h>

/**
* My strlen function
**/
size_t StrLen(const char* str)
{
	size_t len = 0;
	
	assert(str != NULL);
	
	while (str[len] != '\0')
	{
		len++;
	}
	
	return len;
}

/**
* My strcmp function
**/
int StrCmp(const char* str1, const char* str2)
{
	int diff;
	
	assert(str1 != NULL);
	assert(str2 != NULL);	
	
	while ((*str1 != '\0') && (*str2 != '\0') && (*str1 == *str2))
	{
		str1++;
		str2++;
	}
	
	diff = *str1 - *str2;
	
	return (diff);
}

