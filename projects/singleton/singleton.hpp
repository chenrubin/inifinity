#ifndef __SINGLETON_HPP__ 
#define __SINGLETON_HPP__

#include <boost/atomic.hpp> /* fences */
#include <stdio.h> /* perror */
#include <iostream> /* perror */

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
boost::atomic<T *> Singleton<T>::s_instance(static_cast<T*>(0));

template <typename T>
boost::atomic<bool> Singleton<T>::s_init(0);

template <typename T>
void Singleton<T>::ExitHandler()
{
    delete s_instance;
    s_instance = Deadbeef<T>();
}

template <typename T>
T *Singleton<T>::GetInstance()
{
    std::cout << "Inside Singleton GetInsytance\n";
    if (0 == s_instance.load(boost::memory_order_acquire))
    {
        std::cout << "Inside Singleton GetInsytance outer if\n";
        bool initFalse = false;
        if (s_init.compare_exchange_strong(initFalse, true, boost::memory_order_release, boost::memory_order_acquire))
        {
            std::cout << "Inside Singleton GetInsytance inner if\n";
            s_init.store(true, boost::memory_order_release);
            std::cout << "Inside Singleton GetInsytance inner if after store!!\n";
            T* temp = new T; // WTF
            std::cout << "Inside Singleton GetInsytance inner if after temp = new\n";
            s_instance.store(temp, boost::memory_order_release);
            std::cout << "Inside Singleton GetInsytance inner if store of temp\n";
            int result = std::atexit(Singleton::ExitHandler);
            if (0 != result)
            {
                perror("Registration failed: ");
            }
        }
        std::cout << "Inside Singleton GetInsytance after inner if\n";
        while (0 == s_instance.load(boost::memory_order_acquire));
    }
    std::cout << "Inside Singleton GetInsytance after outer if\n";
    return s_instance.load(boost::memory_order_acquire);
}


} // end of namespace
#endif