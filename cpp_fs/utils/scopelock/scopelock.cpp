//#include <mutex> /* std::mutex */
#include "scopelock.hpp"

namespace ilrd
{
/*template <>
ScopeLock<pthread_mutex_t>::ScopeLock(pthread_mutex_t *mutex_)
	: m_lock(mutex_)
{
	pthread_mutex_lock(mutex_);
}

template <>
ScopeLock<pthread_mutex_t>::~ScopeLock()
{
	pthread_mutex_unlock(m_lock);
}*/

ScopeLock<pthread_mutex_t>::ScopeLock(pthread_mutex_t *mutex_)
	: m_lock(mutex_)
{
	pthread_mutex_lock(mutex_);
}

ScopeLock<pthread_mutex_t>::~ScopeLock()
{
	pthread_mutex_unlock(m_lock);
}
}