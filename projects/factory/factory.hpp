// Factory - Header file
// Last update: 30/03/2020
// Author:
// Reviewer: 
/*----------------------------------------------------------------------------*/
#ifndef __FACTORY_HPP__ 
#define __FACTORY_HPP__
/*----------------------------------------------------------------------------*/
#include <boost/function.hpp>	/* boost::function */
#include <map> 					/* std::map */

#include "MyUtils.hpp"		// class Uncopyable
//#include "cpp_std.hpp"			// NOEXCEPT

#define MAP (std::map<K, boost::function<boost::shared_ptr<T>(P)>)
#define FUNC (boost::function<boost::shared_ptr<T>(P)>)
/*----------------------------------------------------------------------------*/
namespace ilrd
{
namespace factory
{
/*----------------------------------------------------------------------------*/
template <typename T, typename K, typename P>	// T = base class
class Factory : private Uncopyable
{
public:
	/*
	*	No Ctor and Dtor by definition
	*/
	// If new key replaced an old one ,return value = false
	// If new key was immediatly inserted return value = true
	bool AddClass(const K& key_, 
			boost::function<boost::shared_ptr<T>(P)> createFunc_);
	
	boost::shared_ptr<T> Create(const K& key_, P param_) const; 
	
	// 2nd phase: unique_ptr c++11, 3rd? args...

private:
	std::map<K, boost::function<boost::shared_ptr<T>(P)> > m_keyFuncPairs;
//	....
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
//t = base, k = key ("hatul"), P param struct
template <typename T, typename K, typename P>
bool Factory<T,K,P>::AddClass(const K& key_, 
			  		   boost::function<boost::shared_ptr<T>(P)> createFunc_)
{
	typename std::map<K, boost::function<boost::shared_ptr<T>(P)> >::iterator iter;
//	std::pair<iter, bool> res;
/*	res = m_keyFuncPairs.insert(std::pair<K, FUNC>(key_, createFunc_));
	if (res == false)
	{
		m_keyFuncPairs[key_] = createFunc_;
	}
*/
	return res;
}
/*----------------------------------------------------------------------------*/
} // namespace factory



} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __FACTORY_HPP__