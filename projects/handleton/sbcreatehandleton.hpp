#ifndef __CREATE_HANDLETONE_HPP__
#define __CREATE_HANDLETONE_HPP__
#include <boost/atomic.hpp> /* fences */

#include "handleton.hpp"

namespace ilrd
{
#define CREATE_HANDLETON(T)\
template <>\
boost::atomic<T *> Singleton<T>::s_instance(0);\
template <>\
boost::atomic<bool> Singleton<T>::s_init(0);\
}
#endif