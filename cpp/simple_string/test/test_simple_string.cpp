/************************************ Includes *************************************/
#include <iostream> /* printf */

#include "simple_string.hpp"

/************************************** main ***************************************/
int main(void)
{
	using ilrd::String;
	
	String s1("string1");
	String s2("string2");
	String s3 = s2;

	std::cout << "s1(string1): " << s1.Cstr() << " ,len: " << s1.Length() << std::endl;
	
	std::cout << "s2(string2): " << s2.Cstr() << " ,len: " << s1.Length() << std::endl;

	std::cout << "s3(string2): " << s3.Cstr() << " ,len: " << s1.Length() << std::endl;

	s2 = s1;
	s1 = s3;

	std::cout << "s1(string2): " << s1.Cstr() << " ,len: " << s1.Length() << std::endl;
	
	std::cout << "s2(string1): " << s2.Cstr() << " ,len: " << s1.Length() << std::endl;

	std::cout << "s3(string2): " << s3.Cstr() << " ,len: " << s1.Length() << std::endl;

	std::cout << "s1 == s2(0): " << (s1 == s2) << std::endl;

	std::cout << "s1 == s3(1): " << (s1 == s3) << std::endl;

	String s4 = "abc";
	String s5 = "abd";
	String s6 = "abcc";
	String s7 = "ab";

	std::cout << "s4 < s5(1): " << (s4 < s5) << std::endl;

	std::cout << "s4 < s6(1): " << (s4 < s6) << std::endl;

	std::cout << "s4 < s7(0): " << (s4 < s7) << std::endl;

	std::cout << "s7 < s4(1): " << (s7 < s4) << std::endl;
	
	std::cout << "s4 < s4(0): " << (s4 < s4) << std::endl;
	
	std::cout << "s4 > s5(0): " << (s4 > s5) << std::endl;
		
	std::cout << "s5 > s4(1): " << (s5 > s4) << std::endl;
		
	std::cout << "s4 > s4(0): " << (s4 > s4) << std::endl;
	
	String s8 = "Hello";

	std::cout << "s8 before (Hello): " << s8.Cstr() << std::endl;

	s8[4] = '!';

	std::cout << "s8 after (Hell!): " << s8.Cstr() << std::endl;

	return (0);
}