#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#pragma once

size_t StrLen(const char* str);
int StrCmp(const char* str1, const char* str2);
char* StrCpy(char* dest, const char* src);
char* StrNCpy(char* dest, const char* src, size_t n);
int StrNCmp(const char* str1, const char* str2, size_t n);
int StrCaseCmp(const char* str1, const char* str2);
char* StrChr(const char* str, int c);
char* StrDup(const char* str);
char* StrCat(char* dest, const char* src);
char* StrNCat(char* dest, const char* src, size_t n);
char* StrStr(const char* haystack, const char* needle);
size_t StrSpn(const char* str1, const char* str2);









