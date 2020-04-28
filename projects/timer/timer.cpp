/**************************************
 *     Author: chen
 *     Reviewer: Dvir
 *     Timer
 *     Date: 21.4.2020            
 * ************************************/

#include <sys/timerfd.h>        // timerFd_create, timerFd_settime
#include <boost/bind.hpp>        // boost::bind

#include "timer.hpp"
#define NANO_IN_SEC (1000000000)

#define TICKS_COMPARE_TOLERANCE (1000)

namespace ilrd
{
Timer::Timer(Reactor *reactor_)
    : m_reactor(reactor_)
    , m_timerFd(timerfd_create(CLOCK_MONOTONIC, 0))
{
    HandleErrorIfExists(m_timerFd.GetFd(), "bad create\n");
    m_reactor->AddFd(m_timerFd.GetFd(), m_reactor->READ, 
                     boost::bind(&Timer::MyCallbackIMP, this, m_timerFd.GetFd()));

}

Timer::~Timer()
{}

void Timer::ScheduleAction(boost::chrono::milliseconds time_,
                           boost::function<void()> task_)
{
    systemClock_t::time_point absolute_time;
    CalculateAbsoluteTime(&absolute_time, time_);
    timeFuncPair_t new_pair = std::make_pair(absolute_time, task_);

    m_queue.push(new_pair);
    if (IsSameTimeIMP(absolute_time, m_queue.top().first, 
                      boost::chrono::milliseconds(TICKS_COMPARE_TOLERANCE)))
    {
        SetTime();
    }
}

void Timer::ConvertChronoToiTimerspec(systemClock_t::time_point time_,
                                      itimerspec *itspec)
{
    boost::chrono::nanoseconds nano = 
           boost::chrono::duration_cast<boost::chrono::nanoseconds>
                                       (time_ - systemClock_t::now());
    long temp = nano.count();

    itspec->it_value.tv_sec = temp / NANO_IN_SEC;
    itspec->it_value.tv_nsec = temp % NANO_IN_SEC;
    itspec->it_interval.tv_nsec = 0;
    itspec->it_interval.tv_sec = 0;
}

void Timer::MyCallbackIMP(int fd_)
{
    uint64_t temp;
    HandleErrorIfExists(read(fd_, &temp, sizeof(temp)), "read failed");
    assert(1 == temp);

    m_queue.top().second();
    m_queue.pop();

    if (!m_queue.empty())
    {
      SetTime();
    }
}

bool Timer::IsSameTimeIMP(systemClock_t::time_point time1, 
                          systemClock_t::time_point time2, 
                          boost::chrono::milliseconds tolerance)
{
    boost::chrono::milliseconds diff = 
        boost::chrono::duration_cast<boost::chrono::milliseconds>(time1 - time2);
    
    return (abs(diff.count()) < tolerance.count());
}

void Timer::CalculateAbsoluteTime(systemClock_t::time_point *current_time, 
                                  boost::chrono::milliseconds time_)
{
    *current_time = systemClock_t::now();
    *current_time += time_;
}

void Timer::SetTime()
{
    itimerspec itspec;

    ConvertChronoToiTimerspec(m_queue.top().first, &itspec);
    HandleErrorIfExists(timerfd_settime(m_timerFd.GetFd(), 0, &itspec, NULL),
                                                                    "settime");
}
}