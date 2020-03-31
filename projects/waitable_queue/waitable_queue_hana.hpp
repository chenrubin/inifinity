// Waitable Queue - Header file
// Last update: 31/03/2020
// Author: Hana
// Reviewer: Sasha
/*----------------------------------------------------------------------------*/
#ifndef __WAITABLE_QUEUE_HPP__ 
#define __WAITABLE_QUEUE_HPP__
/*----------------------------------------------------------------------------*/
#include <boost/thread/recursive_mutex.hpp>		// boost::recursive_mutex
#include <boost/thread/condition_variable.hpp>	// boost::condition_variable_any
#include <boost/chrono.hpp>						// boost::chrono::milliseconds
#include <boost/bind.hpp>						// boost::bind()

#include "uncopyable.hpp"						// class Uncopyable
#include "cpp_std.hpp"							// NOEXCEPT
/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
/* 
Thread safe waitable wrapper to a queue<T>.

Q must support the following operations:
1. void pop() - removes the top element.
2. const T& front() const - returns arefernce to the top element
3. void push(T& value) - push an element (deep copy)
4. bool empty() const - returns true if the queue is empty
5. Default constructor.
6. typedef typename T::reference reference
*/
/*----------------------------------------------------------------------------*/
template <typename Q>
class WaitableQueue : private Uncopyable
{
public:
	// Pushes element_ to the queue.
	// Exceptions: 
	//	- Re-throws any exception from push()
	//	- Might throw boost::thread_resource_error.
	void Push(const typename Q::reference element_);
	
	// Pops the top element from the queue and copy it to element_.
	// If the queue is empty, blocked until the next push. 
	// Exceptions: 
	//	- Re-throws any exception from pop() and front()
	//	- Might throw boost::thread_resource_error.
	void Pop(typename Q::reference element_);

	// Pops the top element from the queue and copy it to element_, 
	// but only it is possible within the timeout_.
	// Return value: If succeed - true. If faild - false.
	// Exceptions: 
	//	- Re-throws any exception from pop() and front()
	//	- Might throw boost::thread_resource_error.
	bool Pop(typename Q::reference element_, 
			boost::chrono::milliseconds timeout_);

	// If the queue is empty, returns true. Otherwise, returns false.
	// Exceptions: Might throw boost::thread_resource_error.
	bool IsEmpty() const;
	
private:
	void PopIMP(typename Q::reference element_);

	mutable boost::recursive_mutex m_mutex;
	boost::condition_variable_any m_cond;
	Q m_queue;
};
/*----------------------------------------------------------------------------*/
template <typename Q>
void WaitableQueue<Q>::Push(const typename Q::reference element_)
{
	boost::recursive_mutex::scoped_lock lock(m_mutex);

	m_queue.push(element_);
	m_cond.notify_one();		// should we take it outside the lock ?
}
/*----------------------------------------------------------------------------*/
template <typename Q>
void WaitableQueue<Q>::Pop(typename Q::reference element_)
{
	boost::recursive_mutex::scoped_lock lock(m_mutex);
	
	m_cond.wait(lock, !(boost::bind(&WaitableQueue::IsEmpty, this)));

	PopIMP(element_);
}
/*----------------------------------------------------------------------------*/
template <typename Q>
bool WaitableQueue<Q>::Pop(typename Q::reference element_,
		boost::chrono::milliseconds timeout_)
{
	boost::recursive_mutex::scoped_lock lock(m_mutex);

	bool waitSucceed = m_cond.wait_for(lock,
			timeout_, !(boost::bind(&WaitableQueue::IsEmpty, this)));
	if (!waitSucceed)
	{
		return false;
	}

	PopIMP(element_);

	return true;
}
/*----------------------------------------------------------------------------*/
template <typename Q>
bool WaitableQueue<Q>::IsEmpty() const
{
	boost::recursive_mutex::scoped_lock lock(m_mutex);

	return m_queue.empty();
}
/*----------------------------------------------------------------------------*/
template <typename Q>
void WaitableQueue<Q>::PopIMP(typename Q::reference element_)
{
	// m_mutex should be locked by the caller ! 
	// boost::recursive_mutex::scoped_lock lock(m_mutex);

	assert(!IsEmpty());
	
	element_ = m_queue.front();
	m_queue.pop();
}
/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __WAITABLE_QUEUE_HPP__