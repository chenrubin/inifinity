#ifndef __SINGLETON_HPP__ 
#define __SINGLETON_HPP__

#include <boost/atomic.hpp> /* fences */
#include <stdio.h> /* perror */

#include "utils.hpp" /* DEADBEEF */

namespace ilrd
{
template <typename T>
class Singleton
{
public:
    static T *GetInstance();

private:
    static boost::atomic<T *> s_instance;
    static boost::atomic<bool> s_init;

    static void ExitHandler();
    Singleton();
    ~Singleton();
};

template <typename T>
void Singleton<T>::ExitHandler()
{
    delete s_instance;
    s_instance = Deadbeef<T>();
}

template <typename T>
T *Singleton<T>::GetInstance()
{
   if (0 == s_instance.load(boost::memory_order_acquire))
    {
        bool initFalse = false;
        if (s_init.compare_exchange_strong(initFalse, true, boost::memory_order_release, boost::memory_order_acquire))
        {
            s_init.store(true, boost::memory_order_release);
            T* temp = new T;
            s_instance.store(temp, boost::memory_order_release);
            int result = std::atexit(Singleton::ExitHandler);
            if (0 != result)
            {
                perror("Registration failed: ");
            }
        }

        while (0 == s_instance.load(boost::memory_order_acquire));
    }
    
    return s_instance.load(boost::memory_order_acquire);
}


} // end of namespace
#endif