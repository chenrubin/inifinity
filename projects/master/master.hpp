#ifndef __MASTER_HPP__
#define __MASTER_HPP__

#include <boost/thread.hpp>             // boost::thread

#include "reactor.hpp"                  // class Reactor
#include "udp_socket.hpp"	            // class UdpSocket
#include "../minion/packet/packet.hpp"                   // class packet.hpp
#include "proxy_packet.hpp"             // RequestPacketRead, RequestPacketWrite

namespace ilrd
{
class Master
{
public:
    explicit Master(Reactor *reactor_);
    ~Master();

    // When read action is wanted master recieves RequestPacketRead
    // and converts it to a packet the minion can understand
    void onRead(const RequestPacketRead& packet_);

    // When write action is wanted master recieves RequestPacketRead
    // and converts it to a packet the minion can understand
    void onWrite(const RequestPacketWrite& packet_);

private:
    Reactor *m_reactor;
    boost::thread m_reactorRun;
    UdpSocket m_socket;
//    DriverProxy m_proxy;
    
};
} // end of namespace ilrd

#endif // __MASTER_HPP__