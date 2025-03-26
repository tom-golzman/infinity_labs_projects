#pragma once

/*---------------------Functions Forward Decleration---------------------*/
char* IToA10(int n, char* str);
int AToI10(char* str);
char* IToABase(int n, char* str, int base);
int AToIBase(char* str, int base);
int IsLittleEndian();
void PrintUniqueLetters(const char* arr1, size_t size1, const char* arr2, size_t size2 , const char* arr3, size_t size3);

/*----------------------includes----------------------*/
#define IS_LITTLE_ENDIAN (*(unsigned char *)&(int){1} == 1)
