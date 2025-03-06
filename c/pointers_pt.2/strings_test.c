#include "strings.h"

void TestStrLen();
void TestStrCmp();
void TestStrCpy();
void TestStrNCpy();
void TestStrNCmp();
void TestStrCaseCmp();
void TestStrChr();
void TestStrDup();
void TestStrCat();
/*void TestStrNCat();*/
void TestStrStr();
void TestStrSpn(); 

int main()
{
	TestStrLen();
	TestStrCmp();
	TestStrCpy();
	TestStrNCpy();
	TestStrNCmp();
	TestStrCaseCmp();
	TestStrChr();
	TestStrDup();
	TestStrCat();
/*	TestStrNCat();*/
	TestStrStr(); 
	TestStrSpn();
	
	return 0;
}

void TestStrLen()
{
	char str[] = "abcd";
	
	printf("\033[1mTest StrLen:\033[0m\n");
	printf("Result => %ld\n", StrLen(str));

	if(StrLen(str) == 4)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
}

void TestStrCmp()
{
	char str1[] = "EQUAL";
	char str2[] = "EQUAL";
	char str3[] = "NOT EQUAL";
	char str4[] = "NOT EQUA";
	
	printf("\n\033[1mTest StrCmp:\033[0m\n");
	printf("str1: %s | str2: %s\n", str1, str2);
	printf("First result => %d\n", StrCmp(str1,str2));
	if(StrCmp(str1,str2) == 0)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
	
	printf("\nstr3: %s | str4: %s\n", str3, str4);
	printf("Second result => %d\n", StrCmp(str3,str4));
	if(StrCmp(str3,str4) != 0)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
}

void TestStrCpy()
{
	char src[] = "src";
	char dest[] = "dest";
	
	printf("\n\033[1mTest StrCpy:\033[0m\n");
	printf("src: %s | dest: %s\n", src, dest);
	StrCpy(dest, src);
	printf("My StrCpy => src: %s | dest: %s\n", src, dest);
	strcpy(dest, src);
	printf("Built-in strcpy => src: %s | dest: %s\n", src, dest);
}

void TestStrNCpy()
{
	char src[] = "Copy This";
	char dest[] = "destination";
	size_t n = 4;
	
	printf("\n\033[1mTest StrNCpy:\033[0m\n");
	printf("src: %s | dest: %s | n = %lu\n", src, dest, n);
	StrNCpy(dest, src, n);
	printf("My StrNCpy => src: %s | dest: %s\n", src, dest);
	strncpy(dest, src, n);
	printf("Built-in strncpy => src: %s | dest: %s\n", src, dest);
}

void TestStrNCmp()
{
/*	char str1[] = "EQUAL";
	char str2[] = "EQUAL";
	char str3[] = "no equal";
	char str4[] = "NO EQUAL";
	size_t n = 5;
	
	printf("\n\033[1mTest StrNCmp:\033[0m\n");
	printf("str1: %s | str2: %s | n = %lu\n", str1, str2, n);
	printf("My StrNCmp => %d\n", StrNCmp(str1, str2, n));
	printf("Built-in strncmp => %d\n", strncmp(str1, str2, n));
	printf("\nstr3: %s | str4: %s | n = %lu\n", str3, str4, n);
	printf("My StrNCmp => %d\n", StrNCmp(str3, str4, n));
	printf("Built-in strncmp => %d\n", strncmp(str3, str4, n)); */
	
	char* str1 = "hello";
	char* str2 = "hello2";
	char* str3 = "hell3";
	char* str4 = "ab";
	char* str5 = "ac";
	           
	printf("\n\033[1mTest StrNCmp:\033[0m\n");
	printf("str1: %s | str2: %s | str3: %s | str4: %s | str5: %s\n", str1, str2, str3, str4, str5);
	printf("StrNCmp(str1, str2, %d): %d\n", 4, StrNCmp(str1,str2,4));
	printf("StrNCmp(str1, str3, %d): %d\n", 6,  StrNCmp(str1,str3,6));
	printf("StrNCmp(str2, str3, %d): %d\n", 5, StrNCmp(str2,str3,5));
	printf("StrNCmp(str4, str5, %d): %d\n", 5, StrNCmp(str4,str5,5));
	printf("\n");
}

void TestStrCaseCmp()
{
	char str1[] = "equal";
	char str2[] = "EQUAL";
	
	printf("\n\033[1mTest StrCaseCmp:\033[0m\n");
	printf("str1: %s | str2: %s\n", str1, str2);
	printf("My StrCaseCmp => %d\n", StrCaseCmp(str1, str2));
	printf("Built-in strcasecmp => %d\n", strcasecmp(str1, str2));
}

void TestStrChr()
{
	char str[] = "find 'n'";
	char ch = 'n';
	
	printf("\n\033[1mTest StrChr:\033[0m\n");
	printf("str: %s\n", str);
	printf("My StrChr => %s\n", StrChr(str, ch));
	printf("Built-in strchr => %s\n", strchr(str, ch));
}

void TestStrDup()
{
	char str[] = "duplicate this";
	char* dup_ptr = StrDup(str);
	
	printf("\n\033[1mTest StrDup:\033[0m\n");
	printf("str: %s\n", str);
	printf("My StrDup => %s\n", dup_ptr);
	
	free(dup_ptr);
}

void TestStrCat()
{
	char src[] = "here";
	char dest[12] = "copied ";
	char* result = StrCat(dest, src);
	
	printf("\n\033[1mTest StrCat:\033[0m\n");
	printf("dest: %s | src: %s\n", dest, src);
	printf("Result => %s\n", result);
	if(StrCmp(result, "copied here") == 0)
	{
		printf("PASSED!\n");
	}
	
	else
	{
		printf("FAILED!\n");
	}
}

void TestStrStr()
{
	char haystack[] = "1234aabc5678";
	char needle[] = "abc";
	
	printf("\n\033[1mTest StrStr:\033[0m\n");
	printf("haystack: %s | needle: %s\n", haystack, needle);
	
	printf("result: %s\n", StrStr(haystack, needle));
	
	/*if((StrStr(haystack, needle)) == NULL)
	{
		printf("no");
	}
	else
	{
		printf("yes");
	}*/
}

void TestStrSpn()
{
	char str1[] = "abc";
	char str2[] = "abc";
	
	size_t res = strspn(str1, str2);

	
	printf("\n %lu\n", res);
}






























