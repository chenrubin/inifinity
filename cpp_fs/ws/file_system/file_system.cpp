#include <cstdlib>
#include <cstddef> /* size_t */
#include <unistd.h> /*  O_RDONLY | O_CREAT */
#include <iostream> /* cout */
#include <sys/types.h> /*  O_RDONLY | O_CREAT */
#include <fcntl.h>
#include <errno.h>
#include <linux/fs.h>
#include <linux/ext2_fs.h>

/*
void CreateFileSystem()
{
    system("sudo modprobe brd");
    system("sudo mkfs.ext2 \\dev\\ram0");
    system("sudo mount -t ext2 \\dev\\ram0 -o sync");
}
*/
int main()
{
    int errno;
    char buf[256] = {'\0'};
    int fd = open("../../../../Downloads/fs_experiment/file.txt", O_RDWR);
    if (-1 == fd) 
    { 
        // print which type of error have in a code 
    } 

    // Read contents from file 
    read(fd, buf, 256);
    std::cout << buf << std::endl;
    close(fd);

    return 0;
}