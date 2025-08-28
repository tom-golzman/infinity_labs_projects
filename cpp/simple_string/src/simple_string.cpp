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
//================== StringManager ================== //
StringManager::StringManager(const char* str_): m_str(StrDup(str_))
{}

StringManager::StringManager(const StringManager& other_): m_str(StrDup(other_.m_str))
{}

StringManager& StringManager::operator=(const StringManager& other_)
{
	// handles self assignment

	char* temp = StrDup(other_.m_str);
	
	delete[] m_str;

	m_str = temp;

	return *this;
}

StringManager::~StringManager() NOEXCEPT
{
	delete[] m_str;

	DEBUG_ONLY(
		m_str = BAD_MEM(char*);
	);
}

char& StringManager::operator[](size_t index)
{
	assert(index < Length());

	return m_str[index];
}

size_t StringManager::Length() const
{
	return strlen(m_str);
}

const char* StringManager::Cstr() const
{
	return m_str;
}

//static
char* StringManager::StrDup(const char* str_)
{
	assert(NULL != str_);
	
	size_t len = strlen(str_);
	char* ret = new char[len + 1];

	memcpy(ret, str_, len + 1);

	return ret;
}

//====================== String ===================== //
String::String(const char* str_): m_data(str_)
{}

String::String(const String& other_): m_data(other_.m_data)
{}

String& String::operator=(const String& other_)
{
	// handles self assignment

	StringManager temp = other_.m_data;
	
	m_data = temp;
	//TODO: no need for temp
	
	return *this;
}

char& String::operator[](size_t index)
{
	assert(index < Length());

	return m_data[index];
}

size_t String::Length() const
{
	return m_data.Length();
}

const char* String::Cstr() const
{
	return m_data.Cstr();
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