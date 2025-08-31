#include <cstring>
#include <iostream>

#include "test_utils.hpp"

#include "simple_string.hpp"

void TestCtor();
void TestLength();
void TestAssignOp();
void TestOpEq();
void TestOpSmall();
void TestOpBig();
void TestOp();
void TestGetAndSetAt();

using ilrd::String;

int main(void)
{
	TestCtor();
	TestLength();
	TestAssignOp();
	TestOpEq();
	TestOpSmall();
	TestOpBig();
	TestOp();
	TestGetAndSetAt();

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

	RUN_TEST("s8[0] = 'H'", s8[0] == 'H');

	s8[1] = 'A';

	RUN_TEST("after s8[1] = 'A'", strcmp(s8.Cstr(), "HAllo") == 0);
}

void TestGetAndSetAt()
{
	TITLE("GetAt()");

	String s9 = "Hello";

	RUN_TEST("s9.GetAt(1) == 'e'", s9.GetAt(1) == 'e');

	s9.SetAt(2, '!');

	RUN_TEST("s9.SetAt(2, '!')", strcmp(s9.Cstr(), "He!lo") == 0);
}