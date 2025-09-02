/**
	Written By: Tom Golzman
	Date: 24/08/2025
**/

#include <cstddef>

#include "buffer.hpp"

namespace ilrd
{

template <>
Buffer<char>::BufferData* Buffer<char>::BufferData::AllocateBuff(size_t size_)
{
    assert(size_ > 0);
    
    size_t total_size = offsetof(Buffer<char>::BufferData, m_arr) + (size_ * sizeof(char));
    void* ret = operator new(total_size);
 
    return new(ret) BufferData(size_);
}

template <>
void Buffer<char>::BufferData::DestroyBuff(Buffer<char>::BufferData* buff_) NOEXCEPT
{
    assert(buff_);

    buff_->~BufferData();

    operator delete(buff_);
}

template <>
Buffer<char>::BufferData* Buffer<char>::BufferData::GetUniqueBuff()
{
    if(1 == m_rc) //buffer isn't shared
    {
        return this;
    }

    Buffer<char>::BufferData* ret = AllocateBuff(m_size);
    std::copy(m_arr, m_arr + m_size, ret->m_arr);
    Detach();

    return ret;
}

} //namespace ilrd