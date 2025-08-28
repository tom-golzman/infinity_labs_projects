/**
	Written By: Tom Golzman
	Date: 21/08/2025
	Reviewed By: 
**/

/*********************************** Includes ************************************/
#include <cassert>		// assert()
#include <cstring>		// std::strlen()
#include <cstdlib>		// memcpy()

#include "simple_string.hpp"

/************************************* *************************************/
namespace ilrd
{
//====================== String ===================== //
String::String(const char* str_): m_buff(strlen(str_) + 1)
{
	char* buff = m_buff.GetW();

	memcpy(buff, str_, strlen(str_) + 1);
}

char& String::operator[](size_t index)
{
	assert(index < m_buff.Size() - 1);

	char* buff = m_buff.GetW();

	return buff[index];
}

char String::operator[](size_t index) const
{
	assert(index < m_buff.Size() - 1);

	const char* buff = m_buff.GetR();

	return buff[index];
}

size_t String::Length() const NOEXCEPT
{
	return m_buff.Size() - 1;
}

const char* String::Cstr() const
{
	return m_buff.GetR();
}

//====================== Global Operators ===================== //
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

}//namespace ilrd