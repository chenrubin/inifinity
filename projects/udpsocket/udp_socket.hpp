/*----------------------------------------------------------------------------*/
#ifndef __UDPSOCKET_HPP__ 
#define __UDPSOCKET_HPP__
/*----------------------------------------------------------------------------*/
#include <cstddef>			// size_t
#include <netinet/in.h>     // in_addr_t */

#include "MyUtils.hpp" /* Uncopyable */

namespace ilrd
{
class UdpSocket
{
public:    
    explicit UdpSocket(unsigned short port_, std::string address_, bool isBind);
    ~UdpSocket();
    UdpSocket(const UdpSocket &other_);

    int GetFd();
    struct sockaddr_in GetSockAddr();

private:
    struct sockaddr_in InitServerIMP();

    unsigned short m_port;
    std::string m_addr;
    int m_fd;
    struct sockaddr_in m_sockaddr;
};
} // end of namespace ilrd
#endif