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
String::String(const char* str_): m_str(StrDup(str_))
{}

String::String(const String& other_): m_str(StrDup(other_.m_str))
{}

String& String::operator=(const String& other_)
{
	// handles self assignment

	const char* temp = StrDup(other_.m_str);
	
	delete[] m_str;

	m_str = temp;
	
	return *this;
}

bool operator==(const String& s1, const String& s2)
{
	return !(strcmp(s1.Cstr(), s2.Cstr()));
}

bool operator<(const String& s1, const String& s2)
{
	return (strcmp(s1.Cstr(), s2.Cstr()) < 0);
}

bool operator>(const String& s1, const String& s2)
{
	return (strcmp(s1.Cstr(), s2.Cstr()) > 0);
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
const char* String::StrDup(const char* str_)
{
	assert(NULL != str_);
	
	size_t len = strlen(str_);
	char* ret = new char[len + 1];

	memcpy(ret, str_, len + 1);

	return ret;
}

}//namespace ilrd
