#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>

int main()
{
    std::string pipe_path = "/home/student0/chen-rubin/cpp_fs/ws/ipc/named_pipe/n_pipe.txt";
    char buf[10] = "pong";
    char read_buf[10] = "";
    int fd = 0;

    for (int i = 0; i < 20; ++i)
    {
        fd = open(pipe_path.c_str(), O_RDONLY);
        read(fd, read_buf, 10);
        std::cout << "secondary reads " << read_buf << std::endl;
        close(fd);
        fd = open(pipe_path.c_str(), O_WRONLY);
        write(fd, buf, 10);
        close(fd);
    }
}