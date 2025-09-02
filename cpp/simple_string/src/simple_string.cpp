/**
	Written By: Tom Golzman
	Date: 21/08/2025
**/

#include <cassert>
#include <cstring>
#include <cstdlib>

#include "simple_string.hpp"

namespace ilrd
{

String::String(const char* str_): m_buff(strlen(str_) + 1)
{
	char* buff = m_buff.GetW();

	std::copy(str_, str_ + m_buff.Size(), buff);
}

String::CharProxy String::operator[](size_t index_)
{
	assert(index_ < Length());

	return CharProxy(this, index_);
}

char String::operator[](size_t index_) const NOEXCEPT
{
	assert(index_ < Length());

    return m_buff.GetR()[index_];
}

void String::SetAt(size_t index_, const char c_)
{
    assert(index_ < Length());

	char* buff = m_buff.GetW();
    buff[index_] = c_;
}

char String::GetAt(size_t index_) const NOEXCEPT
{
    assert(index_ < Length());

    return m_buff.GetR()[index_];
}

size_t String::Length() const NOEXCEPT
{
	return m_buff.Size() - 1;
}

const char* String::Cstr() const
{
	return m_buff.GetR();
}

String::CharProxy::CharProxy(String* str_, size_t index_): m_str(str_), m_index(index_)
{
    assert(index_ < str_->Length());
}

char& String::CharProxy::operator=(const char c_)
{
    m_str->SetAt(m_index, c_);
    
    return m_str->m_buff.GetW()[m_index];
}

char& String::CharProxy::operator=(const CharProxy& other_)
{
    m_str->SetAt(m_index, char(other_));
    
    return m_str->m_buff.GetW()[m_index];
}

char* String::CharProxy::operator&()
{
	return &(m_str->m_buff.GetW()[m_index]);
}

String::CharProxy::operator char() const
{
    return m_str->m_buff.GetR()[m_index];
}

bool operator==(const String& s1_, const String& s2_)
{
	return !(strcmp(s1_.Cstr(), s2_.Cstr()));
}

bool operator<(const String& s1_, const String& s2_)
{
	return (strcmp(s1_.Cstr(), s2_.Cstr()) < 0);
}

bool operator>(const String& s1_, const String& s2_)
{
	return (strcmp(s1_.Cstr(), s2_.Cstr()) > 0);
}

}//namespace ilrd