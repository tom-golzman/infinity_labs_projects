#ifndef __ILRD_BUFFER_H__
#define __ILRD_BUFFER_H__

/************************************ Includes *************************************/
#include <algorithm>    // std::copy()

#include "utils.hpp"	// SUCCESS, FAIL, DEBUG_ONLY(), BAD_MEM(), NOEXCEPT, OVERRIDE

namespace ilrd
{

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

    size_t Size() const NOEXCEPT;

private:
    T* m_arr;
    size_t m_size;

    static T* AllocAndCopy(const Buffer<T>& buff_);
};

template <typename T>
Buffer<T>::Buffer(size_t size_): m_arr(new T[size_]), m_size(size_)
{}

template <typename T>
Buffer<T>::Buffer(const Buffer& other_): m_arr(AllocAndCopy(other_)), m_size(other_.m_size)
{}

template <typename T>
Buffer<T>& Buffer<T>::operator=(const Buffer& other_)
{
	// handles self assignment

    // DANGER ZONE
	T* temp = AllocAndCopy(other_);
    
    delete[] m_arr;
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
size_t Buffer<T>::Size() const NOEXCEPT
{
    return m_size;
}

//static
template <typename T>
T* Buffer<T>::AllocAndCopy(const Buffer<T>& buff_)
{
    T* ret = new T[buff_.m_size];

    try
    {
        std::copy(buff_.m_arr, buff_.m_arr + buff_.m_size, ret);
    }

    catch(...)
    {
        delete[] ret;

        throw;
    }

    return ret;
}

}//namespace ilrd

#endif //__ILRD_BUFFER_H__