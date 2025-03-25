/**
	Written By: Tom Golzman
	Date: 23/02/2025
	Reviewed By: Yuval Almog
**/

/*----------------------includes----------------------*/
#include <stdint.h> /* For uintptr_t */
#include <stddef.h> /* For size_t */
#include <limits.h> /* For CHAR_BIT */
#include <stdio.h>

/*----------------------define----------------------*/
#define WORD_SIZE (sizeof(uintptr_t))
#define MODULO_8(num) (num & 7)

/*---------------------Functions Forward Decleration---------------------*/
void* Memset(void* str, int value, size_t num);
void* Memcpy(void* dest, const void* src, size_t num);
void* Memmove(void* dest, const void* src, size_t num);

/*-------------------------functions-------------------------*/
void* Memset(void* str, int value, size_t num)
{
	size_t i = 0;
	unsigned char* byte_str = NULL;
	uintptr_t* word_size_str = NULL;
	uintptr_t word_size_chunk = 0;
	
	byte_str = (unsigned char*) str;
	word_size_chunk = (unsigned char) value;
	
	for (i = 0; i < (WORD_SIZE - 1); i++)
	{
		word_size_chunk = word_size_chunk << CHAR_BIT;
		word_size_chunk = word_size_chunk | (unsigned char) value;
	}
	
	while (num > 0)
	{
		while (MODULO_8((size_t)byte_str) != 0)
		{
			*byte_str = (unsigned char) value;
			byte_str++;
			num--;
		}
		
		word_size_str = (uintptr_t*) byte_str;
		
		while (num >= WORD_SIZE)
		{
			*word_size_str = word_size_chunk;
			word_size_str++;
			num = num - WORD_SIZE;
		}
		
		byte_str = (unsigned char*) word_size_str;
				
		while (num > 0)
		{
			*byte_str = (unsigned char) value;
			byte_str++;
			num--;
		}
	}
	
	return (str);
}

void* Memcpy(void* dest, const void* src, size_t num)
{
	uintptr_t* word_size_dest = NULL;
	const uintptr_t* word_size_src = NULL;
	unsigned char* byte_dest = NULL;
	const unsigned char* byte_src = NULL;
	
	byte_dest = (unsigned char*) word_size_dest;
	byte_src = (const unsigned char*) word_size_src;
	
	while (MODULO_8((size_t)byte_src) != 0)
	{
		*byte_dest = *byte_src;
		byte_dest++;
		byte_src++;
		num--;
	}
	
	word_size_dest = (uintptr_t*) dest;
	word_size_src = (const uintptr_t*) src;
	
	while (num >= WORD_SIZE)
	{
		*word_size_dest = *word_size_src;
		word_size_dest++;
		word_size_src++;
		num = num - WORD_SIZE;
	}
	
	byte_dest = (unsigned char*) word_size_dest;
	byte_src = (const unsigned char*) word_size_src;
	
	while (num > 0)
	{
		*byte_dest = *byte_src;
		byte_dest++;
		byte_src++;
		num--;
	}
	
	return (dest);
}

void* Memmove(void* dest, const void* src, size_t num)
{
	uintptr_t* word_size_dest = NULL;
	const uintptr_t* word_size_src = NULL;
	unsigned char* byte_dest = NULL;
	const unsigned char* byte_src = NULL;
	
	word_size_dest = (uintptr_t*) dest;
	word_size_src = (const uintptr_t*) src;
	
	if (word_size_dest < word_size_src)
	{
		Memcpy(dest, src, num);
	}
	
	else
	{
		word_size_dest = (uintptr_t*)((unsigned char*)dest + num);
		word_size_src = (const uintptr_t*)((const unsigned char*)src + num);
		byte_dest = (unsigned char*) word_size_dest;
		byte_src = (const unsigned char*) word_size_src;
		
		while (MODULO_8(num) > 0)
		{
			--byte_dest;
			--byte_src;
			*byte_dest = *byte_src;
			
			num--;
		}
		
		word_size_dest = (uintptr_t*) byte_dest;
		word_size_src = (uintptr_t*) byte_src;
		
		while (num >= WORD_SIZE)
		{
			--word_size_dest;
			--word_size_src;
			*word_size_dest = *word_size_src;
			
			num = num - WORD_SIZE;
		}
	}
	
	return (dest);
}
