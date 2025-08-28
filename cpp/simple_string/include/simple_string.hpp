#ifndef __ILRD_SIMPLE_STRING_H__
#define __ILRD_SIMPLE_STRING_H__

/************************************ Includes *************************************/
#include <cstddef> // size_t

#include "utils.hpp"	// SUCCESS, FAIL, DEBUG_ONLY(), BAD_MEM(), NOEXCEPT, OVERRIDE

namespace ilrd
{

class String;

bool operator==(const String& s1, const String& s2);
bool operator<(const String& s1, const String& s2);
bool operator>(const String& s1, const String& s2);

class StringManager
{
public:
    // constructor
    StringManager(const char* str_); //non explicit on purpose
    //TODO: i think this constructor can be explicit
    
    // copy ctor
    StringManager(const StringManager& other_);
    
    // assignment operator
    StringManager& operator=(const StringManager& other_);
    
    char& operator[](size_t index) const;

    // dtor
    ~StringManager() NOEXCEPT;

    size_t Length() const;
    const char* Cstr() const;

private:
    char* m_str;
    static char* StrDup(const char* str_);
};

class String
{
public:
    // constructor
    String(const char* str_); //non explicit on purpose
    
    // copy ctor
    String(const String& other_);
    
    // assignment operator
    String& operator=(const String& other_);
    
    // dtor
    ~String() NOEXCEPT
    {}
    
    char& operator[](size_t index) const;
    
    size_t Length() const;
    const char* Cstr() const;

private:
    StringManager m_data;
};

}//namespace ilrd

#endif //__ILRD_SIMPLE_STRING_H__
