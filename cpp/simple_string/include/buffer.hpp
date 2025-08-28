#ifndef __ILRD_BUFFER_H__
#define __ILRD_BUFFER_H__

/************************************ Includes *************************************/
#include <algorithm>    // std::copy()

#include "utils.hpp"	// SUCCESS, FAIL, DEBUG_ONLY(), BAD_MEM(), NOEXCEPT, OVERRIDE

namespace ilrd
{

template <typename T>
T* AllocAndCopy (T* other_arr_, size_t other_size);

template <typename T>
class Buffer
{
public:
    explicit Buffer(size_t size_);
    Buffer(const Buffer& other_);

    Buffer& operator=(const Buffer& other_);

    ~Buffer() NOEXCEPT;

    const T* GetR() const;
    T* GetW();

    size_t Size() const;

private:
    T* m_arr;
    size_t m_size;
};

template <typename T>
T* AllocAndCopy(T* other_arr_, size_t other_size)
{
    T* new_arr = new T[other_size];

    try
    {
        std::copy(other_arr_, other_arr_ + other_size, new_arr);
    }

    catch(...)
    {
        delete[] new_arr;
    }

    return new_arr;
}

template <typename T>
Buffer<T>::Buffer(size_t size_): m_arr(new T[size_]), m_size(size_)
{}

template <typename T>
Buffer<T>::Buffer(const Buffer& other_): m_arr(AllocAndCopy(other_.m_arr, other_.m_size)), m_size(other_.m_size)
{}

template <typename T>
Buffer<T>& Buffer<T>::operator=(const Buffer& other_)
{
	// handles self assignment

    // DANGER ZONE
	T* temp = AllocAndCopy(other_.m_arr, other_.m_size);

	m_arr = temp;
    m_size = other_.m_size;
    
    // end of DANGER ZONE
	
    return *this;
}

template <typename T>
Buffer<T>::~Buffer() NOEXCEPT
{
    delete[] m_arr;

    DEBUG_ONLY(
        m_arr = BAD_MEM(T*);
        m_size = 0;
    );
}

template <typename T>
const T* Buffer<T>::GetR() const
{
    return m_arr;
}

template <typename T>
T* Buffer<T>::GetW()
{
    return m_arr;
}

template <typename T>
size_t Buffer<T>::Size() const
{
    return m_size;
}

}//namespace ilrd

#endif //__ILRD_BUFFER_H__