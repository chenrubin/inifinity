#include <iostream>             // cout
#include <sys/inotify.h>        // inotify
#include <unistd.h>             // read
#include <stdio.h>              // perror
#include <errno.h>              // errno
#include <dirent.h>             // NAME_MAX

#include "dirmonitor.hpp"       // dirmonitor class

namespace ilrd
{
DirMonitor::DirMonitor(std::string path)
    : m_fd(inotify_init())
    , m_watchFd(AddWatchIMP(path))
    , m_monitor(boost::thread(boost::bind(&DirMonitor::MonitorRoutineIMP, this)))
{}

DirMonitor::~DirMonitor()
{
    inotify_rm_watch(m_fd, m_watchFd);
    close(m_fd);
}

int DirMonitor::AddWatchIMP(std::string path)
{
    m_watchFd = inotify_add_watch(m_fd, path.c_str(), 
                                  IN_CREATE | IN_DELETE | IN_MODIFY);
    if (-1 == m_watchFd)
    {
        close(m_fd);
        perror("inotify_add_watch failed");
    }

    return m_watchFd;                              
}

void DirMonitor::MonitorRoutineIMP()
{
    struct inotify_event iEvents = {0};

    while (true)
    {
        ssize_t numread = read(m_fd, &iEvents, sizeof (iEvents) + NAME_MAX + 1);
        if (0 == numread)
        {
            perror("numread = 0");
        }
        else if (-1 == numread)
        {
            std::cout << "errno = " << errno << "\n";
            perror("-1 == numread");
        }
        std::cout << "mask = " << iEvents.mask << "\n";
    }
}
/*
int main()
{
    int notifyFd = inotify_init();
    if (-1 == notifyFd)
    {
        perror("notifyFd");
    }
    std::cout << "notifyFd = " << notifyFd << "\n";
    int watchFd = inotify_add_watch(notifyFd, "/home/student/practice/dirPractice",
                                                IN_CREATE | IN_DELETE | IN_MODIFY);
    if (-1 == watchFd)
    {
        perror("watchFd");
    }
    std::cout << "watchFd = " << watchFd << "\n";

    struct inotify_event iEvents = {0};
    std::cout << "sizeof(iEvents) = " << sizeof (struct inotify_event) + NAME_MAX + 1 << "\n";
 
        ssize_t numread = read(notifyFd, &iEvents, sizeof (struct inotify_event) + NAME_MAX + 1);
        if (0 == numread)
        {
            perror("numread = 0");
        }
        else if (-1 == numread)
        {
            std::cout << "errno = " << errno << "\n";
            perror("-1 == numread");
        }
        std::cout << "mask = " << iEvents.mask << "\n";
    }

    return 0;
}*/
} // end pf namespace