#include <stddef.h> /* For size_t */

#pragma once

long Pow2(unsigned int x, unsigned int y);
int IsPow2Loop(unsigned int n);
int IsPow2NoLoop(unsigned int n);
int AddOne(int n);
void PrintWhereThreeBits(unsigned int* arr, size_t size); 
unsigned char ByteMirrorLoop(unsigned char n);
unsigned char ByteMirrorNoLoop(unsigned char n);
int Is2n6BitsOn(unsigned char c);
int IsAny2n6BitsOn(unsigned char c);
unsigned char Swap3n5Bits(unsigned char c);
unsigned int ClosestDiv16(unsigned int n);
void Swap(int* a, int* b);
unsigned int CountSetBitsLoop(int n);
unsigned int CountSetBitsNoLoop(int n);
void PrintFloatBits(float f);
