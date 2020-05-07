#ifndef __MASTER_HPP__
#define __MASTER_HPP__

#include <boost/thread.hpp>             // boost::thread

#include "reactor.hpp"                  // class Reactor
#include "driver_proxy.hpp"             // class driver_procy
#include "udp_socket.hpp"	            // class UdpSocket
#include "packet.hpp"                   // class packet.hpp
#include "proxy_packet.hpp"             // RequestPacketRead, RequestPacketWrite
#include "../timer/timer.hpp"            // class timer

namespace ilrd
{
class Master
{
public:
    explicit Master(Reactor *reactor_, 
                    std::vector<std::pair<unsigned short, std::string> > &ipPortPairs_,
                    Timer *timer_);
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
    // Create read request packet for the minion
    void CreateReadRequestPacket(RequestPacket *reqPack, 
                                 RequestPacketRead const *pkFromProxy);
    // Create write request packet for the minion                             
    void CreateWriteRequestPacket(RequestPacket *reqPack, 
                                  RequestPacketWrite const *pkFromProxy);
    // Create read reply packet for proxy. Uses 
    // CreateReplyPacketExceptForData and CopyReadDataIntoPacket
    void CreateReplyPacketRead(ReplyPacketRead *repPack, 
                               ReplyPacket *ResponseFromMinion);
    // Create write reply packet for proxy                           
    // uses CreateReplyPacketExceptForData
    void CreateReplyPacketWrite(ReplyPacketWrite *repPack, 
                                ReplyPacket *ResponseFromMinion);

    // Create the common packet data between read and write reply packet
    // to proxy                            
    template <typename packetType>
    void CreateReplyPacketExceptForData(packetType *repPack, 
                                        ReplyPacket *ResponseFromMinion);
    
    // copy data section from minionResponse packet to packetToProxy
    void CopyReadDataIntoPacket(ReplyPacketRead *repPacketToProxy,
                                ReplyPacket *ResponseFromMinion);
    
    //  Add minions to master (port and ip address)
    //  ipAddress should be written as "x.x.x.x" where  0=< x <= 254
    void AddMinions(std::vector<std::pair<unsigned short, 
                                              std::string> > &ipPortPairs_);
    // This function will be invoked by reactor when a response is received
    // from minion                                          
    void MyCallback(int fd_);
    // invoked by timer when it is time
    void TimerCallback(struct RequestPacket *reqPack, uint64_t offset);
    // Create unique uid for master->minion packet since uid from nbd
    // is not necessarily uniqie
    uint64_t CreateUniqueUid();
    // Erase key uid from m_requests and m_masterMinionToNbdUid  
    void EraseSentPacketsInfoFromContainers(uint64_t uid);

    Reactor *m_reactor;
    DriverProxy m_proxy;
    boost::thread m_reactorRun;
    std::map<int, size_t> m_minionFds; // pairs of fd and minion index ?
    std::vector<UdpSocket *> m_minionSock; // sockets for each minion 
    // uid of Master minion 'network' form and len for each request
    std::map<uint64_t, uint64_t> m_requests; 
    // map of key = master to/from minion uid, value = nbd uid 
    // both uids are in their 'host' form
    std::map<uint64_t, uint64_t> m_masterMinionToNbdUid;
    size_t m_numOfMinions;
    // uid between master and minion, not related to nbd uid
    // because nbd uid won't nessecarily be unique
    uint64_t m_uidMasterMinion;
    Timer *m_timer; 
};
} // end of namespace ilrd

#endif // __MASTER_HPP__