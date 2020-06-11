#ifndef __DIRMONITOR_HPP__ 
#define __DIRMONITOR_HPP__

#include <boost/thread.hpp>     // boostr::thread
#include "MyUtils.hpp"          // uncopyable

namespace ilrd
{
class DirMonitor : private Uncopyable
{
public:
    explicit DirMonitor(std::string path);
    ~DirMonitor();

private:
    void MonitorRoutineIMP();
    int AddWatchIMP(std::string path);

    int m_fd;
    int m_watchFd;
    boost::thread m_monitor;
};
}
#endif