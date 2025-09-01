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
    class BufferData;
    BufferData* m_data;

}; //class Buffer

template <typename T>
class Buffer<T>::BufferData
{
public:
    static BufferData* AllocateBuff(size_t size_);
    void Attach() NOEXCEPT;
    void Detach() NOEXCEPT;
    
    T* GetArr();
    BufferData* GetUniqueBuff();
    
    size_t Size() const NOEXCEPT;
    
private:
    const size_t m_size;
    size_t m_rc;
    T m_arr[1]; //flexible array. this field has to be the last one.
    
    explicit BufferData(size_t size_) NOEXCEPT; //private Ctor used by AllocateBuff()
    ~BufferData() NOEXCEPT; //private Dtor prevents clients from using delete

    void DestroyBuff(BufferData* buff_) NOEXCEPT;
    
    // disabled copy constructor and assigment operator. DON'T IMPLEMENT
    BufferData(const BufferData&);
    BufferData& operator=(const BufferData&);

}; //class BufferData

// static
template <>
typename Buffer<char>::BufferData* Buffer<char>::BufferData::AllocateBuff(size_t size_);
template <>
void Buffer<char>::BufferData::DestroyBuff(BufferData* buff_) NOEXCEPT;
template <>
typename Buffer<char>::BufferData* Buffer<char>::BufferData::GetUniqueBuff();

template <typename T>
Buffer<T>::Buffer(size_t size_): m_data(BufferData::AllocateBuff(size_))
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
    return m_data->GetArr();
}

template <typename T>
T* Buffer<T>::GetW()
{
    m_data = m_data->GetUniqueBuff();

    return m_data->GetArr();
}

template <typename T>
size_t Buffer<T>::Size() const NOEXCEPT
{
    return m_data->Size();
}

template <typename T>
Buffer<T>::BufferData::BufferData(size_t size_) NOEXCEPT: m_size(size_), m_rc(1)
{}

template <typename T>
Buffer<T>::BufferData::~BufferData() NOEXCEPT
{
    DEBUG_ONLY(
        m_rc = 0;
        const_cast<size_t&>(m_size) = 0;
    );
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
T* Buffer<T>::BufferData::GetArr()
{
    return m_arr;
}

template <typename T>
size_t Buffer<T>::BufferData::Size() const NOEXCEPT
{
    return m_size;
}

}//namespace ilrd

#endif //__ILRD_BUFFER_H__