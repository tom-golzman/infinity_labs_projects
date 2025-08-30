#ifndef __ILRD_BUFFER_DATA_HPP__
#define __ILRD_BUFFER_DATA_HPP__

/*************************************** Includes ****************************************/
#include <cassert>      // assert
#include <cstddef>      // size_t
#include <new>          // operator new, operator delete

#include "utils.hpp"	// SUCCESS, FAIL, DEBUG_ONLY(), BAD_MEM(), NOEXCEPT, OVERRIDE

/************************************ namespace ilrd *************************************/

namespace ilrd
{

class BufferData
{
public:
    inline static BufferData* AllocateBuff(size_t size_);
    inline void Attach();
    inline void Detach();
    
    inline char* GetBuff();
    inline const char* GetBuff() const;
    
    inline size_t Size() const NOEXCEPT;
    
private:
    size_t m_size;
    size_t m_rc;
    char m_arr[1];
    
    inline explicit BufferData(size_t size_);
    inline ~BufferData() NOEXCEPT;
    
    BufferData(const BufferData&);
    BufferData& operator=(const BufferData&);
};

inline BufferData::BufferData(size_t size_): m_size(size_), m_rc(1)
{}

inline BufferData::~BufferData() NOEXCEPT
{
    DEBUG_ONLY(
        m_size = BAD_MEM(size_t);
        m_rc = BAD_MEM(size_t);
    );
}

//static
inline BufferData* BufferData::AllocateBuff(size_t size_)
{
    size_t total_size = sizeof(BufferData) + ((size_ - 1) * sizeof(char));
    void* ret = operator new(total_size);

    return new(ret) BufferData(size_);
}

inline void BufferData::Attach()
{
    ++m_rc;
}

inline void BufferData::Detach()
{
    assert(m_rc > 0);

    --m_rc;

    if(0 == m_rc)
    {
        delete this;
    }
}

inline char* BufferData::GetBuff()
{
    return m_arr;
}

inline const char* BufferData::GetBuff() const
{
    return m_arr;
}

inline size_t BufferData::Size() const NOEXCEPT
{
    return m_size;
}

}//namespace ilrd

#endif //__ILRD_BUFFER_DATA_HPP__
