#ifndef __DLLOADER_HPP__ 
#define __DLLOADER_HPP__

#include <dirent.h>             // NAME_MAX
#include <boost/thread.hpp>     // boostr::thread
#include "MyUtils.hpp"          // uncopyable

namespace ilrd
{
class Dlloader : private Uncopyable
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