/**
	Written By: Tom Golzman
	Date: 24/08/2025
**/

#ifndef __ILRD_BUFFER_H__
#define __ILRD_BUFFER_H__

#include <algorithm>
#include <cassert>
#include <new>

#include "utils.hpp"

namespace ilrd
{

template <typename T>
class Buffer
{
public:
    explicit Buffer(size_t size_);
    Buffer(const Buffer& other_) NOEXCEPT;

    Buffer& operator=(const Buffer& other_);

    ~Buffer() NOEXCEPT;

    const T* GetR() const;
    T* GetW();

    size_t Size() const NOEXCEPT;

private:
    class BufferData
    {
        public:
            static BufferData* AllocateBuff(size_t size_);
            void Attach() NOEXCEPT;
            void Detach() NOEXCEPT;
            
            T* GetBuff();
            const T* GetBuff() const;
            BufferData* GetUniqueBuff();
            
            size_t Size() const NOEXCEPT;
            
        private:
            size_t m_size;
            size_t m_rc;
            T m_arr[1];
            
            explicit BufferData(size_t size_);
            ~BufferData() NOEXCEPT;

            void DestroyBuff(BufferData* buff_) NOEXCEPT;
            
            // disabled copy constructor and assigment operator
            BufferData(const BufferData&);
            BufferData& operator=(const BufferData&);

    }; //class BufferData

    BufferData* m_data;

}; //class Buffer

template <typename T>
Buffer<T>::Buffer(size_t size_): m_data(Buffer<T>::BufferData::AllocateBuff(size_))
{}

template <typename T>
Buffer<T>::Buffer(const Buffer& other_) NOEXCEPT : m_data(other_.m_data)
{
    m_data->Attach();
}

template <typename T>
Buffer<T>& Buffer<T>::operator=(const Buffer& other_)
{
	// handles self assignment

    // DANGER ZONE
	BufferData* temp = other_.m_data;
    
    temp->Attach();

    m_data->Detach();
    
    m_data = temp;

    // end of DANGER ZONE
	
    return *this;
}

template <typename T>
Buffer<T>::~Buffer() NOEXCEPT
{
    m_data->Detach();

    DEBUG_ONLY(
        m_data = BAD_MEM(BufferData*);
    );
}

template <typename T>
const T* Buffer<T>::GetR() const
{
    return m_data->GetBuff();
}

template <typename T>
T* Buffer<T>::GetW()
{
    m_data = m_data->GetUniqueBuff();

    return m_data->GetBuff();
}

template <typename T>
size_t Buffer<T>::Size() const NOEXCEPT
{
    return m_data->Size();
}

template <typename T>
Buffer<T>::BufferData::BufferData(size_t size_): m_size(size_), m_rc(1)
{}

template <typename T>
Buffer<T>::BufferData::~BufferData() NOEXCEPT
{
    DEBUG_ONLY(
        m_rc = BAD_MEM(size_t);
    );
}

// static
template <>
typename Buffer<char>::BufferData* Buffer<char>::BufferData::AllocateBuff(size_t size_)
{
    size_t total_size = sizeof(BufferData) + ((size_ - 1) * sizeof(char));
    void* ret = NULL;
    
    try
    {
        ret = operator new(total_size);
        return new(ret) BufferData(size_);
    }

    catch (...)
    {
        operator delete(ret);
        throw;
    }
}

template <>
void Buffer<char>::BufferData::DestroyBuff(BufferData* buff_) NOEXCEPT
{
    assert(buff_);

    buff_->~BufferData();

    operator delete(buff_);
}

template <typename T>
void Buffer<T>::BufferData::Attach() NOEXCEPT
{
    ++m_rc;
}

template <typename T>
void Buffer<T>::BufferData::Detach() NOEXCEPT
{
    assert(m_rc > 0);

    --m_rc;

    if(0 == m_rc) //buffer isn't shared
    {
        DestroyBuff(this);
    }
}

template <typename T>
T* Buffer<T>::BufferData::GetBuff()
{
    return m_arr;
}

template <typename T>
const T* Buffer<T>::BufferData::GetBuff() const
{
    return m_arr;
}

template <>
typename Buffer<char>::BufferData* Buffer<char>::BufferData::GetUniqueBuff()
{
    if(1 == m_rc) //buffer isn't shared
    {
        return this;
    }

    BufferData* ret = AllocateBuff(m_size);
    Detach();
    std::copy(m_arr, m_arr + m_size, ret->m_arr);

    return ret;
}

template <typename T>
size_t Buffer<T>::BufferData::Size() const NOEXCEPT
{
    return m_size;
}

}//namespace ilrd

#endif //__ILRD_BUFFER_H__