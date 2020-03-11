#ifndef __SCOPELOCK_HPP__
#define __SCOPELOCK_HPP__

#include <iostream> /* cout endl */
#include <pthread.h> /* pthread_mutex */
#include "MyUtils.hpp"

namespace ilrd
{
template <typename T>
class ScopeLock : private Uncopyable
{
public:
    explicit ScopeLock(T* mutex_);
    ~ScopeLock();

private:
    T* m_lock;
};

template <typename T>
ScopeLock<T>::ScopeLock(T* mutex_)
	: m_lock(mutex_)
{
	std::cout << "Ctor" << std::endl;
	m_lock->lock();
}

template <typename T>
ScopeLock<T>::~ScopeLock()
{
	std::cout << "Dtor" << std::endl;
	m_lock->unlock();
}
/*
template <>
ScopeLock<pthread_mutex_t>::ScopeLock(pthread_mutex_t *mutex_);
template <>
ScopeLock<pthread_mutex_t>::~ScopeLock();
*/
/**/

template <>
class ScopeLock<pthread_mutex_t> : private Uncopyable
{
public:
    explicit ScopeLock(pthread_mutex_t* mutex_);
    ~ScopeLock();

private:
    pthread_mutex_t* m_lock;
};
}
#endif