#include <boost/bind.hpp>	// boost::bind
#include <stdexcept>		// std::runtime_error
#include <cassert>			// assert
#include <sys/timerfd.h>	// timerfd_create, timerfd_settime, itimerspec

#include "timer_kewan.hpp"

#define SECOND 1000000000

namespace ilrd
{
Timer::Timer(Reactor *reactor_)
	: m_timer_fd(CreateTimerIMP())
	, m_reactor(reactor_)
	, m_tasks_queue()
{
	m_reactor->AddFd(m_timer_fd, Reactor::READ, 
			boost::bind(&Timer::TimerCallbackIMP, this));
}

Timer::~Timer()
{ }

void Timer::ScheduleAction(boost::chrono::nanoseconds time_, 
				boost::function<void()> task_)
{
	boost::chrono::system_clock::time_point new_time = 
			boost::chrono::system_clock::now();
	new_time += time_;
	Task new_task = {task_, new_time};

	m_tasks_queue.push(new_task);
	boost::chrono::system_clock::time_point current_top = 
			m_tasks_queue.top().m_time;
	
	if (current_top == new_time)
	{
		SetTimerIMP(new_time);
	}
}

void Timer::TimerCallbackIMP()
{
	uint64_t read_buf;
	read(m_timer_fd, &read_buf, sizeof(read_buf));
	assert(1 == read_buf);
	
	Task top_task = m_tasks_queue.top();
	top_task.m_callback();
	m_tasks_queue.pop();
	
	if (!m_tasks_queue.empty())
	{
		top_task = m_tasks_queue.top();
		SetTimerIMP(m_tasks_queue.top().m_time);
	}
}

int Timer::CreateTimerIMP()
{
	int fd = timerfd_create(CLOCK_REALTIME, 0);
	
	if (-1 == fd)
	{
		throw std::runtime_error("timerfd_create");
	}
	
	return fd;
}

void Timer::SetTimerIMP(const boost::chrono::system_clock::time_point& time_)
{
	struct itimerspec new_val = {0};

	boost::chrono::nanoseconds nano_time = 
			boost::chrono::duration_cast<boost::chrono::nanoseconds>
			(time_ - boost::chrono::system_clock::now());
	long converter = nano_time.count();

	new_val.it_value.tv_nsec = converter % SECOND;
	new_val.it_value.tv_sec = converter / SECOND;
	timerfd_settime(m_timer_fd, 0, &new_val, NULL);
}

bool Timer::CompareFunc::operator()
		(Task task1_, Task task2_)
{
	return (boost::chrono::nanoseconds(0) < 
			boost::chrono::duration_cast<boost::chrono::nanoseconds>(
			task1_.m_time - task2_.m_time));
}

} // namespace ilrd
