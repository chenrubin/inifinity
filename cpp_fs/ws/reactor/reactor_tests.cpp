#include <iostream> /* cout */
#include <arpa/inet.h> /* send, connect */
#include <stdio.h> /* perror */

#include "reactor.hpp" /* reactor */
#include "MyUtils.hpp" /* Uncopyable */

#define PORT (4651)
#define STDIN 0
#define STDOUT 1
#define BUFF_SIZE 256

using namespace ilrd;

void TestCtroDtor();
void TestSimpleAddRemove();
void TestRun();

static Reactor rctr;

namespace
{
    void Callback1IMP(int fd);
    void Callback2IMP(int fd);
    void Callback3IMP(int fd);
    void Callback4IMP(int fd);
    void Callback5IMP(int fd);
    void Callback6IMP(int fd);

    void UpdateAddressIMP(struct sockaddr_in *addr, 
                          short family, 
                          unsigned long s_addr, 
                          unsigned short port);
    void CallbackUdpIMP(int socket);
    void CallbackBcIMP(int socket);
    void CallbackStdout1IMP(int socket);
    void CallbackStdout2IMP(int socket);
    void CallbackTcpIMP(int socket);
    void CreateAndBindSocketIMP(int *sock, 
                               short family, 
                               __socket_type type, 
                               int proto,
                               struct sockaddr_in *addr);
}

int main()
{
    TestCtroDtor();
    TestSimpleAddRemove();
    TestRun();

    return 0;
}

void TestCtroDtor()
{
    Reactor rctr;
}

void TestSimpleAddRemove()
{
    Reactor rctr;
    int fd1 = 1;
    int fd2 = 2;
    int fd3 = 3;
    int fd4 = 4;
    int fd5 = 5;
    int fd6 = 6;
    boost::function<void(int)> call1 = Callback1IMP;
    boost::function<void(int)> call2 = Callback2IMP;
    boost::function<void(int)> call3 = Callback3IMP;
    boost::function<void(int)> call4 = Callback4IMP;
    boost::function<void(int)> call5 = Callback5IMP;
    boost::function<void(int)> call6 = Callback6IMP;
  
    rctr.AddFd(fd1, rctr.READ, call1);
    rctr.AddFd(fd2, rctr.WRITE, call2);
    rctr.AddFd(fd3, rctr.EXCEPT, call3);
    rctr.AddFd(fd4, rctr.READ, call4);
    rctr.AddFd(fd5, rctr.WRITE, call5);
    rctr.AddFd(fd6, rctr.EXCEPT, call6);

    rctr.RemoveFd(fd3, rctr.EXCEPT);
    rctr.RemoveFd(fd4, rctr.READ);
    rctr.RemoveFd(fd2, rctr.WRITE);
    rctr.RemoveFd(fd1, rctr.READ);
    rctr.RemoveFd(fd5, rctr.WRITE);
    rctr.RemoveFd(fd6, rctr.EXCEPT);
}

void TestRun()
{
    struct sockaddr_in non_bc_server_addr;
	struct sockaddr_in bc_server_addr;
    memset(&non_bc_server_addr, 0, sizeof(non_bc_server_addr));
	memset(&bc_server_addr, 0, sizeof(non_bc_server_addr));
    struct sockaddr_in server_array[] = {non_bc_server_addr, bc_server_addr};
    int sockfd_bc = 0;
    int sockfd_tcp = 0;
    int sockfd_udp = 0;

    int sock_arr[] = {sockfd_bc, sockfd_udp, sockfd_tcp, STDIN};
    int broadcast = 1;

    UpdateAddressIMP(&server_array[0], AF_INET, INADDR_LOOPBACK, PORT);
    UpdateAddressIMP(&server_array[1], AF_INET, INADDR_BROADCAST, PORT);

    try
	{
		CreateAndBindSocketIMP(&sock_arr[0], 
							   AF_INET, 
							   SOCK_DGRAM, 
							   IPPROTO_UDP, 
							   &server_array[1]);

		CreateAndBindSocketIMP(&sock_arr[1], 
							   AF_INET, 
							   SOCK_DGRAM, 
							   IPPROTO_UDP, 
							   &server_array[0]);
		CreateAndBindSocketIMP(&sock_arr[2],
                                AF_INET, 
                            	SOCK_STREAM, 
                           		IPPROTO_TCP, 
                           		&server_array[0]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}   
  
    if (-1 == setsockopt(sock_arr[0], SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)))
    {
        perror("setsockopt");
		throw std::runtime_error("setseckopt failed");
    }

    /* prepare pairs of fd and callback */
    boost::function<void(int)> callBc = CallbackBcIMP;
    boost::function<void(int)> callUdp = CallbackUdpIMP;
    boost::function<void(int)> callStdout1 = CallbackStdout1IMP;
    boost::function<void(int)> callStdout2 = CallbackStdout2IMP;
    boost::function<void(int)> callTcp = CallbackTcpIMP;

    rctr.AddFd(sock_arr[0],rctr.READ, callBc);
    rctr.AddFd(sock_arr[1],rctr.READ, callUdp);
    rctr.AddFd(STDOUT,rctr.WRITE, callStdout1);
 //   rctr.AddFd(sock_arr[2],rctr.READ, callTcp);
    Reactor::error_t status = rctr.Run();

    std::cout << "status = " << status << std::endl;
}

namespace
{
    void Callback1IMP(int fd)
    {
        std::cout << "callback 1 -> fd read = " << fd << std::endl; 
    }

    void Callback2IMP(int fd)
    {
        std::cout << "callback 2 -> fd = " << fd << std::endl; 
    }

    void Callback3IMP(int fd)
    {
        std::cout << "callback 3 -> fd = " << fd << std::endl; 
    }

    void Callback4IMP(int fd)
    {
        std::cout << "callback 4 -> fd = " << fd << std::endl; 
    }

    void Callback5IMP(int fd)
    {
        std::cout << "callback 5 -> fd = " << fd << std::endl; 
    }

    void Callback6IMP(int fd)
    {
        std::cout << "callback 6 -> fd = " << fd << std::endl; 
    }

/*
    Auxiliary functions for Run
*/

    void UpdateAddressIMP(struct sockaddr_in *addr, 
                      short family, 
                      unsigned long s_addr, 
                      unsigned short port)
    {
        assert(addr);

        addr->sin_family = family;
        addr->sin_addr.s_addr = htonl(s_addr);
        addr->sin_port = htons(port);
    }

    void CallbackUdpIMP(int socket)
    {
        assert(socket >= 0);

        std::cout << "sockfd reply" << std::endl;
        struct sockaddr_in addr;
        socklen_t size = sizeof(addr);
        char read_buff[BUFF_SIZE];
        char send_buff[] = "server send a reply";

        ssize_t bytes = recvfrom(socket, read_buff, BUFF_SIZE, 0, (struct sockaddr *)&addr, &size);
        if (0 < bytes)
        {
            std::cout << read_buff << std::endl;

            if (-1 == sendto(socket, send_buff, 
                             BUFF_SIZE, MSG_CONFIRM,
                             (struct sockaddr *)&addr, 
                             size))
            {
                perror("server sendto");
                throw std::runtime_error("sendto failed");
            }
        }
     /*   else if (0 == bytes)
        {
            FD_CLR(socket, readfds);
        }
     */   else
        {
            perror("server recvfrom");
            throw std::runtime_error("recvfrom failed");
        }
    }

    void CallbackStdout1IMP(int socket)
    {
        static size_t counter = 0;
        std::cout << "This is stdout callback1" << std::endl;
        ++counter;
        if (10 == counter)
        {
            rctr.RemoveFd(socket, rctr.WRITE);
            rctr.AddFd(STDOUT,rctr.WRITE, CallbackStdout2IMP);
        }
    }

    void CallbackStdout2IMP(int socket)
    {
        static size_t counter = 0;
        std::cout << "This is stdout callback2" << std::endl;
        ++counter;
        if (10 == counter)
        {
            //rctr.RemoveFd(socket, rctr.WRITE);
            rctr.Stop();
        }
    }

    void CallbackTcpIMP(int socket)
    {
        assert(socket >= 0);

        std::cout << "sockfd tcp accept" << std::endl;
        struct sockaddr_in addr;
        socklen_t size = sizeof(addr);
        
        int new_tcp_sock = accept(socket, (struct sockaddr *)&addr, &size);
        if (-1 == new_tcp_sock)
        {
            perror("accept: ");
            throw std::runtime_error("accept failed");
        }

        rctr.AddFd(new_tcp_sock, rctr.READ, CallbackUdpIMP);
    }

    void CallbackBcIMP(int socket)
    {
        assert(socket >= 0);

        std::cout << "sockfd_bc" << std::endl;
        struct sockaddr_in addr;
        socklen_t size = sizeof(addr);
        char read_buff[BUFF_SIZE];
        char send_buff[] = "server send bc reply";

        recvfrom(socket, read_buff, BUFF_SIZE, 0, (struct sockaddr *)&addr, &size);
        std::cout << read_buff << std::endl;

        if (-1 == sendto(socket, send_buff, 
                        BUFF_SIZE, MSG_CONFIRM, 
                        (struct sockaddr *)&addr, 
                        size))
        {
            perror("server bc sendto");
        }
    }

   void CreateAndBindSocketIMP(int *sock, 
                               short family, 
                               __socket_type type, 
                               int proto,
                               struct sockaddr_in *addr)
    {
        assert(sock);
        assert(addr);

        *sock = socket(family, type, proto);
        if (-1 == *sock)
        {
            perror("socket: ");
            throw std::runtime_error("socket failed");
        }
        if (-1 == bind(*sock, (struct sockaddr *)addr, sizeof(*addr)))
        {
            perror("bind: ");
            throw std::runtime_error("bind failed");
        }
    }
}
