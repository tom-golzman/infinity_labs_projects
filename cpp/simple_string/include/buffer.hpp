#ifndef __ILRD_BUFFER_H__
#define __ILRD_BUFFER_H__

/************************************ Includes *************************************/
#include <algorithm>        // std::copy()

#include "buffer_data.hpp"  // class BufferData
#include "utils.hpp"	    // SUCCESS, FAIL, DEBUG_ONLY(), BAD_MEM(), NOEXCEPT, OVERRIDE

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
    BufferData<T>* m_data;
};

template <typename T>
Buffer<T>::Buffer(size_t size_): m_data(BufferData<T>::AllocateBuff(size_))
{}

template <typename T>
Buffer<T>::Buffer(const Buffer& other_): m_data(other_.m_data)
{
    m_data->Attach();
}

template <typename T>
Buffer<T>& Buffer<T>::operator=(const Buffer& other_)
{
	// handles self assignment

    // DANGER ZONE
	BufferData<T>* temp = other_.m_data;
    
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
        m_data = BAD_MEM(BufferData<T>*);
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
    return m_data->Size() / sizeof(T);
}

}//namespace ilrd

#endif //__ILRD_BUFFER_H__