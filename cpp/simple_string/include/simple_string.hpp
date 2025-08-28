#ifndef __ILRD_SIMPLE_STRING_H__
#define __ILRD_SIMPLE_STRING_H__

/************************************ Includes *************************************/
#include <cstddef>      // size_t

#include "buffer.hpp"   // class Buffer
#include "utils.hpp"	// SUCCESS, FAIL, DEBUG_ONLY(), BAD_MEM(), NOEXCEPT, OVERRIDE

namespace ilrd
{

class String;

bool operator==(const String& s1, const String& s2);
bool operator<(const String& s1, const String& s2);
bool operator>(const String& s1, const String& s2);

class String
{
public:
    // constructor
    String(const char* str_); //non explicit on purpose
    
    // generated copy ctor, assignment operator, dtor

    char& operator[](size_t index);
    char operator[](size_t index) const;
    
    size_t Length() const NOEXCEPT;
    const char* Cstr() const;

private:
    Buffer<char> m_buff;
};

}//namespace ilrd

#endif //__ILRD_SIMPLE_STRING_H__