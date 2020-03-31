// Waitable Queue - Header file
// Last update: 29/03/2020
// Author: RD 78-79
// Reviewer: Tsisana
/*----------------------------------------------------------------------------*/
#ifndef __WAITABLE_QUEUE_HPP__ 
#define __WAITABLE_QUEUE_HPP__
/*----------------------------------------------------------------------------*/
#include <boost/thread/recursive_mutex.hpp> 	// boost_recursive_mutex
#include <boost/chrono.hpp>						// boost::chrono::milliseconds
#include <boost/thread/condition_variable.hpp>	// boost::condition_variable_any
#include <boost/bind.hpp>						// boost::bind()
#include <queue>								

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

	// pushes element int queue. No Exceptions
	void Push(typename Q::const_reference element_) NOEXCEPT;
	
	/*
	*	Pops element from queue. waits until queue is not empty or until timeout
	*	whichever comes first. No Exceptions
	*/ 
	bool Pop(typename Q::reference element_, boost::chrono::milliseconds timeout_); // wait_for can throw exception
	
	// Pops element from queue. waits until queue is not empty. No Exceptions
	void Pop(typename Q::reference element_) NOEXCEPT;

	// returns whether queue is empty or not. No Exceptions
	bool IsEmpty() const; // throws according to lock
	
private:
	mutable boost::recursive_mutex m_mutex;
	boost::condition_variable_any m_cond;
	Q m_queue;

	// add another queue template for custom made queue
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
	
	m_cond.wait(lock, !boost::bind(&WaitableQueue::IsEmpty, this));
	element_ = PeekFuncIMP(m_queue);
	m_queue.pop();
}

template <typename Q>
bool WaitableQueue<Q>::Pop(typename Q::reference element_, 
						   boost::chrono::milliseconds timeout_)
{
	boost::unique_lock<boost::recursive_mutex> lock(m_mutex);
	bool status = true;
	
	status =  m_cond.wait_for(lock, timeout_, !boost::bind(&WaitableQueue::IsEmpty, this));

	if (status == true)
	{
		element_ = PeekFuncIMP(m_queue);
		m_queue.pop();
	}
	
	return (status == true);
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