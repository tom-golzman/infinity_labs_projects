/************************************includes************************************/
#include "circular_buffer.h"
#include <stdio.h> /* printf */

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define TITLE "\033[1;4m"

/************************************main************************************/
int main(void)
{
cb_t* buff_write = CBCreate(5);
	char buff_read[5] = {'0', '0', '0', '0', '0'};
	char input1[] = {'A', 'B', 'C'};
	char input2[] = {'D', 'E'};
	char input3[] = {'X', 'Y', 'Z'};
	size_t written = 0;
	size_t read = 0;
	size_t i = 0;

	printf(TITLE "Test: CBWrite Regular\n" RESET);
	printf("input1[] = A,B,C | n = 3 | buff_write = ");
	PrintCharBuffer(buff_write, CBGetCapacity(buff_write));
	written = CBWrite(buff_write, input1, 3);
	printf("written: %lu | after write: ", written);
	PrintCharBuffer(buff_write, CBGetCapacity(buff_write));

	printf(TITLE "\nTest: CBWrite Complete Fill\n" RESET);
	printf("input2[] = D,E | n = 2 | buff_write = ");
	PrintCharBuffer(buff_write, CBGetCapacity(buff_write));	
	written = CBWrite(buff_write, input2, 2);
	printf("written: %lu | after write: ", written);	
	PrintCharBuffer(buff_write, CBGetCapacity(buff_write));


	printf(TITLE "\nTest: CBRead 3 Bytes\n" RESET);
	read = CBRead(buff_write, buff_read, 3);
	printf("read: %lu | data: ", read);
	for (i = 0; i < read; ++i)
	{
		printf("%c ", buff_read[i]);
	}
	printf("| after read: ");
	PrintCharBuffer(buff_write, CBGetCapacity(buff_write));


	printf(TITLE "\nTest: CBWrite Overflow Attempt\n" RESET);
	printf("input2[] = X,Y,Z | n = 3 | buff_write = ");
	PrintCharBuffer(buff_write, CBGetCapacity(buff_write));		
	written = CBWrite(buff_write, input3, 3);
	printf("written: %lu | after write: ", written);	
	PrintCharBuffer(buff_write, CBGetCapacity(buff_write)); 

	printf(TITLE "\nTest: CBRead Remaining Bytes\n" RESET);
	read = CBRead(buff_write, buff_read, 5);
	printf("read: %lu | data: ", read);
	for (i = 0; i < read; ++i)
	{
		printf("%c ", buff_read[i]);
	}
	printf("| after read: ");
	PrintCharBuffer(buff_write, CBGetCapacity(buff_write));

	CBDestroy(buff_write);
	
	return (0);
}
