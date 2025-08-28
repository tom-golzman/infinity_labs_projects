#ifndef __ILRD_BUFFER_H__
#define __ILRD_BUFFER_H__

/************************************ Includes *************************************/
#include <cstddef> // size_t

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

private:
    T* m_arr;
    size_t m_size;
};

Buffer::Buffer(size_t size_): m_arr(new T[size_]), m_size(size_)
{}

Buffer::Buffer(const Buffer& other_): m_arr(new T[other_.m_size]), m_size(other.m_size)
{
    memcopy(m_arr, other_.m_arr, other_.m_size * sizeof(T));
}

Buffer& Buffer::operator=(const Buffer& other_)
{
	// handles self assignment

    // DANGER ZONE
	T* temp = new T[other_.m_size];

    memcpy(temp, other_.m_arr, other_.m_size * sizeof(T));
	
	delete[] m_arr;

	m_arr = temp;
    m_size = other_.m_size;
    
    // end of DANGER ZONE
	
    return *this;
}

Buffer::~Buffer() NOEXCEPT
{
    delete[] m_arr;

    DEBUG_ONLY(
        m_arr = BAD_MEM(T*);
    );
}

const T* GetR() const
{
    return m_arr;
}

T* GetW()
{
    return m_arr;
}

}//namespace ilrd

#endif //__ILRD_BUFFER_H__