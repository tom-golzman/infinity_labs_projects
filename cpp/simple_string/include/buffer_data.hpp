#ifndef __ILRD_BUFFER_DATA_HPP__
#define __ILRD_BUFFER_DATA_HPP__

/*************************************** Includes ****************************************/
#include <cassert>      // assert
#include <cstddef>      // size_t
#include <new>          // operator new, operator delete

#include "utils.hpp"	// SUCCESS, FAIL, DEBUG_ONLY(), BAD_MEM(), NOEXCEPT, OVERRIDE

namespace ilrd
{

template <typename T>
class BufferData
{
public:
    static BufferData<T>* AllocateBuff(size_t size_);
    void Attach();
    void Detach() NOEXCEPT;
    
    T* GetBuff();
    const T* GetBuff() const;
    BufferData<T>* GetUniqueBuff();
    
    size_t Size() const NOEXCEPT;
    
private:
    size_t m_size;
    size_t m_rc;
    T m_arr[1];
    
    explicit BufferData(size_t size_);
    ~BufferData() NOEXCEPT;
    
    BufferData(const BufferData<T>&);
    BufferData<T>& operator=(const BufferData<T>&);

    void DestroyBuff(BufferData<T>* buff_);
};

//=================== Implementation ===================//

template <typename T>
BufferData<T>::BufferData(size_t size_): m_size(size_), m_rc(1)
{}

template <typename T>
BufferData<T>::~BufferData() NOEXCEPT
{
    DEBUG_ONLY(
        m_size = BAD_MEM(size_t);
        m_rc = BAD_MEM(size_t);
    );
}

// static
template <typename T>
BufferData<T>* BufferData<T>::AllocateBuff(size_t size_)
{
    size_t total_size = sizeof(BufferData) + ((size_ - 1) * sizeof(T));
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

template <typename T>
void BufferData<T>::DestroyBuff(BufferData<T>* buff_)
{
    assert(buff_);

    buff_->~BufferData();

    operator delete(buff_);
}

template <typename T>
void BufferData<T>::Attach()
{
    ++m_rc;
}

template <typename T>
void BufferData<T>::Detach() NOEXCEPT
{
    assert(m_rc > 0);

    --m_rc;

    if(0 == m_rc) //buffer isn't shared
    {
        DestroyBuff(this);
    }
}

template <typename T>
T* BufferData<T>::GetBuff()
{
    return m_arr;
}

template <typename T>
const T* BufferData<T>::GetBuff() const
{
    return m_arr;
}

template <typename T>
BufferData<T>* BufferData<T>::GetUniqueBuff()
{
    if(1 == m_rc) //buffer isn't shared
    {
        return this;
    }

    BufferData<T>* ret = AllocateBuff(m_size);

    try
    {
        std::copy(GetBuff(), GetBuff() + m_size, ret->GetBuff());
    }

    catch(...)
    {
        DestroyBuff(ret);
        throw;
    }

    return ret;
}

template <typename T>
size_t BufferData<T>::Size() const NOEXCEPT
{
    return m_size;
}

} // namespace ilrd

#endif //__ILRD_BUFFER_DATA_HPP__