/*----------------------------------------------------------------------------*/
#ifndef __UDPSOCKET_HPP__ 
#define __UDPSOCKET_HPP__
/*----------------------------------------------------------------------------*/
#include <cstddef>			// size_t
#include <netinet/in.h>     // in_addr_t */

#include "MyUtils.hpp" /* Uncopyable */

namespace ilrd
{
class UdpSocket : private Uncopyable
{
public:    
    explicit UdpSocket(unsigned short port_, in_addr_t address,
                       int optname_,bool isBroadcast);
    ~UdpSocket();
    int GetFd();

private:
    unsigned short m_port;
    unsigned long m_addr;
    int m_fd;
    bool m_isBroadcast;
    int m_optname;
    struct sockaddr_in *m_sockaddr;

    void InitServer();
};
} // end of namespace ilrd
#endif