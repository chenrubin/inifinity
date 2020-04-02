// Serializer - Header file
// Last update: 2/4/2020
// Author: Chen
// Reviewer: Ofer
/*----------------------------------------------------------------------------*/
#ifndef __SERIALIZER_HPP__ 
#define __SERIALIZER_HPP__
/*----------------------------------------------------------------------------*/
#include <boost/shared_ptr.hpp> /* shared_ptr */
#include <boost/function.hpp>   /* boost function */
#include <cstring>              /* memcpy */
#include <typeinfo>             /* typeid */

#include "factory.hpp"
#include "MyUtils.hpp"
/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
template<typename T>
class Serializer : private Uncopyable
{
public:
    // Throws Bad_alloc
    template<typename U>
    bool AddClass();

    void Serialize(const T& object_, std::ostream& ostream_) const NOEXCEPT;

    // Throws Bad_alloc
    boost::shared_ptr<T> Deserialize(std::istream& istream_) const;

private:
    factory::Factory<T, std::string, int> m_factory;
    
    template<typename U>
    static boost::shared_ptr<T> AddObjectIMP(int i);
};

template<typename T>
void Serializer<T>::Serialize(const T& object_, std::ostream& ostream_) const
{
    ostream_ << typeid(object_).name() << "\n";
    object_.operator>>(ostream_);
}

template<typename T>
boost::shared_ptr<T> Serializer<T>::Deserialize(std::istream& istream_) const NOEXCEPT
{
    std::string object_name;

    istream_ >> object_name;
    boost::shared_ptr<T> obj = m_factory.Create(object_name, 1);
    *obj << istream_;
    
    return obj;
}

template <typename T>
template <typename U>
bool Serializer<T>::AddClass()
{
    std::string object_name = typeid(U).name();

    return (m_factory.AddClass(object_name, Serializer<T>::AddObjectIMP<U>));
}

template <typename T>
template <typename U>
boost::shared_ptr<T> Serializer<T>::AddObjectIMP(int i)
{
    boost::shared_ptr<T> shared_ptr(new U());

    return shared_ptr;
}

/*----------------------------------------------------------------------------*/
}   // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __SERIALIZER_HPP__
