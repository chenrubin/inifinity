#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <cerrno>

#include "reactor.hpp"

#define MAX_LEN (300)

using namespace ilrd;
namespace
{
void HendlerFd1(int fd_);
void HendlerFd2(int fd_);
//------------------------------------------------------------------------------
} // end namespace anonymous
//------------------------------------------------------------------------------
Reactor r;

int main()
{
	int fd1 = open("./1.txt", O_APPEND);
    if (-1 == fd1)
    {
        perror("open file 1");
    }
	boost::function<void(int)> ff = HendlerFd1;
	r.AddFd(fd1, Reactor::READ, ff);

  
    
	r.Run();
 	close(fd1);
	return 0;
}
//------------------------------------------------------------------------------
namespace
{
void HendlerFd2(int fd_)
{
	static int flagWhenRemove = 30;
	
	if (flagWhenRemove)
	{
        write(fd_, "tatata\n", strlen("tatata\n") + 1); 
        char input[MAX_LEN] = {'\0'};
        read(fd_, input, 1);
   
	--flagWhenRemove;
	std::cout<<"fd 2 now" << flagWhenRemove <<std::endl;
	}
	
	if (flagWhenRemove == 10)
	{
        r.RemoveFd(fd_, Reactor::READ);
        std::cout<< "removed 2 fd"<<std::endl;
        std::cout<< flagWhenRemove<<std::endl;
        while(flagWhenRemove)
        {
            --flagWhenRemove;
        }
        std::cout<< "stop 2 fd"<<std::endl;
        std::cout<< flagWhenRemove<<std::endl;
        r.Stop();
        close(fd_);
	}	
}
//------------------------------------------------------------------------------
void HendlerFd1(int fd_)
{
	static int flagWhenAddTest = 10;
    static int flagWhenRemove = 10;
	if (flagWhenAddTest)
	{
        write(fd_, "tatata\n", strlen("tatata\n") + 1); 
        char input[MAX_LEN] = {'\0'};
        read(fd_, input, 1);
    
        --flagWhenAddTest;
        --flagWhenRemove;
        std::cout<<"fd 1 now" << flagWhenRemove <<std::endl;
	}
	if (flagWhenAddTest == 8)
	{
        int s = open("./2.txt", O_APPEND);
        if (-1 == s)
        {
            perror("open file 2");
        }
        boost::function<void(int)> ss = HendlerFd2;
        r.AddFd(s, Reactor::READ, ss);
        std::cout<<"added 2 fd"<<std::endl;
        std::cout<< flagWhenAddTest<<std::endl;
	}
	if (flagWhenRemove == 5)
	{
        r.RemoveFd(fd_, Reactor::READ);
        std::cout<< "removed 1 fd"<<std::endl;
        std::cout<< flagWhenRemove<<std::endl;
	}
}
//------------------------------------------------------------------------------
} // end namespace anonymous

