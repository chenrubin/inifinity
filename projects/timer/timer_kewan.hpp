// Reactor - Header file
// Last update: 17/03/2020
// Author: RD 78-79
// Reviewer: Kewan Kheralden 
/*----------------------------------------------------------------------------*/
#ifndef __TIMER_HPP__ 
#define __TIMER_HPP__

#include <queue>				// std::priority_queue
#include <boost/chrono.hpp>		// boost::chrono::nanoseconds
#include <boost/function.hpp>	// boost::function

#include "reactor.hpp"			// class Reactor
#include "MyUtils.hpp"			// class Uncopyable
/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
// Timer designed to schedule tasks to work in a specific time,
// the task recived as callback, and the time is in nanoseconds.
class Timer: private Uncopyable
{
public:
	// create timer object, reactor should be running when
	// creating the timer.
	// throw std::runtime_error or std::bad_alloc.
	explicit Timer(Reactor *reactor_);
	~Timer();

	// Schedule the callback to run after the given time is passed.
	void ScheduleAction(boost::chrono::nanoseconds time_, 
				boost::function<void()> task_);

private:
	struct Task
	{
		boost::function<void()> m_callback;
		boost::chrono::system_clock::time_point m_time;
	};
	
	class CompareFunc
	{
	public:
		bool operator() (Task task1_, Task task2_);
	};
	
	void TimerCallbackIMP();
	int CreateTimerIMP();
	void SetTimerIMP(const boost::chrono::system_clock::time_point& time_);
	
	int m_timer_fd;
	Reactor *m_reactor;
	std::priority_queue<Task, std::vector<Task>, CompareFunc> m_tasks_queue;
};
/*----------------------------------------------------------------------------*/
}   // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __TIMER_HPP__