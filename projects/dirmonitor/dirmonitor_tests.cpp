#include <iostream>             // cout
#include <sys/inotify.h>        // inotify
#include <unistd.h>             // read
#include <stdio.h>              // perror
#include <errno.h>              // errno
#include <dirent.h>             // NAME_MAX

#include "dirmonitor.hpp"       // class dirmonitor

using namespace ilrd;


int main()
{
    DirMonitor monitor("/home/student/practice/dirPractice");

    sleep(20);
  /*  
    {
        std::cout << "event struct - wd = " << iEvents.wd << "\n";
        std::cout << "event struct - mask = " << iEvents.mask << "\n";
        std::cout << "event struct - cookie = " << iEvents.cookie << "\n";
        std::cout << "event struct - len = " << iEvents.len << "\n";
        std::cout << "event struct - name[] = " << iEvents.name << "\n";
    }
*/
    return 0;
}