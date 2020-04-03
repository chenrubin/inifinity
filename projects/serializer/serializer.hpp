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
/*
Serializer takes a base class and convert it into a stream that can be then 
sent to another station and then deserialized. This is similar to 
Jason, XML...
In order for this to work user must have operators >> and << inside its T
class. These operators should be able to convert object to ostream and istream 
to object respectively. This ostream/istream will be given by user 
*/
/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
template<typename T>
class Serializer : private Uncopyable
{
public:
    // Throws BadCreate
    // Add derive class to the serializer (type U)
    // returns false if it fails to add the class
    template<typename U>
    bool AddClass();

    // turns object members into ostream supplied by user
    // Have to be called after AddClass so class "recepie" will be known 
    void Serialize(const T& object_, std::ostream& ostream_) const NOEXCEPT;

    // Throws BadCreate or BadKey
    // Turns the istream su0pllied by user back to an object which is a 
    // derived class of T
    boost::shared_ptr<T> Deserialize(std::istream& istream_) const;

private:
    factory::Factory<T, std::string, std::istream&> m_factory;
    
    template<typename U>
    static boost::shared_ptr<T> AddObjectIMP(std::istream& istream_);
};

template<typename T>
void Serializer<T>::Serialize(const T& object_, std::ostream& ostream_) const
{
    ostream_ << typeid(object_).name() << "\n";
    ostream_ << object_;
}

template<typename T>
boost::shared_ptr<T> Serializer<T>::Deserialize(std::istream& istream_) const NOEXCEPT
{
    std::string object_name;

    istream_ >> object_name;
    boost::shared_ptr<T> obj = m_factory.Create(object_name, istream_);
    
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
boost::shared_ptr<T> Serializer<T>::AddObjectIMP(std::istream& istream_)
{
    T *obj = new U;
    istream_ >> *obj;
    boost::shared_ptr<T> shared_ptr(obj);

    return shared_ptr;
}

/*----------------------------------------------------------------------------*/
}   // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __SERIALIZER_HPP__
