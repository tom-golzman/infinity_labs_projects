/**
	Written By: Tom Golzman
	Date: 02/04/2025
	Reviewed By: Sami 
**/

/************************************includes************************************/
#include <limits.h> /* CHAR_BIT */
#include <stddef.h> /* size_t */
#include <stdio.h> /* printf */
#include <assert.h> /* assert */

/************************************define************************************/
#define WORD_SIZE_BITS (sizeof(size_t)*CHAR_BIT)
#define STRING_BUFFER_SIZE (sizeof(size_t)*CHAR_BIT + 1)
#define ASCII_ZERO (int)'0'

/************************************typedef************************************/
typedef enum {
    ON  = 1,
    OFF = 0
} state_t;

typedef size_t bit_array_t;

/************************************Functions************************************/
bit_array_t BitArrInit(state_t val) 
{
	if (OFF == val)
	{
		return ((size_t)0);
	}
	
	return (~(size_t)0);
}

bit_array_t BitArrSetVal(bit_array_t arr, size_t index, state_t val)
{
	size_t mask;

	assert(index < WORD_SIZE_BITS);
	
	mask = (size_t)1 << index;
	
	if (val == ON)
	{
		arr = arr | mask;
	}
	
	else
	{
		arr = arr & (~mask);
	}
	
	return (arr);
}

state_t BitArrGetVal(bit_array_t arr, size_t index)
{
	assert(index < WORD_SIZE_BITS);

	arr = (arr >> index) & (size_t)1;
	
	if (arr == 1)
	{
		return (ON);
	}
	
	return (OFF);
}

size_t BitArrCountOn(bit_array_t arr)
{
	size_t count = 0;
	size_t i = 0;
	
	for (i = 0; i < WORD_SIZE_BITS; i++)
	{
		if ((arr & 1) == 1)
		{
			count++;
		}
		arr = arr >> 1;
	}
	
	return (count);
}

size_t BitArrCountOff(bit_array_t arr)
{
	size_t count = 0;
	size_t i = 0;
	
	for (i = 0; i < WORD_SIZE_BITS; i++)
	{
		if ((arr & 1) == 0)
		{
			count++;
		}
		arr = arr >> 1;
	}
	
	return (count);
}

bit_array_t BitArrSetOn(bit_array_t arr, size_t index)
{
	size_t mask;
	
	assert(index < WORD_SIZE_BITS);
	
	mask = (size_t)1 << index;
		
	arr = arr | mask;

	return (arr);
}

bit_array_t BitArrSetOff(bit_array_t arr, size_t index)
{
	size_t mask;
	
	assert(index < WORD_SIZE_BITS);
	
	mask = (size_t)1 << index;	
	
	arr = arr & (~mask);

	return (arr);
}

bit_array_t BitArrToggle(bit_array_t arr, size_t index)
{
	size_t mask;
	size_t curr_bit;
	
	assert(index < WORD_SIZE_BITS);
	
	mask = (size_t)1 << index;	
	curr_bit = (arr >> index) & (size_t)1;
	
	if (OFF == curr_bit)
	{
		arr = arr | mask;
	}
	
	else
	{
		arr = arr & (~mask);
	}
	
	return (arr);
}

bit_array_t BitArrSetAllOn(bit_array_t arr)
{
	size_t mask = ~((size_t)0);
	
	arr = arr | mask;
	
	return (arr);
}

bit_array_t BitArrSetAllOff(bit_array_t arr)
{
	size_t mask = (size_t)0;
	
	arr = arr & mask;
	
	return (arr);
}

bit_array_t BitArrMirror(bit_array_t arr)
{
	size_t result = 0;
	size_t i = 0;
	
	for (i = 0; i < WORD_SIZE_BITS; i++)
	{
		result = result << 1;
		
		if ((arr & 1) == 1)
		{
			result += 1;
		}
		arr = arr >> 1;
	}
	
	return (result);
}

bit_array_t BitArrRotateLeft(bit_array_t arr, unsigned int amount)
{
	bit_array_t new_arr = arr;
	
	amount = amount % WORD_SIZE_BITS;
	
	new_arr = (arr << amount) | (arr >> (WORD_SIZE_BITS - amount));
	
	return (new_arr);
}

bit_array_t BitArrRotateRight(bit_array_t arr, unsigned int amount)
{
	bit_array_t new_arr = arr;
	
	amount = amount % WORD_SIZE_BITS;
	
	new_arr = (arr >> amount) | (arr << (WORD_SIZE_BITS - amount));
	
	return (new_arr);
}

char* BitArrToString(bit_array_t arr, char buffer[STRING_BUFFER_SIZE])
{
	size_t i = WORD_SIZE_BITS;
	size_t mask = (size_t)1;
	
	assert(NULL != buffer);
	
	for (i = WORD_SIZE_BITS; i > 0; i--)
	{
		buffer[i - 1] = (char)(arr & mask) + ASCII_ZERO;
		arr >>= 1;
	}
	
	buffer[WORD_SIZE_BITS] = '\0';
	
	return (buffer);
}





