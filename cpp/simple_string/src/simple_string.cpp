/**
	Written By: Tom Golzman
	Date: 21/08/2025
	Reviewed By: 
**/

/************************************ Includes *************************************/
#include <cassert>	// assert()
#include <cstring>	// std::strlen()
#include <cstdlib>	// memcpy()

#include "utils.h"	/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

#include "simple_string.hpp"

/************************************* *************************************/

namespace ilrd
{
String::String(const char* str_): m_str(CloneStr(str_))
{}

String::String(const String& other_): m_str(CloneStr(other_.m_str))
{}

String& String::operator=(const String& other_)
{
	// handles assignment to self

	const char* temp = CloneStr(other_.m_str);
	
	delete[] m_str;

	m_str = temp;
	
	return *this;
}

bool operator==(const String& string1, const String& string2)
{
	const char* str1 = string1.Cstr();
	const char* str2 = string2.Cstr();

	while ('\0' != *str1 && '\0' != *str2)
	{
		if (*str1 != *str2)
		{
			return false;
		}

		++str1;
		++str2;
	}

	return ('\0' == *str1 && '\0' == *str2);
}

bool operator<(const String& string1, const String& string2)
{
	const char* str1 = string1.Cstr();
	const char* str2 = string2.Cstr();

	while ('\0' != *str1 && '\0' != *str2)
	{
		if (*str1 < *str2)
		{
			return true;
		}
		else if (*str1 > *str2)
		{
			return false;
		}

		++str1;
		++str2;
	}

	return ('\0' == *str1 && '\0' != *str2);
}

bool operator>(const String& string1, const String& string2)
{
	const char* str1 = string1.Cstr();
	const char* str2 = string2.Cstr();

	while ('\0' != *str1 && '\0' != *str2)
	{
		if (*str1 > *str2)
		{
			return true;
		}
		else if (*str1 < *str2)
		{
			return false;
		}

		++str1;
		++str2;
	}

	return ('\0' != *str1 && '\0' == *str2);
}

String::~String()
{
	delete[] m_str;

	DEBUG_ONLY(
		m_str = BAD_MEM(char*);
	);
}

size_t String::Length() const
{
	return strlen(m_str);
}

const char* String::Cstr() const
{
	return m_str;
}

//static
const char* String::CloneStr(const char* str_)
{
	assert(NULL != str_);
	
	size_t len = strlen(str_);
	char* ret = new char[len + 1];

	memcpy(ret, str_, len + 1);

	return ret;
}

}//namespace ilrd
