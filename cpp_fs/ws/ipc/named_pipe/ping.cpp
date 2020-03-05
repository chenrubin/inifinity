#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <errno.h>

#define RW (0666)

int main()
{
    std::string pipe_path = "/home/student0/chen-rubin/cpp_fs/ws/ipc/named_pipe/n_pipe.txt";
    int status = mkfifo(pipe_path.c_str(), RW);
    if (-1 == status)
    {
        std::cout << "mkfifo failed\n";
        std::cout << "errno = " << errno << std::endl;
    }
    char buf[10] = "ping";
    char read_buf[10] = "";
    int fd = 0;

    for (int i = 0; i < 20; ++i)
    {
        fd = open(pipe_path.c_str(), O_WRONLY);
        write(fd, buf, 10);
        close(fd);
        fd = open(pipe_path.c_str(), O_RDONLY);
        read(fd, read_buf, 10);
        std::cout << "primary reads " << read_buf << std::endl;
        close(fd);
    }
}