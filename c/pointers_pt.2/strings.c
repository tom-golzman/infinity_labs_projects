#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/** Hagai reviewed **/

/** My strlen function **/
size_t StrLen(const char* str)
{
	const char* runner_ptr = NULL;
	
	assert(str != NULL);
	
	runner_ptr = str;
	
	while (*runner_ptr != '\0')
	{
		runner_ptr++;
	}
	
	return ((size_t)(runner_ptr - str));
}

/** My strcmp function **/
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

/** My strncpy function **/
char* StrCpy(char* dest, const char* src)
{
	char* dest_index = dest;
	
	assert(dest != NULL);
	assert(src != NULL);
	
	do {
		*dest_index = *src;
		dest_index++;
		src++;
	} while (*src != '\0');
	
	*dest_index = '\0';
	
	return (dest);
}

/** My strncpy function **/
char* StrNCpy(char* dest, const char* src, size_t n)
{
	char* dest_runner = dest;
	
	assert(dest != NULL);
	assert(src != NULL);	
	
	/* Copy from src to dest */
	while (*src != '\0' && n > 0)
	{
		*dest_runner = *src;
		dest_runner++;
		src++;
		n--;
	}
	
	while(n > 0)
	{
		*dest_runner = '\0';
		dest_runner++;
		n--;
	}
	
	return (dest);
}

/** My strncmp function **/
int StrNCmp(const char* str1, const char* str2, size_t n)
{
	int diff;
	
	assert(str1 != NULL);
	assert(str2 != NULL);	
	
	while ((n > 0) && (*str1 != '\0') && (*str2 != '\0') && (*str1 == *str2))
	{
		str1++;
		str2++;
		n--;
	}
	
	diff = *str1 - *str2;
	
	return (diff);
}

/** My strcasecmp function **/
int StrCaseCmp(const char* str1, const char* str2)
{
	int diff = 0;
	
	assert(str1 != NULL);
	assert(str2 != NULL);	
	
	while((*str1 != '\0') && (*str2 != '\0') && (tolower(*str1) == tolower(*str2)))
	{
		str1++;
		str2++;
	}
	
	diff = *str1 - *str2;
	
	return (diff);
}

/** My strchr function **/
char* StrChr(const char* str, int c)
{
	assert(str != NULL);	
	
	while(*str != '\0')
	{
		if(*str == (char)c)
		{
			return (char*)(str);
		}
		
		str++;
	}
	
	/* Checks if c is '\0' (in ASCII '\0' = 0) */
	if (c == 0)
	{
		return (char*)(str);
	}
	
	return (char*)(NULL);
}

/** My strdup function **/
char* StrDup(const char* str)
{
	size_t len = StrLen(str) + 1; /* +1 for '\0' */
	char* dup = (char*)malloc(len * sizeof(char)); 
	
	assert(str != NULL);
	assert(dup != NULL);
	
	StrNCpy(dup, str, len);
	
	return (dup);
}

/** My strcat function **/
char* StrCat(char* dest, const char* src)
{
	size_t dest_len = StrLen(dest);
	size_t src_len = StrLen(src);
	char* dest_end = dest + dest_len;
	
	assert(dest != NULL);
	assert(src != NULL);

	StrNCpy(dest_end, src, src_len);
	
	return (dest);
}

/** My strncat function **/
char* StrNCat(char* dest, const char* src, size_t n)
{
	char* dest_end = StrChr(dest, 0);
	
	assert(dest != NULL);
	assert(src != NULL);
	
	StrNCpy(dest_end, src, n);
	
	return (dest);
}

/** My strstr function **/
char* StrStr(const char* haystack, const char* needle)
{
	size_t needle_size = StrLen(needle);
	
	assert(haystack != NULL);
	assert(needle != NULL);
	
	if (needle_size == 0)
	{
		return (char*)(haystack);
	}
	
	while(StrChr(haystack, *needle) != NULL)
	{
		if(StrNCmp(haystack, needle, needle_size) == 0)
		{
			return (char*)(haystack);
		}
		
		haystack++;
	}
	
	return ((char*)NULL);
	
}

/** My strspn function **/
size_t StrSpn(const char* str1, const char* str2)
{
	size_t count = 0;
	size_t ind;
	
	assert(str1 != NULL);
	assert(str2 != NULL);
	
	while(str1[count] != '\0')
	{
		ind = 0;
		while(str2[ind] != '\0' && str1[count] != str2[ind])
		{
			ind++;
		}
		
		if(str2[ind] == '\0')
		{
			return (count);
		}
		
		count++;
	}
	
	return (count);
}

/** My strtok function **/
char* StrTok(char* str, const char* delim)
{
	static char* next = NULL;
	
	if(str == NULL)
	{
		str = next;
	}
	
	/* Skip initial delimiters */
	next = StrChr(str, *delim);
	
	if(next == NULL)
	{
		return (str);
	}
	
	/* If a delimiter was found, replace it with '\0' to terminate the token */
	if(*next != '\0')
	{
		*next = '\0';
		next++;
	}
	else
	{
		next = NULL;
	}
	
	return (str);
}
	
	














