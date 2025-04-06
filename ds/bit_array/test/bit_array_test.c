/************************************includes************************************/
#include <stdio.h> /* printf */
#include "bit_array.h" 

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define TITLE "\033[1;4m"

/************************************Functions Forward Decleration************************************/
void TestInit();
void TestSetVal();
void TestGetVal();
void TestCountOn();
void TestCountOff();
void TestSetOn();
void TestSetOff();
void TestToggle();
void TestSetAllOn();
void TestSetAllOff();
void TestMirror();
void TestRotateLeft();
void TestRotateRight();
void TestToString();
static void PrintBits(bit_array_t arr);

/************************************main************************************/
int main(void)
{
	TestInit();
	TestSetVal();
	TestGetVal();
	TestCountOn();
	TestCountOff();
	TestSetOn();
	TestSetOff();
	TestToggle();
	TestSetAllOn();
	TestSetAllOff();	
	TestMirror();
	TestRotateLeft();
	TestRotateRight();
	TestToString();
	
	return (0);
}

void TestInit()
{
	bit_array_t arr_off = BitArrInit(OFF);
	bit_array_t arr_on = BitArrInit(ON);
	
	printf(TITLE "Test Init:\n" RESET);
	
	printf("arr_off -> ");
	PrintBits(arr_off);
	printf("arr_on -> ");
	PrintBits(arr_on);
}

void TestSetVal()
{
	bit_array_t arr1 = BitArrInit(OFF);
	bit_array_t arr2 = BitArrInit(OFF);	
	bit_array_t arr3 = BitArrInit(ON);
	bit_array_t arr4 = BitArrInit(ON);
			
	printf(TITLE "\nTest SetVal:\n" RESET);	
	
	printf("arr1 before SetVal: ");
	PrintBits(arr1);
	arr1 = BitArrSetVal(arr1 , 0, ON);
	printf("BitArrSetVal(arr1 , 0, ON)\n");
	printf("arr1 after SetVal: ");
	PrintBits(arr1);
	if (arr1 == 1)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
	
	printf("arr2 before SetVal: ");
	PrintBits(arr2);
	arr2 = BitArrSetVal(arr2 , 63, ON);
	printf("BitArrSetVal(arr2 , 63, ON)\n");
	printf("arr2 after SetVal: ");
	PrintBits(arr2);
	if (arr2 == 9223372036854775808)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
	
	printf("arr3 before SetVal: ");
	PrintBits(arr3);
	arr3 = BitArrSetVal(arr3 , 0, OFF);
	printf("BitArrSetVal(arr3 , 0, OFF)\n");
	printf("arr3 after SetVal: ");
	PrintBits(arr3);
	if (arr3 == 18446744073709551614)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
	
	printf("arr4 before SetVal: ");
	PrintBits(arr4);
	arr4 = BitArrSetVal(arr4 , 63, OFF);
	printf("BitArrSetVal(arr4 , 63, OFF)\n");
	printf("arr4 after SetVal: ");
	PrintBits(arr4);
	if (arr4 == 9223372036854775807)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
}

void TestGetVal()
{
	bit_array_t arr1 = (size_t)84325324;
	bit_array_t arr2 = (size_t)9223372036854774783;	
			
	printf(TITLE "\nTest GetVal:\n" RESET);
	
	printf("arr1: ");
	PrintBits(arr1);
	printf("BitArrGetVal(arr1, 18)\n");
	if (BitArrGetVal(arr1, 18) == 1)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
	
	printf("arr2: ");
	PrintBits(arr2);
	printf("BitArrGetVal(arr2, 10)\n");
	if (BitArrGetVal(arr2, 10) == 0)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
}

void TestCountOn()
{
	bit_array_t arr1 = BitArrInit(OFF);
	bit_array_t arr2 = BitArrInit(ON);	
	bit_array_t arr3 = (size_t)7;
			
	printf(TITLE "\nTest CountOnl:\n" RESET);
	
	printf("arr1: ");
	PrintBits(arr1);
	printf("BitArrCountOn(arr1)\n");
	if (BitArrCountOn(arr1) == 0)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
	
	printf("arr2: ");
	PrintBits(arr2);
	printf("BitArrCountOn(arr2)\n");
	if (BitArrCountOn(arr2) == 64)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
	
	printf("arr3: ");
	PrintBits(arr3);
	printf("BitArrCountOn(arr3)\n");
	if (BitArrCountOn(arr3) == 3)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
}

void TestCountOff()
{
	bit_array_t arr1 = BitArrInit(OFF);
	bit_array_t arr2 = BitArrInit(ON);	
	bit_array_t arr3 = (size_t)7;
			
	printf(TITLE "\nTest CountOff:\n" RESET);
	
	printf("arr1: ");
	PrintBits(arr1);
	printf("BitArrCountOff(arr1)\n");
	if (BitArrCountOff(arr1) == 64)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
	
	printf("arr2: ");
	PrintBits(arr2);
	printf("BitArrCountOff(arr2)\n");
	if (BitArrCountOff(arr2) == 0)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
	
	printf("arr3: ");
	PrintBits(arr3);
	printf("BitArrCountOff(arr3)\n");
	if (BitArrCountOff(arr3) == 61)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
}

void TestSetOn()
{
	bit_array_t arr1 = BitArrInit(OFF);
	bit_array_t arr2 = BitArrInit(OFF);
	bit_array_t arr3 = BitArrInit(OFF);
			
	printf(TITLE "\nTest SetOn:\n" RESET);	
	
	printf("arr1 before SetOn: ");
	PrintBits(arr1);
	arr1 = BitArrSetOn(arr1, 0);
	printf("SetOn(arr1 , 0)\n");
	printf("arr1 after SetOn: ");
	PrintBits(arr1);
	if (arr1 == 1)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
	
	printf("arr2 before SetOn: ");
	PrintBits(arr2);
	arr2 = BitArrSetOn(arr2, 63);
	printf("SetOn(arr2 , 63)\n");
	printf("arr2 after SetOn: ");
	PrintBits(arr2);
	if (arr2 == 9223372036854775808)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
	
	printf("arr3 before SetOn: ");
	PrintBits(arr3);
	arr3 = BitArrSetOn(arr3, 6);
	printf("SetOn(arr3 , 6)\n");
	printf("arr3 after SetOn: ");
	PrintBits(arr3);
	if (arr3 == 64)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
}

void TestSetOff()
{
	bit_array_t arr1 = BitArrInit(ON);
	bit_array_t arr2 = BitArrInit(ON);
	bit_array_t arr3 = BitArrInit(ON);
			
	printf(TITLE "\nTest SetOff:\n" RESET);	
	
	printf("arr1 before SetOff: ");
	PrintBits(arr1);
	arr1 = BitArrSetOff(arr1, 0);
	printf("SetOff(arr1 , 0)\n");
	printf("arr1 after SetOff: ");
	PrintBits(arr1);
	if (arr1 == 18446744073709551614)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
	
	printf("arr2 before SetOff: ");
	PrintBits(arr2);
	arr2 = BitArrSetOff(arr2, 63);
	printf("SetOff(arr2 , 63)\n");
	printf("arr2 after SetOff: ");
	PrintBits(arr2);
	if (arr2 == 9223372036854775807)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
	
	printf("arr3 before SetOff: ");
	PrintBits(arr3);
	arr3 = BitArrSetOff(arr3, 6);
	printf("SetOff(arr3 , 6)\n");
	printf("arr3 after SetOff: ");
	PrintBits(arr3);
	if (arr3 == 18446744073709551551)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
}

void TestToggle()
{
	bit_array_t arr1 = BitArrInit(ON);
	bit_array_t arr2 = BitArrInit(ON);
	
	printf(TITLE "\nTest Toggle:\n" RESET);	
		
	printf("arr1 before Toggle: ");
	PrintBits(arr1);
	arr1 = BitArrToggle(arr1, 0);
	printf("Toggle(arr1, 0)\n");
	printf("arr1 after Toggle: ");
	PrintBits(arr1);
	if (arr1 == 18446744073709551614)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
	
	printf("arr2 before Toggle: ");
	PrintBits(arr2);
	arr2 = BitArrToggle(arr2, 63);
	printf("Toggle(arr2, 0)\n");
	printf("arr2 after Toggle: ");
	PrintBits(arr2);
	if (arr2 == 9223372036854775807)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
}

void TestSetAllOn()
{
	bit_array_t arr1 = BitArrInit(OFF);
	
	printf(TITLE "\nTest SetAllOn:\n" RESET);	
		
	printf("arr1 before SetAllOn: ");
	PrintBits(arr1);
	arr1 = BitArrSetAllOn(arr1);
	printf("SetAllOn(arr1)\n");
	printf("arr1 after SetAllOn: ");
	PrintBits(arr1);
	if (arr1 == -1)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
}

void TestSetAllOff()
{
	bit_array_t arr1 = BitArrInit(ON);
	
	printf(TITLE "\nTest SetAllOff:\n" RESET);	
		
	printf("arr1 before SetAllOff: ");
	PrintBits(arr1);
	arr1 = BitArrSetAllOff(arr1);
	printf("SetAllOff(arr1)\n");
	printf("arr1 after SetAllOff: ");
	PrintBits(arr1);
	if (arr1 == 0)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
}

void TestMirror()
{
	bit_array_t arr1 = (size_t)7;
	
	printf(TITLE "\nTest Mirror:\n" RESET);	
		
	printf("arr1 before Mirror: ");
	PrintBits(arr1);
	arr1 = BitArrMirror(arr1);
	printf("Mirror(arr1)\n");
	printf("arr1 after Mirror: ");
	PrintBits(arr1);
	if (arr1 == 16140901064495857664)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
}

void TestRotateLeft()
{
	bit_array_t arr1 = (size_t)1;
	
	printf(TITLE "\nTest RotateLeft:\n" RESET);	
		
	printf("arr1 before RotateLeft: ");
	PrintBits(arr1);
	arr1 = BitArrRotateLeft(arr1, 2);
	printf("RotateLeft(arr1, 2)\n");
	printf("arr1 after RotateLeft: ");
	PrintBits(arr1);
	if (arr1 == 4)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
}

void TestRotateRight()
{
	bit_array_t arr1 = (size_t)1;
	
	printf(TITLE "\nTest RotateRight:\n" RESET);	
		
	printf("arr1 before RotateRight: ");
	PrintBits(arr1);
	arr1 = BitArrRotateRight(arr1, 10);
	printf("RotateRight(arr1, 2)\n");
	printf("arr1 after RotateRight: ");
	PrintBits(arr1);
	if (arr1 == 18014398509481984)
	{
		printf(GREEN "PASSED\n" RESET);
	}
	else
	{
		printf(RED "FAILED\n" RESET);
	}
}

void TestToString()
{
	bit_array_t arr1 = (size_t)3;
	char* buffer = (char*)malloc(sizeof(char) * STRING_BUFFER_SIZE);
	
	printf(TITLE "\nTest ToString:\n" RESET);	
		
	buffer = BitArrToString(arr1, buffer);
	printf("arr1: %s\n", buffer);
	
	free(buffer);
}

static void PrintBits(bit_array_t arr)
{
	int i = WORD_SIZE_BITS;
	
	for (i = WORD_SIZE_BITS - 1; i >= 0; i--)
	{
		printf("%lu", ((arr >> i) & 1));
	}
	printf("\n");
}
