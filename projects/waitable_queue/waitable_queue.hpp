// Waitable Queue - Header file
// Last update: 29/03/2020
// Author: RD 78-79
// Reviewer: Tsisana
/*----------------------------------------------------------------------------*/
#ifndef __WAITABLE_QUEUE_HPP__ 
#define __WAITABLE_QUEUE_HPP__
/*----------------------------------------------------------------------------*/
#include <boost/thread/thread.hpp> 	// boost::condition_variable
#include <boost/thread/mutex.hpp> 	// boost::mutex
#include <boost/thread/recursive_mutex.hpp> 	// boost_recursive_mutex
#include <iostream> 	// boost::mutex

#include <queue>    				// queue, priority_queue

#include "MyUtils.hpp"				// class Uncopyable

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

	void Push(typename Q::const_reference element_);
	
	// blocking until timeout
	bool Pop(typename Q::reference element_, boost::chrono::milliseconds timeout_);
	
	// blocking
	void Pop(typename Q::reference element_);

	bool IsEmpty() const;
	
private:
	mutable boost::recursive_mutex m_mutex;
	boost::condition_variable_any m_cond;
	Q m_queue;

	typename Q::value_type PeekFuncIMP(std::queue<typename Q::value_type> &queue);
	typename Q::value_type PeekFuncIMP(std::priority_queue<typename Q::value_type> &queue);
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
	boost::unique_lock<boost::recursive_mutex> lock(m_mutex);

	m_queue.push(element_);
	m_cond.notify_one();
}

template <typename Q>
void WaitableQueue<Q>::Pop(typename Q::reference element_)
{ 
	boost::unique_lock<boost::recursive_mutex> lock(m_mutex);
	
	if (IsEmpty())
	{
		m_cond.wait(lock);
	}
	
	element_ = PeekFuncIMP(m_queue);
	m_queue.pop();
}

template <typename Q>
bool WaitableQueue<Q>::Pop(typename Q::reference element_, 
						   boost::chrono::milliseconds timeout_)
{
	boost::unique_lock<boost::recursive_mutex> lock(m_mutex);
	boost::cv_status status = boost::cv_status::no_timeout;
	
	if (IsEmpty())
	{
		status =  m_cond.wait_for(lock, timeout_);
	}

	if (status == boost::cv_status::no_timeout)
	{
		element_ = PeekFuncIMP(m_queue);
		m_queue.pop();
	}
	
	return (status == boost::cv_status::timeout);
}

template <typename Q>
bool WaitableQueue<Q>::IsEmpty() const
{
	boost::unique_lock<boost::recursive_mutex> lock(m_mutex);

	return m_queue.empty();
}

template <typename Q>
typename Q::value_type WaitableQueue<Q>::PeekFuncIMP(std::queue<typename Q::value_type> &queue)
{
	return queue.front();
}

template <typename Q>
typename Q::value_type WaitableQueue<Q>::PeekFuncIMP(std::priority_queue<typename Q::value_type> &queue)
{
	return queue.top();
}

/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __WAITABLE_QUEUE_HPP__
