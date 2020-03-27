// Waitable Queue - Header file
// Last update: 26/03/2020
// Author: RD 78-79
/*----------------------------------------------------------------------------*/
#ifndef __WAITABLE_QUEUE_HPP__ 
#define __WAITABLE_QUEUE_HPP__
/*----------------------------------------------------------------------------*/
#include <boost/thread/mutex.hpp> 	// boost::mutex
#include <boost/thread/thread.hpp> 	// boost::condition_variable

#include "MyUtils.hpp"			// class Uncopyable

/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
template <typename Q>
class WaitableQueue : private Uncopyable
{
public:
	explicit WaitableQueue();

	~WaitableQueue();

	// TODO: check traits syntax 
	void Push(typename Q::const_reference element_);
	
	// blocking until timeout
	bool Pop(typename Q::reference element_, boost::chrono::milliseconds timeout_);
	
	// blocking
	void Pop(typename Q::reference element_);

	bool IsEmpty() const;
	
private:
	boost::mutex m_mutex;
	boost::condition_variable m_cond;
	Q m_queue; 
};

template <typename Q>
WaitableQueue<Q>::WaitableQueue()
{}

template <typename Q>
WaitableQueue<Q>::~WaitableQueue()
{}

template <typename Q>
void WaitableQueue<Q>::Push(typename Q::const_reference element_)
{
	
}

/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __WAITABLE_QUEUE_HPP__
