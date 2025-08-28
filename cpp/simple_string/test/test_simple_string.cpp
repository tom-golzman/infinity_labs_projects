/************************************ Includes *************************************/
#include <cstring>			// strcmp()
#include <iostream>

#include "test_utils.hpp"	// RUN_TEST, colors, titles, status

#include "simple_string.hpp"

/******************************* Forward Declaration ********************************/
void TestCtor();
void TestLength();
void TestAssignOp();
void TestOpEq();
void TestOpSmall();
void TestOpBig();
void TestOp();

using ilrd::String;

/************************************** main ***************************************/
int main(void)
{
	TestCtor();
	TestLength();
	TestAssignOp();
	TestOpEq();
	TestOpSmall();
	TestOpBig();
	TestOp();
	
	return (0);
}

void TestCtor()
{
    TITLE("constructor");

	String s1("string1");
	String s2("");
	
	RUN_TEST("s1(string1)", strcmp(s1.Cstr(), "string1") == 0);
	RUN_TEST("s2("")", strcmp(s2.Cstr(), "") == 0);
}

void TestLength()
{
    TITLE("Length()");

	String s1("string1");
	String s2("");
	
	RUN_TEST("s1.Length()", s1.Length() == 7);
	RUN_TEST("s2.Length()", s2.Length() == 0);
}

void TestOpEq()
{
    TITLE("operator==");

	String s1("string1");
	String s2("string1");
	String s3("string3");

	RUN_TEST("s1 == s2(true)" ,(s1 == s2) == true);
	RUN_TEST("s1 == s3(false)" ,(s1 == s3) == false);
}

void TestAssignOp()
{
    TITLE("assignment operator");

	String s1("string1");
	String s2("string2");
	String s3("string3");

	s1 = s3;

	RUN_TEST("s1 == s2(false)" ,(s1 == s2) == false);
	RUN_TEST("s1 == s3(true)" ,(s1 == s3) == true);
}

void TestOpSmall()
{
    TITLE("operator <");

	String s4 = "abc";
	String s5 = "abd";
	String s6 = "abcc";
	String s7 = "ab";

	RUN_TEST("s4 < s5", (s4 < s5) == true);

	RUN_TEST("s4 < s6", (s4 < s6) == true);

	RUN_TEST("s4 < s7", (s4 < s7) == false);

	RUN_TEST("s7 < s4", (s7 < s4) == true);
	
	RUN_TEST("s4 < s4", (s4 < s4) == false);
}

void TestOpBig()
{
    TITLE("operator >");

	String s4 = "abc";
	String s5 = "abd";
	String s6 = "abcc";
	String s7 = "ab";

	RUN_TEST("s4 > s5", (s4 > s5) == false);
	RUN_TEST("s5 > s4", (s5 > s4) == true);
	RUN_TEST("s4 > s4", (s4 > s4) == false);
}

void TestOp()
{
    TITLE("operator []");

	String s8 = "Hello";

	s8[4] = '!';

	RUN_TEST("s8 after s8[4] = '!'", strcmp(s8.Cstr(), "Hell!") == 0);
}