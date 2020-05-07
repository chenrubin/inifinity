#include <arpa/inet.h> /* send, connect */
#include <cstring> /* memset */
#include <sys/socket.h> /* family */
#include <stdio.h> /* perror */
#include <unistd.h> /* close(socket) */
#include <stdexcept> /* std::runtime_error*/	

#include "udp_socket.hpp"
#include "MyUtils.hpp"

namespace ilrd
{

UdpSocket::UdpSocket(unsigned short port_, std::string address_, bool isBind)
    : m_port(port_)
    , m_addr(address_)
    , m_fd(-1)
    , m_sockaddr(InitServerIMP())
{
    std::cout << "size = " << sizeof(m_sockaddr) << "\n";
    

    m_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    HandleErrorIfExists(m_fd, "socket");

    if (isBind)
    {
        HandleErrorIfExists(bind(m_fd, (struct sockaddr *)&m_sockaddr, 
                                        sizeof(m_sockaddr)), "bind");
    }
}

UdpSocket::~UdpSocket()
{
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!Inside UdpSocket Dtor!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    if (-1 != m_fd)
    {
        close(m_fd);
    }
}

UdpSocket::UdpSocket(const UdpSocket &other_)
{
    std::cout << "inside socket CCtor before port\n";
    this->m_port = other_.m_port;
    std::cout << "inside socket CCtor before m_addr\n";
    this->m_addr = other_.m_addr;
    std::cout << "inside socket CCtor before m_fd\n";
    this->m_fd = other_.m_fd;
    std::cout << "inside socket CCtor before sockaddr\n";
    this->m_sockaddr = other_.m_sockaddr;
    std::cout << "inside socket CCtor end\n";
}

int UdpSocket::GetFd()
{
    return m_fd;
}

struct sockaddr_in UdpSocket::InitServerIMP()
{
    std::cout << "Inside initServer\n";
    struct sockaddr_in sockaddr/* = {0}*/;
    std::cout << "sizeof(sockaddr) = " << sizeof(sockaddr) << "\n";
 //   memset(&sockaddr, 0, sizeof(sockaddr));

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(m_addr.c_str());
    sockaddr.sin_port = htons(m_port);

    return sockaddr;
}

struct sockaddr_in UdpSocket::GetSockAddr()
{
    return m_sockaddr;
}
} // end of namespace ilrd