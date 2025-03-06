#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
/** Hagai reviewed **/

/** My strlen function **/
size_t StrLen(const char* str)
{
	size_t len = 0;
	
	assert(str != NULL);
	
	while (str[len] != '\0')
	{
		len++;
	}
	
	return (len);
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
	char* destIndex = dest;
	
	assert(dest != NULL);
	assert(src != NULL);
	
	while(*src != '\0')
	{
		*destIndex = *src;
		destIndex++;
		src++;
	}
	
	*destIndex = '\0';
	
	return (dest);
}

/** My strncpy function **/
char* StrNCpy(char* dest, const char* src, size_t n)
{
	size_t i;
	char* dest_index = dest;
	
	assert(dest != NULL);
	assert(src != NULL);	
	
	/* Copy from src to dest */
	for(i = 0; src[i] != '\0' && i < n; i++)
	{
		dest_index[i] = src[i];
	}
	
	while(i < n)
	{
		dest_index[i] = '\0';
		i++;
	}
	
	return (dest);
}

/** My strncmp function **/
int StrNCmp(const char* str1, const char* str2, size_t n)
{
	int diff;
	
	assert(str1 != NULL);
	assert(str2 != NULL);	
	
	while ((n != 0) && (*str1 != '\0') && (*str2 != '\0') && (*str1 == *str2))
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
	
	while((*str1 != '\0') && (*str2 != '\0'))
	{
		char c1 = tolower(*str1);
		char c2 = tolower(*str2);
		
		if(c1 != c2)
		{
			return (c1 - c2);
		}
		
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
	char* dup = (char*)malloc(len);
	
	assert(str != NULL);
	assert(dup != NULL);
	
	memcpy(dup, str, len);
	
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
	const char* haystack_ptr = haystack;
	
	assert(haystack != NULL);
	assert(needle != NULL);
	
	while(*haystack_ptr != '\0')
	{
		if(*haystack_ptr == needle[0])
		{
			if(StrNCmp(haystack_ptr, needle, needle_size) == 0)
			{
				return (char*)(haystack_ptr);
			}
		}
		haystack_ptr++;
	}
	
	if(*haystack_ptr == '\0')
	{
		return (char*)(haystack);
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
