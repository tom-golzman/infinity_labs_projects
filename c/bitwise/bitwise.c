#include <stddef.h> /* For size_t */
#include <stdio.h> /* For printf */

#define TRUE 1
#define FALSE 0

long Pow2(unsigned int x, unsigned int y)
{
	long result = 0;
	
	result = (x << y);
	
	return (result);
}

int IsPow2Loop(unsigned int n)
{
	while (n > 1)
	{ 
		if ((n & 1) == 1)
		{
			return (FALSE);
		}
		n = n >> 1;
	}
	
	return (TRUE);
}

int IsPow2NoLoop(unsigned int n)
{
	if (0 == n)
	{
		return (FALSE);
	}
	
	if ((n & (n-1)) == 0)
	{
		return (TRUE);
	}
	
	return (FALSE);
}

int AddOne(int n)
{
	int carry = 1;

	while (n & carry)
	{
		n = n ^ carry;
		carry = carry << 1;
	}

	n = n ^ carry;

	return (n);	
}

void PrintWhereThreeBits(unsigned int* arr, size_t size)
{
	size_t i = 0;
	unsigned int tmp = 0;
	short count = 0;
	
	for (i = 0; i < size; i++)
	{
		count = 0;
		tmp = arr[i];
		
		while (tmp > 0)
		{
			if ((tmp & 1) == 1)
			{
				count++;	
			}
			tmp = tmp >> 1;
		}
		
		if (count == 3)
		{
			printf("%u ", arr[i]);
		}
	}
	
	printf("\n");
}

unsigned char ByteMirrorLoop(unsigned char n)
{
	unsigned int result = 0;
	short i = 0;
	
	for (i = 0; i < 8; i++)
	{
		result = result << 1;
		
		if ((n & 1) == 1)
		{
			result += 1;
		}
		n = n >> 1;
	}
	
	return (result);
}

unsigned char ByteMirrorNoLoop(unsigned char n)
{
	unsigned char close_bits_left = 0x55; /* 01010101 */
	unsigned char close_bits_right = 0xAA; /* 10101010 */
	unsigned char pair_bits_left = 0x33; /* 00110011 */
	unsigned char pair_bits_right = 0xCC; /* 11001100 */
	unsigned char nibble_left = 0x0F; /* 00001111 */
	unsigned char nibble_right = 0xF0; /* 11110000 */
	
	n = (((n & close_bits_left) << 1) | ((n & close_bits_right) >> 1));
	n = (((n & pair_bits_left) << 2) | ((n & pair_bits_right) >> 2));
	n = (((n & nibble_left) << 4) | ((n & nibble_right) >> 4));
	
	return (n);
}

int Is2n6BitsOn(unsigned char c)
{
	unsigned char second_bit_mask = 0x02;
	unsigned char sixth_bit_mask = 0x20;
	unsigned char second_bit_status = 0;
	unsigned char sixth_bit_status = 0;
		
	second_bit_status = (c & second_bit_mask);
	sixth_bit_status = (c & sixth_bit_mask);
	
	return ((second_bit_status >> 1) & (sixth_bit_status >> 5));
}

int IsAny2n6BitsOn(unsigned char c)
{
	unsigned char second_bit_mask = 0x02;
	unsigned char sixth_bit_mask = 0x20;
	unsigned char second_bit_status = 0;
	unsigned char sixth_bit_status = 0;
		
	second_bit_status = (c & second_bit_mask);
	sixth_bit_status = (c & sixth_bit_mask);
	
	return ((second_bit_status >> 1) | (sixth_bit_status >> 5));
}

unsigned char Swap3n5Bits(unsigned char c)
{
	unsigned char third_bit = (c >> 2) & 1;
	unsigned char fifth_bit = (c >> 4) & 1;

	c = c ^ ((third_bit ^ fifth_bit) << 2);
	c = c ^ ((third_bit ^ fifth_bit) << 4);

	return (c);
}

unsigned int ClosestDiv16(unsigned int n)
{
	return ((n >> 4) << 4);
}


void Swap(int* a, int* b)
{
	*a = *a ^ *b;
	*b = *a ^ *b;
	*a = *a ^ *b;
}

unsigned int CountSetBitsLoop(int n)
{
	size_t count = 0;
	int i = 0;
	
	for (i = 0; i < 32; i++)
	{
		if ((n & 1) == 1)
		{
			count++;
		}
		n = n >> 1;
	}
	
	return (count);
}

unsigned int CountSetBitsNoLoop(int n)
{
	int check_1_bites_mask  = 0x55555555;
	int check_2_bites_mask  = 0x33333333;
	int check_4_bites_mask  = 0x0F0F0F0F;
	int check_8_bites_mask  = 0x00FF00FF;
	int check_16_bites_mask = 0x0000FFFF;
	unsigned int to_add1, to_add2;
	
	to_add1 = (n & check_1_bites_mask);
	to_add2 = ((n >> 1) & check_1_bites_mask);
	n = to_add1 + to_add2;
	
	to_add1 = (n & check_2_bites_mask);
	to_add2 = ((n >> 2) & check_2_bites_mask);
	n = to_add1 + to_add2;
	
	to_add1 = (n & check_4_bites_mask);
	to_add2 = ((n >> 4) & check_4_bites_mask);
	n = to_add1 + to_add2;
	
	to_add1 = (n & check_8_bites_mask);
	to_add2 = ((n >> 8) & check_8_bites_mask);
	n = to_add1 + to_add2;
	
	to_add1 = (n & check_16_bites_mask);
	to_add2 = ((n >> 16) & check_16_bites_mask);
	n = to_add1 + to_add2;
	
	return (n);
}

static void PrintBits(int* num)
{
	int i = 31;
	int temp = *num;
	
	for (i = 31; i >= 0; i--)
	{
		printf("%d", ((temp >> i) & 1));
	}
	printf("\n");
}

void PrintFloatBits(float f)
{
	int num_as_int = 0;
	
	num_as_int = *(int*)(&f);
	
	printf("Print float in bits:\n");
	PrintBits(&num_as_int);
}







