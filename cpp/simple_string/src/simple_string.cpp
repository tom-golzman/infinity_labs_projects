/**
	Written By: Tom Golzman
	Date: 21/08/2025
	Reviewed By: 
**/

/************************************ Includes *************************************/
#include <cassert>	// assert()
#include <cstring>	// std::strlen, std::strcpy

#include "simple_string.hpp"

/************************************* Defines *************************************/

namespace ilrd
{
/******************************** Static Functions *********************************/
static void CopyString(char* dest_, const char* src_);
static size_t StrLen(const char* str_);
static char* CloneStr(const char* str_);

/************************************ Functions ************************************/
String::String(const char* str_): m_str(CloneStr(str_))
{}

String::String(const String& other_): m_str(CloneStr(other_.m_str))
{}

String& String::operator=(const String& other_)
{
	delete[] this->m_str;

	this->m_str = CloneStr(other_.m_str);

	return *this;
}

String::~String()
{
	delete[] this->m_str;
}

size_t String::Length() const
{
	return StrLen(this->m_str);
}

const char* String::Cstr() const
{
	return this->m_str;
}

static void CopyString(char* dest_, const char* src_)
{
    char* curr_dest = (assert(NULL != dest_), dest_);

    while (*src_ != '\0')
    {
        *curr_dest = *src_;
    
		++curr_dest;
		++src_;
    }

	*curr_dest = '\0';
}

static size_t StrLen(const char* str_)
{
	size_t length = 0;

	assert(NULL != str_);

	while (str_[length] != '\0')
	{
		++length;
	}

	return length;
}

static char* CloneStr(const char* str_)
{
	size_t len = (assert(NULL != str_), StrLen(str_));
	char* new_str = new char[len + 1];

	CopyString(new_str, str_);

	return new_str;
}

}//namespace ilrd
