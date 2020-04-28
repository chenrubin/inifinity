#include <unistd.h>         // close

#include "timerfd.hpp"



namespace ilrd
{
TimerFD::TimerFD(int fd_)
    : m_timerFd(fd_)
{}

TimerFD::~TimerFD()
{
    close(m_timerFd);
}

int TimerFD::GetFd()
{
    return m_timerFd;
}
}// end of namespace ilrd