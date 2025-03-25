/*----------------------includes----------------------*/
#include <stdio.h> /* For printf */
#include <stddef.h> /* For size_t */
#include <stdlib.h> /* For malloc */
#include <string.h>
#include "wbo.h"

/*---------------------Functions Forward Decleration---------------------*/
void TestMemset();
void TestMemcpy();
void TestMemmove();
void PrintBuffer(char* buff, size_t size);

/*---------------------main---------------------*/
int main()
{	
	TestMemset();
	TestMemcpy();
	TestMemmove();
	
	return 0;
}

/*-------------------------functions-------------------------*/
void TestMemset()
{
	char str[10] = "0000000000";
	
	printf("\033[1mTest Memset:\033[0m\n");
	printf("str before memset: %s\n", str);
	Memset((str+1), 49, 5);
	
	printf("str after memset: %s\n", str);
}

void TestMemcpy()
{
	char src[17] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', '\0'};
	char dest[17] = {0};
	
	printf("\n\033[1mTest Memcpy:\033[0m\n");
	
	Memcpy(dest, src, 17);
	
	printf("src: ");
	PrintBuffer(src, 17);
	
	printf("dest: ");
	PrintBuffer(dest,17);
}

void TestMemmove()
{
	char src[9] = "hello---\0";
	char* dest = src + 2;
	
	printf("\n\033[1mTest Memmove:\033[0m\n");
	
	printf("src before memove: ");
	PrintBuffer(src, 5);

	Memmove(dest, src, 5);
		
	printf("src after memmove: ");
	PrintBuffer(dest,5);
	

}

void PrintBuffer(char* buff, size_t size)
{
	size_t i = 0;
	
	for (i = 0; i < size; i++)
	{
		printf(" %c", buff[i]);
	}
	
	printf("\n");
}
