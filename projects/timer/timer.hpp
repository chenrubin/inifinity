
#include <boost/chrono.hpp>     // boost::chrono::milliseconds
#include <boost/function.hpp>   // boost::function
#include <queue>                // std::priority_queue
#include <sys/timerfd.h>        // timerFd_create, timerFd_settime

#include "reactor.hpp"          // class Reactor
#include "MyUtils.hpp"            // class Uncopyable

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
    explicit Timer(Reactor *reactor_);
    ~Timer();

    // user schedules task_ to be invoked at time_ interval  from now
    void ScheduleAction(boost::chrono::milliseconds time_, 
                        boost::function<void()> task_);

private:
    typedef std::pair<boost::chrono::system_clock::time_point, 
                        boost::function<void()> > timeFuncPair_t;
    class CompareFunc
    {
    public:
        bool operator()(const timeFuncPair_t &time1, 
                        const timeFuncPair_t &time2);
    };

    std::priority_queue<timeFuncPair_t, 
                        std::vector<timeFuncPair_t>,
                        CompareFunc> m_queue;

    Reactor *m_reactor;
    int m_timerFd;

    void MyCallbackIMP(int fd_);

    // Updates struct itimerspec with the appropriate time_ 
    void UpdateItimerSpecIMP(itimerspec *tspec, 
                             boost::chrono::milliseconds time_);
    
    // Compare between two time duration with respect to tolerance
    // Function compares cpu ticks                         
    bool IsSameTimeIMP(boost::chrono::system_clock::time_point time1, 
                       boost::chrono::system_clock::time_point time2,
                       boost::chrono::milliseconds tolerance);

    // Calculate absolute time of time_                   
    void CalculateAbsoluteTime(/*itimerspec*/boost::chrono::system_clock::time_point *itspec, 
                               boost::chrono::milliseconds time_);
    void ConvertChronoToiTimerspec(boost::chrono::system_clock::time_point time,
                                   itimerspec *itspec);                           
};

}   // namespace ilrd