#ifndef __ILRD_SIMPLE_STRING_H__
#define __ILRD_SIMPLE_STRING_H__

#include <cstddef>

#include "buffer.hpp"
#include "utils.hpp"

namespace ilrd
{

class String;
class CharProxy;

bool operator==(const String& s1_, const String& s2_);
bool operator<(const String& s1_, const String& s2_);
bool operator>(const String& s1_, const String& s2_);

class String
{
private:
    class CharProxy;

public:
    String(const char* str_); //non explicit on purpose
    
    // generated copy ctor, assignment operator, dtor

    char operator[](size_t index_) const NOEXCEPT;
    CharProxy operator[](size_t index_);

    void SetAt(size_t index_, const char c_);
    char GetAt(size_t index_) const NOEXCEPT;

    size_t Length() const NOEXCEPT;
    const char* Cstr() const;

private:
    Buffer<char> m_buff;

}; //class String

class String::CharProxy
{
public:
    explicit CharProxy(String* str_, size_t index_);
    CharProxy(const CharProxy& other_);

    char& operator=(const char c_);
    char& operator=(const CharProxy& other_);
    char* operator&();
    operator char() const;

private:
    String* const m_str;
    const size_t m_index;

}; //class CharProxy

}//namespace ilrd

#endif //__ILRD_SIMPLE_STRING_H__