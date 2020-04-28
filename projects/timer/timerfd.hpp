#ifndef __TIMERFD_HPP__ 
#define __TIMERFD_HPP__

namespace ilrd
{
class TimerFD
{
public:    
    TimerFD(int fd_);
    ~TimerFD();

    int GetFd();

private:
    int m_timerFd;
};
} // end of namespace ilrd
#endif // __TIMERFD_HPP__ 