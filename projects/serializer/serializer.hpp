// Serializer - Header file
// Last update: 1/4/2020
// Author: 
/*----------------------------------------------------------------------------*/
#ifndef __SERIALIZER_HPP__ 
#define __SERIALIZER_HPP__
/*----------------------------------------------------------------------------*/
#include "factory.hpp"
/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
template<typename T>
class Serializer: private Uncopyable
{
public:
    template<typename U>
    bool AddClass();

    void Serialize(const T& object_, std::ostream& ostream_) const;

    boost::share_ptr<T> Deserialize(std::istream& istream_) const;

private:
    Factory<T, std::string, void> m_factory;
};

/*----------------------------------------------------------------------------*/
}   // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __SERIALIZER_HPP__
