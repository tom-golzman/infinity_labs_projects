#ifndef __ILRD_SIMPLE_STRING_H__
#define __ILRD_SIMPLE_STRING_H__

#include <cstddef>

#include "buffer.hpp"
#include "utils.hpp"

namespace ilrd
{

class String;

bool operator==(const String& s1_, const String& s2_);
bool operator<(const String& s1_, const String& s2_);
bool operator>(const String& s1_, const String& s2_);

class String
{
public:
    String(const char* str_); //non explicit on purpose
    
    // generated copy ctor, assignment operator, dtor

    char operator[](size_t index_) const;
    void SetAt(size_t index_, const char c_);
    char GetAt(size_t index_) const;

    size_t Length() const NOEXCEPT;
    const char* Cstr() const;

private:
    class CharProxy
    {
        public:
            CharProxy(size_t index_, Buffer<char>& buff_);
            CharProxy(const CharProxy& other_);
            
            CharProxy& operator=(const char c_);
            operator char() const;

        private:
            const size_t m_index;
            Buffer<char>& m_proxy_buff;

    };

    Buffer<char> m_buff;

public:
    CharProxy operator[](size_t index_);
    
};

}//namespace ilrd

#endif //__ILRD_SIMPLE_STRING_H__