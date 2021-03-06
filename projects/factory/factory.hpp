// Factory - Header file
// Last update: 31/03/2020
// Author: Chen
// Reviewer: Ran
/*----------------------------------------------------------------------------*/
#ifndef __FACTORY_HPP__ 
#define __FACTORY_HPP__
/*----------------------------------------------------------------------------*/
#include <boost/function.hpp>	// boost::function
#include <map> 					// std::map

#include "MyUtils.hpp"			// class Uncopyable
/*----------------------------------------------------------------------------*/
namespace ilrd
{
namespace factory
{
/*----------------------------------------------------------------------------*/
// K is the key type by which the factory knows which 
// 
template <typename T, typename K, typename P>
class Factory : private Uncopyable
{
public:
	// If new key replaced an old one ,return value = false
	// If new key was immediatly inserted return value = true
	// throws bad::alloc<stddef.h>
	bool AddClass(const K& key_, 
			boost::function<boost::shared_ptr<T>(P)> createFunc_);
	
	//	throws BadCreate and BadKey
	//  Create an object according to "receipe"
	// receives key and param
	// returns shared pointer of object from type T
	boost::shared_ptr<T> Create(const K& key_, P param_) const NOEXCEPT;
	
	// 2nd phase: unique_ptr c++11, 3rd? args...

private:
	typedef boost::function<boost::shared_ptr<T>(P)> func_t;
	typedef typename std::map<K, boost::function<boost::shared_ptr<T>(P)> >::iterator map_iterator_t; 
	// A map of key and function (receipe for creating an object of type T)
	//std::map<K, boost::function<boost::shared_ptr<T>(P)> > m_keyFuncPairs/*AddTaskWrite_t*/;
	mutable std::map<K, boost::function<boost::shared_ptr<T>(P)> > m_keyFuncPairs;
};
/*----------------------------------------------------------------------------*/
class BadCreate : public std::runtime_error
{
public:
	explicit BadCreate();
};

class BadKey : public std::logic_error
{
public:
	explicit BadKey();
};

template <typename T, typename K, typename P>
bool Factory<T,K,P>::AddClass(const K& key_, func_t createFunc_)
{
	std::pair<map_iterator_t, bool> res;
									//boost::function<boost::shared_ptr<T>(P)>
	/*res = */m_keyFuncPairs.insert(std::pair<K, func_t>(key_, createFunc_));
	/*if (res.second == false)
	{
		m_keyFuncPairs[key_] = createFunc_;
	}
*/
	return res.second;
}

template <typename T, typename K, typename P>
boost::shared_ptr<T> Factory<T,K,P>::Create(const K& key_, P param_) const
{
	map_iterator_t iter;
	boost::shared_ptr<T> res;
	iter = m_keyFuncPairs.find(key_);

	if (iter == m_keyFuncPairs.end())
	{
		throw BadKey();
	}

	try
	{
		res = iter->second(param_);
	}
	catch(const std::exception& e)
	{
		throw BadCreate();
	}
	
	return res;
}	
/*----------------------------------------------------------------------------*/
} // namespace factory
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __FACTORY_HPP__