#include "bitwise.h"
#include <stdio.h> /* For printf */
#include <stddef.h> /* For size_t */

void TestPow2();
void TestIsPow2Loop();
void TestIsPow2NoLoop();
void TestAddOne();
void TestPrintWhereThreeBits();
void TestByteMirrorLoop();
void TestByteMirrorNoLoop();
void TestIs2n6BitsOn();
void TestIsAny2n6BitsOn();
void TestSwap3n5Bits();
void TestClosestDiv16();
void TestSwap();
void TestCountSetBitsLoop();
void TestCountSetBitsNoLoop();
void TestPrintFloatBits();

int main()
{
	TestPow2();
	TestIsPow2Loop();
	TestIsPow2NoLoop();
	TestAddOne();
	TestPrintWhereThreeBits();
	TestByteMirrorLoop();
	TestByteMirrorNoLoop();
	TestIs2n6BitsOn();
	TestIsAny2n6BitsOn();
	TestSwap3n5Bits();
	TestClosestDiv16();
	TestSwap(); 
	TestCountSetBitsLoop();
	TestCountSetBitsNoLoop();
	TestPrintFloatBits();   
	
	return (0);
}

void TestPow2()
{
	unsigned int x = 2;
	unsigned int y = 3;
	long expected = 16;
	long result = 0;
	
	result = Pow2(x, y);
	
	printf("\033[1mTest Pow2:\033[0m\n");
	printf("x = %u | y = %u | (expected: %ld)\n", x, y, expected);
	printf("Result = %ld\n", result);
	if (result == expected)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
}

void TestIsPow2Loop()
{
	unsigned int n1 = 8;
	int expected1 = 1;
	int result1 = IsPow2Loop(n1);
	unsigned int n2 = 7;
	int expected2 = 0;
	int result2 = IsPow2Loop(n2);
	
	printf("\n\033[1mTest IsPow2Loop:\033[0m\n");
	printf("n1 = %u | (expected1: %d)\n", n1, expected1);
	printf("Result1 = %d\n", result1);
	if (result1 == expected1)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
	
	printf("n2 = %u | (expected2: %d)\n", n2, expected2);
	printf("Result2 = %d\n", result2);
	if (result2 == expected2)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
}

void TestIsPow2NoLoop()
{
	unsigned int n1 = 8;
	int expected1 = 1;
	int result1 = IsPow2NoLoop(n1);
	unsigned int n2 = 7;
	int expected2 = 0;
	int result2 = IsPow2NoLoop(n2);
	
	printf("\n\033[1mTest IsPow2NoLoop:\033[0m\n");
	printf("n1 = %u | (expected1: %d)\n", n1, expected1);
	printf("Result1 = %d\n", result1);
	if (result1 == expected1)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
	
	printf("n2 = %u | (expected2: %d)\n", n2, expected2);
	printf("Result2 = %d\n", result2);
	if (result2 == expected2)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
}


void TestAddOne()
{
	int num1 = 5;
	int expected1 = 6;
	int result1 = AddOne(num1);
	int num2 = -1;
	int expected2 = 0;
	int result2 = AddOne(num2);
	
	printf("\n\033[1mTest AddOne:\033[0m\n");
	printf("num = %d | (expected: %d)\n", num1, expected1);
	printf("Result = %d\n", result1);
	if (result1 == expected1)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
	
	
	
	printf("\n\033[1mTest AddOne:\033[0m\n");
	printf("num = %d | (expected: %d)\n", num2, expected2);
	printf("Result = %d\n", result2);
	if (result2 == expected2)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
}

void TestPrintWhereThreeBits()
{
	unsigned int arr [] = {7, 3, 2, 11, 10, 14, 15};
	size_t size = 7;
	
	printf("\n\033[1mTest PrintWhereThreeBits:\033[0m\n");
	printf("arr = {7, 3, 2, 11, 10, 14, 15} | size = 7 | (expected: 7, 11, 14)\n");
	
	PrintWhereThreeBits(arr,size);
}

void TestByteMirrorLoop()
{
	unsigned char n1 = 8;
	unsigned char expected1 = 16;
	unsigned char result1 = ByteMirrorLoop(n1);
	unsigned char n2 = 7;
	unsigned char expected2 = 224;
	unsigned char result2 = ByteMirrorLoop(n2);
	
	printf("\n\033[1mTest ByteMirrorLoop:\033[0m\n");
	printf("n1 = %u | (expected1: %d)\n", n1, expected1);
	printf("Result1 = %d\n", result1);
	if (result1 == expected1)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
	
	printf("n2 = %u | (expected2: %d)\n", n2, expected2);
	printf("Result2 = %d\n", result2);
	if (result2 == expected2)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
}

void TestByteMirrorNoLoop()
{
	unsigned char n1 = 8;
	unsigned char expected1 = 16;
	unsigned char result1 = ByteMirrorNoLoop(n1);
	unsigned char n2 = 7;
	unsigned char expected2 = 224;
	unsigned char result2 = ByteMirrorNoLoop(n2);
	
	printf("\n\033[1mTest ByteMirrorNoLoop:\033[0m\n");
	printf("n1 = %u | (expected1: %d)\n", n1, expected1);
	printf("Result1 = %d\n", result1);
	if (result1 == expected1)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
	
	printf("n2 = %u | (expected2: %d)\n", n2, expected2);
	printf("Result2 = %d\n", result2);
	if (result2 == expected2)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
}

void TestIs2n6BitsOn()
{
	unsigned char n1 = 255;
	int expected1 = 1;
	int result1 = Is2n6BitsOn(n1);
	unsigned char n2 = 30;
	int expected2 = 0;
	int result2 = Is2n6BitsOn(n2);
	
	printf("\n\033[1mTest Is2n6BitsOn:\033[0m\n");
	printf("n1 = %u | (expected1: %d)\n", n1, expected1);
	printf("Result1 = %d\n", result1);
	if (result1 == expected1)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
	
	printf("n2 = %u | (expected2: %d)\n", n2, expected2);
	printf("Result2 = %d\n", result2);
	if (result2 == expected2)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
}

void TestIsAny2n6BitsOn()
{
	unsigned char n1 = 30;
	int expected1 = 1;
	int result1 = IsAny2n6BitsOn(n1);
	unsigned char n2 = 29;
	int expected2 = 0;
	int result2 = IsAny2n6BitsOn(n2);
	
	printf("\n\033[1mTest IsAny2n6BitsOn:\033[0m\n");
	printf("n1 = %u | (expected1: %d)\n", n1, expected1);
	printf("Result1 = %d\n", result1);
	if (result1 == expected1)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
	
	printf("n2 = %u | (expected2: %d)\n", n2, expected2);
	printf("Result2 = %d\n", result2);
	if (result2 == expected2)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
}

void TestSwap3n5Bits()
{
	unsigned char n1 = 20;
	unsigned char expected1 = 20;
	unsigned char result1 = Swap3n5Bits(n1);
	unsigned char n2 = 51;
	unsigned char expected2 = 39;
	unsigned char result2 = Swap3n5Bits(n2);
	
	printf("\n\033[1mTest Swap3n5Bits():\033[0m\n");
	printf("n1 = %u | (expected1: %d)\n", n1, expected1);
	printf("Result1 = %d\n", result1);
	if (result1 == expected1)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
	
	printf("n2 = %u | (expected2: %d)\n", n2, expected2);
	printf("Result2 = %d\n", result2);
	if (result2 == expected2)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
}

void TestClosestDiv16()
{
	unsigned int n1 = 33;
	unsigned int expected1 = 32;
	unsigned int result1 = ClosestDiv16(n1);
	unsigned int n2 = 19;
	unsigned int expected2 = 16;
	unsigned int result2 = ClosestDiv16(n2);
	
	printf("\n\033[1mTest ClosestDiv16():\033[0m\n");
	printf("n1 = %u | (expected1: %d)\n", n1, expected1);
	printf("Result1 = %d\n", result1);
	if (result1 == expected1)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
	
	printf("n2 = %u | (expected2: %d)\n", n2, expected2);
	printf("Result2 = %d\n", result2);
	if (result2 == expected2)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
}

void TestSwap()
{
	int a = 33;
	int b = 12;
	int expected_a = 12;
	int expected_b = 33;
		
	printf("\n\033[1mTest Swap():\033[0m\n");
	printf("a = %d | b = %d | (expected: a=%d, b=%d)\n", a, b, expected_a, expected_b);
	Swap(&a, &b);
	printf("Result: a = %d | b = %d\n", a, b);
	if ((a == expected_a) && (b == expected_b))
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
}

void TestCountSetBitsLoop()
{
	unsigned int n1 = -10;
	unsigned int expected1 = 30;
	unsigned int result1 = CountSetBitsLoop(n1);
	unsigned int n2 = 14;
	unsigned int expected2 = 3;
	unsigned int result2 = CountSetBitsLoop(n2);
	
	printf("\n\033[1mTest CountSetBitsLoop():\033[0m\n");
	printf("n1 = %d | (expected1: %d)\n", n1, expected1);
	printf("Result1 = %d\n", result1);
	if (result1 == expected1)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
	
	printf("n2 = %u | (expected2: %d)\n", n2, expected2);
	printf("Result2 = %d\n", result2);
	if (result2 == expected2)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
}

void TestCountSetBitsNoLoop()
{
	int n1 = -1;
	int expected1 = 32;
	int result1 = CountSetBitsNoLoop(n1);
	int n2 = 25554;
	int expected2 = 8;
	int result2 = CountSetBitsNoLoop(n2);
	
	printf("\n\033[1mTest CountSetBitsNoLoop():\033[0m\n");
	printf("n1 = %d | (expected1: %d)\n", n1, expected1);
	printf("Result1 = %d\n", result1);
	if (result1 == expected1)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
	
	printf("n2 = %u | (expected2: %d)\n", n2, expected2);
	printf("Result2 = %d\n", result2);
	if (result2 == expected2)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
}

void TestPrintFloatBits()
{
	float num = 5.75f;
	
	printf("\n\033[1mTest PrintFloatBits():\033[0m\n");
	printf("num = %f\n", num);
		
	PrintFloatBits(num);
}
