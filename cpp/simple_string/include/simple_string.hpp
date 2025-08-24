#ifndef __ILRD_SIMPLE_STRING_H__
#define __ILRD_SIMPLE_STRING_H__

/************************************ Includes *************************************/
#include <cstddef> // size_t

namespace ilrd
{

class String;

bool operator==(const String& string1, const String& string2);
bool operator<(const String& string1, const String& string2);
bool operator>(const String& string1, const String& string2);

class String
{
public:
    // constructor from const char*
    String(const char* str_); //non explicit on purpose

    // copy ctor
    String(const String& other_);

    // assignment operator
    String& operator=(const String& other_);

    // dtor
    ~String();

    size_t Length() const;
    const char* Cstr() const;

private:
    const char* m_str;
    static const char* CloneStr(const char* str_);
};

}//namespace ilrd

#endif //__ILRD_SIMPLE_STRING_H__
