/**************************************
 *     Author: chen
 *     Reviewer: Dvir
 *     Timer
 *     Date: 21.4.2020            
 * ************************************/

#include <sys/timerfd.h>        // timerFd_create, timerFd_settime
#include <boost/bind.hpp>        // boost::bind

#include "timer.hpp"

#define TICKS_COMPARE_TOLERANCE (1000)

namespace ilrd
{
Timer::Timer(Reactor *reactor_)
    : m_reactor(reactor_)
    , m_timerFd(timerfd_create(CLOCK_MONOTONIC, 0))
{
    HandleErrorIfExists(m_timerFd, "bad create\n");
    m_reactor->AddFd(m_timerFd, m_reactor->READ, 
                     boost::bind(&Timer::MyCallbackIMP, this, m_timerFd));

}

Timer::~Timer()
{}

void Timer::ScheduleAction(boost::chrono::milliseconds time_,
                           boost::function<void()> task_)
{
    boost::chrono::system_clock::time_point absolute_time;
    CalculateAbsoluteTime(&absolute_time, time_);
    timeFuncPair_t new_pair = std::make_pair(absolute_time, task_);

    m_queue.push(new_pair);
    if (IsSameTimeIMP(absolute_time, m_queue.top().first, 
                      boost::chrono::milliseconds(TICKS_COMPARE_TOLERANCE)))
    {
        itimerspec itspec;

        ConvertChronoToiTimerspec(m_queue.top().first, &itspec);
        HandleErrorIfExists(timerfd_settime(m_timerFd, 0, &itspec, NULL),"settime3");
    }
}

void Timer::ConvertChronoToiTimerspec(boost::chrono::system_clock::time_point time_,
                                      itimerspec *itspec)
{
    boost::chrono::nanoseconds nano = 
           boost::chrono::duration_cast<boost::chrono::nanoseconds>
                                       (time_ - boost::chrono::system_clock::now());
    long temp = nano.count();

    itspec->it_value.tv_sec = temp / 1000000000;
    itspec->it_value.tv_nsec = temp % 1000000000;
    itspec->it_interval.tv_nsec = 0;
    itspec->it_interval.tv_sec = 0;
}

void Timer::MyCallbackIMP(int fd_)
{
    uint64_t temp;
    read(fd_, &temp, sizeof(temp));

    boost::chrono::system_clock::time_point now = boost::chrono::system_clock::now();
    std::cout << "time = " << now << "\n";

    m_queue.top().second();
    m_queue.pop();

    if (!m_queue.empty())
    {
        itimerspec itspec;
        ConvertChronoToiTimerspec(m_queue.top().first, &itspec);
        HandleErrorIfExists(timerfd_settime(m_timerFd, 0, &itspec, NULL), "settime1");
    }
}

bool Timer::IsSameTimeIMP(boost::chrono::system_clock::time_point time1, 
                          boost::chrono::system_clock::time_point time2, 
                          boost::chrono::milliseconds tolerance)
{
    boost::chrono::milliseconds diff = boost::chrono::duration_cast<boost::chrono::milliseconds>(time1 - time2);
    
    return (abs(diff.count()) < tolerance.count());
}

bool Timer::CompareFunc::operator()(const timeFuncPair_t& time1, const timeFuncPair_t& time2)
{
    return (time1.first > time2.first);
}

void Timer::CalculateAbsoluteTime(boost::chrono::system_clock::time_point *current_time, 
                                  boost::chrono::milliseconds time_)
{
    *current_time = boost::chrono::system_clock::now();
    *current_time += time_;
}
}