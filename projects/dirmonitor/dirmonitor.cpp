#include <iostream>             // cout
#include <sys/inotify.h>        // inotify
#include <unistd.h>             // read
#include <stdio.h>              // perror
#include <errno.h>              // errno
#include <dirent.h>             // NAME_MAX


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
 //   {
  //             int      wd;       /* Watch descriptor */
   //            uint32_t mask;     /* Mask describing event */~
    //           uint32_t cookie;   /* Unique cookie associating related
     //                                events (for rename(2)) */
      //         uint32_t len;      /* Size of name field */
       //        char     name[];   /* Optional null-terminated name */
       //    };
    while (true)//for (size_t i = 0; i < 5; ++i)
    {
    //    std::cout << "i = " << i << "\n";
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
}