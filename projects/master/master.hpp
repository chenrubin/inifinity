#ifndef __MASTER_HPP__
#define __MASTER_HPP__

#include <boost/thread.hpp>             // boost::thread

#include "reactor.hpp"                  // class Reactor
#include "driver_proxy.hpp"             // class driver_procy
#include "udp_socket.hpp"	            // class UdpSocket
#include "packet.hpp"                   // class packet.hpp
#include "proxy_packet.hpp"             // RequestPacketRead, RequestPacketWrite

namespace ilrd
{
class Master
{
public:
    explicit Master(Reactor *reactor_, 
                    std::vector<std::pair<unsigned short, std::string> > &ipPortPairs_);
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

    void CreateReplyPacketRead(ReplyPacketRead *repPack, 
                               ReplyPacket *ResponseFromMinion);
    void CreateReplyPacketWrite(ReplyPacketWrite *repPack, 
                                ReplyPacket *ResponseFromMinion);
    template <typename packetType>
    void CreateReplyPacketExceptForData(packetType *repPack, 
                                        ReplyPacket *ResponseFromMinion);
    
    void CopyReadDataIntoPacket(ReplyPacketRead *repPacketToProxy,
                                ReplyPacket *ResponseFromMinion);
    
    // ipAddress should be written as "x.x.x.x" where  0=< x <= 254
 //   void AddMinion(std::string ipAddress);
    void MyCallback(int fd_);

    Reactor *m_reactor;
    DriverProxy m_proxy;
    boost::thread m_reactorRun;
    std::map<int, size_t> m_minionFds; // pairs of fd and minion index ?
    std::vector<UdpSocket *> m_minionSock; // sockets for each minion 
    std::map<uint64_t, uint64_t> m_requests; // uid and len for each request
};
} // end of namespace ilrd

#endif // __MASTER_HPP__