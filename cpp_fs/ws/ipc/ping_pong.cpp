#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int pipedes1[2];
    int pipedes2[2];
    pipe(pipedes1);
    pipe(pipedes2);

    pid_t pid = fork();
    for (int i = 0; i < 200; ++i)
    {
        if (pid == 0) // child
        {
            char buf[256];
            close(pipedes1[0]);
            write(pipedes1[1], "Pong", 5);
            close(pipedes2[1]);
            read(pipedes2[0], buf, 256);
            std::cout << "child read = " << buf << std::endl;
        }
        else
        {
            char buf[256];
            close(pipedes2[0]);
            write(pipedes2[1], "Ping", 5);
            close(pipedes1[1]);
            read(pipedes1[0], buf, 256);
            std::cout << "parent read =     " << buf << std::endl;
        }
    }   
}