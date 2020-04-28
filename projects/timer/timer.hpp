
/**************************************
 *     Author: chen
 *     Reviewer: Dvir
 *     Timer
 *     Date: 21.4.2020            
 * ************************************/
#ifndef __TIMER_HPP__ 
#define __TIMER_HPP__

#include <boost/chrono.hpp>     // boost::chrono::milliseconds
#include <boost/function.hpp>   // boost::function
#include <queue>                // std::priority_queue
#include <sys/timerfd.h>        // timerFd_create, timerFd_settime

#include "reactor.hpp"          // class Reactor
#include "MyUtils.hpp"          // class Uncopyable
#include "timerfd.hpp"          // RAII timerFD

/*
    user uses timer in order to schedule in advance callbacks.
    user inputs callback and a time interval that he wants the callback 
    to be invoked at.
*/
namespace ilrd
{
class Timer: private Uncopyable
{
public:
    // create timer object, reactor should be running when
	// creating the timer.
	// throw std::runtime_error or std::bad_alloc.
    explicit Timer(Reactor *reactor_);
    ~Timer();

    // user schedules task_ to be invoked at time_ interval  from now
    void ScheduleAction(boost::chrono::milliseconds time_, 
                        boost::function<void()> task_);

private:
    typedef boost::chrono::system_clock systemClock_t;
    typedef std::pair<systemClock_t::time_point, 
                        boost::function<void()> > timeFuncPair_t;
    class CompareFunc
    {
    public:
        bool operator()(const timeFuncPair_t &time1, 
                        const timeFuncPair_t &time2);
    };

    void MyCallbackIMP(int fd_);

    // Updates struct itimerspec with the appropriate time_ 
    void UpdateItimerSpecIMP(itimerspec *tspec, 
                             boost::chrono::milliseconds time_);
    
    // Compare between two time duration with respect to tolerance
    // Function compares cpu ticks                         
    bool IsSameTimeIMP(systemClock_t::time_point time1, 
                       systemClock_t::time_point time2,
                       boost::chrono::milliseconds tolerance);

    // Calculate absolute time of time_                   
    void CalculateAbsoluteTime(systemClock_t::time_point *itspec, 
                               boost::chrono::milliseconds time_);
    void ConvertChronoToiTimerspec(systemClock_t::time_point time,
                                   itimerspec *itspec);
    void SetTime();                               

    std::priority_queue<timeFuncPair_t, 
                        std::vector<timeFuncPair_t>,
                        CompareFunc> m_queue;

    Reactor *m_reactor;
    TimerFD m_timerFd;
};

inline bool Timer::CompareFunc::operator()(const timeFuncPair_t& time1, const timeFuncPair_t& time2)
{
    return (time1.first > time2.first);
}
}   // namespace ilrd
#endif // __TIMER_HPP__