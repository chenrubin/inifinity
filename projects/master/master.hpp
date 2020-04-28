#ifndef __MASTER_HPP__
#define __MASTER_HPP__

#include <boost/thread.hpp>             // boost::thread

#include "reactor.hpp"                  // class Reactor
#include "driver_proxy.hpp"             // class driver_procy
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
    size_t LocateMinionIndexByOffset(uint64_t offset);
    size_t LocateLocalBlockIndexByOffset(uint64_t offset);
    void CreateReadRequestPacket(RequestPacket *reqPack, 
                                 RequestPacketRead const *pkFromProxy);
    void CreateWriteRequestPacket(RequestPacket *reqPack, 
                                  RequestPacketWrite const *pkFromProxy);
    template <typename packetType>                              
    void CreateReplyPacket(packetType *repPack, 
                           ReplyPacket *ResponseFromMinion);                              
    // ipAddress should be written as "x.x.x.x" where  0=< x <= 254
    void AddMinion(std::string ipAddress);                             

    Reactor *m_reactor;
    DriverProxy m_proxy;
    boost::thread m_reactorRun;
    UdpSocket m_socket;
    std::vector<sockaddr_in> m_minionSockAddr;
    std::map<uint64_t, uint64_t> m_requests;
    
};
} // end of namespace ilrd

#endif // __MASTER_HPP__