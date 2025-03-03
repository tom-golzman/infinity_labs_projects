
#include "exe.h"

void TestSwap ();
void TestCpyArray();
void TestPrintAddress();
void TestSwapSizeT();

int main ()
{
	TestSwap();
	TestCpyArray();
	TestPrintAddress();
	TestSwapSizeT();
	return 0;
}

void TestSwap ()
{
	int a = 5, b = 10;

	printf("Before swap: a = %d, b = %d\n", a, b);

	Swap(&a, &b);

	printf("After swap: a = %d, b = %d\n", a, b);

	if (a == 10 && b == 5)
	{
		printf("TEST PASSED!\n");
	}
	else
	{
		printf("TEST FAILED!\n");
	}
}

void TestCpyArray()
{
	int src[] = {1,2,3,4,5};
	int dest[5];
	
	CpyArray(src, sizeof(src[0]), sizeof(src)/sizeof(src[0]), dest);
	
	printf("\nCopied array: ");
	for (size_t i = 0; i < sizeof(src)/sizeof(src[0]); i++)
	{
		printf("%d ", dest[i]);
	}
	printf("\n");
}

void TestPrintAddress()
{
	PrintAddress();
}

void TestSwapSizeT()
{
	size_t a = 10, b = 20;
	size_t* ptr1 = &a;
	size_t* ptr2 = &b;

	printf("\nBefore swap:\n");
	printf("a = %lu, b = %lu\n", a, b);
	printf("ptr1 = %p, ptr2 = %p\n", (void*)ptr1, (void*)ptr2);

	SwapSizeT(&a, &b);
	printf("\nAfter swapping values:\n");
	printf("a = %lu, b = %lu\n", a, b);
        printf("ptr1 = %p, ptr2 = %p\n", (void*)ptr1, (void*)ptr2);

	SwapPointers(&ptr1, &ptr2);
	printf("\nAfter swapping pointers:\n");
	printf("ptr1 = %p, ptr2 = %p\n", (void*)ptr1, (void*)ptr2);
}





