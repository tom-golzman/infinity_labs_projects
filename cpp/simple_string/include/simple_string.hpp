#ifndef __ILRD_SIMPLE_STRING_H__
#define __ILRD_SIMPLE_STRING_H__

/************************************ Includes *************************************/
#include <cstddef> // size_t

namespace ilrd
{
class String
{
public:
    // constructor from const char*
    String(const char* str_);

    // copy ctor
    String(const String& other_);

    // assignment operator
    String& operator=(const String& other_);

    bool operator==(const String& other_) const;

    bool operator<(const String& other_) const;

    bool operator>(const String& other_) const;

    // dtor
    ~String();

    size_t Length() const;
    const char* Cstr() const;

private:
    const char* m_str;
    static const char* CloneStr(const char* str_);
};

void Foo(String str);
String Bla();

}//namespace ilrd

#endif //__ILRD_SIMPLE_STRING_H__
