#include <cstring>
#include <iostream>

#include "test_utils.hpp"
#include "simple_string.hpp"

using ilrd::String;

void TestCtor();
void TestOpBracketRead();
void TestOpBracketWrite();
void TestSetAt();
void TestGetAt();
void TestLength();
void TestCstr();
void TestOpAssign();
void TestOpEq();
void TestOpLt();
void TestOpGt();
void TestOpAddressOf();

int main(void)
{
	TestCtor();
	TestOpBracketRead();
	TestOpBracketWrite();
	TestSetAt();
	TestGetAt();
	TestLength();
	TestCstr();
	TestOpAssign();
	TestOpEq();
	TestOpLt();
	TestOpGt();
	TestOpAddressOf();

	return 0;
}

void TestCtor()
{
	TITLE("Constructor");

	String s1("abc");
	String s2("");

	RUN_TEST("s1 == 'abc'", strcmp(s1.Cstr(), "abc") == 0);
	RUN_TEST("s2 == ''", strcmp(s2.Cstr(), "") == 0);
}

void TestOpBracketRead()
{
	TITLE("operator[] (read)");

	String s("hello");

	RUN_TEST("s[0] == 'h'", s[0] == 'h');
	RUN_TEST("s[4] == 'o'", s[4] == 'o');
}

void TestOpBracketWrite()
{
	TITLE("operator[] (write)");

	String s("test");

	s[0] = 'b';
	s[1] = 'e';

	RUN_TEST("s == 'best'", strcmp(s.Cstr(), "best") == 0);
}

void TestSetAt()
{
	TITLE("SetAt()");

	String s("Hello");

	s.SetAt(0, 'Y');
	s.SetAt(4, '!');

	RUN_TEST("s == 'Yell!'", strcmp(s.Cstr(), "Yell!") == 0);
}

void TestGetAt()
{
	TITLE("GetAt()");

	String s("World");

	RUN_TEST("GetAt(0)", s.GetAt(0) == 'W');
	RUN_TEST("GetAt(4)", s.GetAt(4) == 'd');
}

void TestLength()
{
	TITLE("Length()");

	String s1("12345");
	String s2("");

	RUN_TEST("s1.Length() == 5", s1.Length() == 5);
	RUN_TEST("s2.Length() == 0", s2.Length() == 0);
}

void TestCstr()
{
	TITLE("Cstr()");

	String s("World");

	RUN_TEST("strcmp(Cstr(), 'World')", strcmp(s.Cstr(), "World") == 0);
}

void TestOpAssign()
{
	TITLE("operator= (assignment)");

	String s1("foo");
	String s2("bar");

	s1 = s2;

	RUN_TEST("s1 == s2", strcmp(s1.Cstr(), s2.Cstr()) == 0);

	s1 = s1;
	RUN_TEST("self-assignment no change", strcmp(s1.Cstr(), "bar") == 0);
}

void TestOpEq()
{
	TITLE("operator==");

	String s1("abc");
	String s2("abc");
	String s3("abd");

	RUN_TEST("s1 == s2", s1 == s2);
	RUN_TEST("s1 != s3", !(s1 == s3));
}

void TestOpLt()
{
	TITLE("operator<");

	String a("aaa");
	String b("aab");
	String c("aa");
	String d("aaa");

	RUN_TEST("a < b", a < b);
	RUN_TEST("c < a", c < a);
	RUN_TEST("a < d == false", !(a < d));
}

void TestOpGt()
{
	TITLE("operator>");

	String a("zzz");
	String b("zza");
	String c("zzzz");
	String d("zzz");

	RUN_TEST("a > b", a > b);
	RUN_TEST("c > a", c > a);
	RUN_TEST("a > d == false", !(a > d));
}

void TestOpAddressOf()
{
	TITLE("operator& (CharProxy)");

	String s1("abc");
	String s2 = s1;

	char* addr = &s1[1];

	*addr = 'Z';

	RUN_TEST("s1 == 'aZc'", strcmp(s1.Cstr(), "aZc") == 0);
	RUN_TEST("s2 == 'abc'", strcmp(s2.Cstr(), "abc") == 0);
}