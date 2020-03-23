#ifndef __SINGLETON_HPP__ 
#define __SINGLETON_HPP__

namespace ilrd
{
template <typename T>
class Singleton
{
public:
    static Singleton *GetInstance();
    
    Singleton::~Singleton();

private:
    static Singleton *m_instance;

    Singleton();
};

template <typename T>
Singleton<T> *Singleton<T>::m_instance = 0;

template <typename T>
Singleton<T>::Singleton()
{}

Singleton<T>::~Singleton()
{
    delete m_
}

template <typename T>
Singleton<T> *Singleton<T>::GetInstance()
{
    if (0 == m_instance)
    {
        m_instance = new Singleton;
    }

    return m_instance;
}


} // end of namespace
#endif