#include <arpa/inet.h> /* send, connect */
#include <cstring> /* memset */
#include <sys/socket.h> /* family */
#include <stdio.h> /* perror */
#include <unistd.h> /* close(socket) */
#include <stdexcept> /* std::runtime_error*/	

#include "udp_socket.hpp"

namespace ilrd
{

UdpSocket::UdpSocket(unsigned short port_, in_addr_t address_,
                     int optname_,bool isBroadcast_)
    : m_port(port_)
    , m_addr(address_)
    , m_fd(-1)
    , m_isBroadcast(isBroadcast_)
    , m_optname(optname_)
    , m_sockaddr(new sockaddr_in)
{
    memset(m_sockaddr, 0, sizeof(*m_sockaddr));
    InitServer();

    m_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (-1 == m_fd)
    {
        perror("socket: ");
		throw std::runtime_error("socket failed");
    }

    if (-1 == bind(m_fd, (struct sockaddr *)m_sockaddr, sizeof(*m_sockaddr)))
    {
        perror("bind: ");
		throw std::runtime_error("bind failed");
    }

    if (m_isBroadcast)
    {
        int broadcast = 1;
        if (-1 == setsockopt(m_fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)))
        {
            perror("setsockopt");
            throw std::runtime_error("setsockopt failed");
        }
    }
}

UdpSocket::~UdpSocket()
{
    if (-1 != m_fd)
    {
        close(m_fd);
    }
    
    delete m_sockaddr;
}

int UdpSocket::GetFd()
{
    return m_fd;
}

void UdpSocket::InitServer()
{
    m_sockaddr->sin_family = AF_INET;
    m_sockaddr->sin_addr.s_addr = htonl(m_addr);
    m_sockaddr->sin_port = htons(m_port);
}
} // end of namespace ilrd 