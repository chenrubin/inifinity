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

#define MAP_ITERATOR typename std::map<K, boost::function<boost::shared_ptr<T>(P)> >::iterator 
#define FUNC boost::function<boost::shared_ptr<T>(P)>
/*----------------------------------------------------------------------------*/
namespace ilrd
{
namespace factory
{
/*----------------------------------------------------------------------------*/
template <typename T, typename K, typename P>
class Factory : private Uncopyable
{
public:
	// If new key replaced an old one ,return value = false
	// If new key was immediatly inserted return value = true
	bool AddClass(const K& key_, 
			boost::function<boost::shared_ptr<T>(P)> createFunc_);
	
	//	throws BadCreate and BadKey
	boost::shared_ptr<T> Create(const K& key_, P param_) const NOEXCEPT; 
	
	// 2nd phase: unique_ptr c++11, 3rd? args...

private:
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
bool Factory<T,K,P>::AddClass(const K& key_, 
			  		   boost::function<boost::shared_ptr<T>(P)> createFunc_)
{
	std::pair<MAP_ITERATOR, bool> res;
	res = m_keyFuncPairs.insert(std::pair<K, FUNC>(key_, createFunc_));
	if (res.second == false)
	{
		m_keyFuncPairs[key_] = createFunc_;
	}

	return res.second;
}

template <typename T, typename K, typename P>
boost::shared_ptr<T> Factory<T,K,P>::Create(const K& key_, P param_) const
{
	MAP_ITERATOR iter;
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

factory::BadCreate::BadCreate()
	: std::runtime_error("Bad create")
{}

factory::BadKey::BadKey()
	: std::logic_error("Bad key")
{}	
/*----------------------------------------------------------------------------*/
} // namespace factory
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __FACTORY_HPP__