#include <arpa/inet.h>          // htonl
//#define _BSD_SOURCE
#include <endian.h>             // htobe64
#include <errno.h>

#include "master.hpp"
#include "MyUtils.hpp"       // HandleErrorIfExists

#define BLOCK_SIZE (4096)
#define s_NUM_OF_BLOCKS (128)
#define MASTER_PORT (11111)
#define MINION_PORT (12345)

namespace ilrd
{

Master::Master(Reactor *reactor_)
    : m_reactor(reactor_)
    , m_proxy(m_reactor, boost::bind(&Master::onRead, this, _1), boost::bind(&Master::onWrite, this, _1))
    , m_reactorRun(boost::bind(&Reactor::Run, m_reactor))
    , m_socket(MASTER_PORT, INADDR_ANY, SO_REUSEADDR, false)
    , m_minionSockAddr()
    , m_requests()
{
/*    try
    {
        m_reactorRun = boost::thread(boost::bind(&Reactor::Run, m_reactor));
    }
    catch(const std::exception& e)
    {
        std::cout << "Inside catch\n";
        std::cerr << e.what() << '\n';
        perror("perror failed");
    }
  */  
    AddMinion("192.168.1.20");
    //AddMinion("127.0.0.1");
 //   m_reactor->AddFd(m_socket, Reactor::READ, )
}

Master::~Master()
{
    m_reactorRun.join();
}

// When read action is wanted master recieves RequestPacketRead (from proxy)
// and converts it to a packet the minion can understand
void Master::onRead(const RequestPacketRead& pk)
{
    std::cout << "onRead function\n";

    struct RequestPacket reqPack;
    
    CreateReadRequestPacket(&reqPack, &pk);
    m_requests[pk.uid] = pk.len;
    size_t minionIndex = LocateMinionIndexByOffset(pk.offset);
    socklen_t size = sizeof(m_minionSockAddr[minionIndex]);

    HandleErrorIfExists(sendto(m_socket.GetFd(), &reqPack, sizeof(reqPack), 0,
                 (struct sockaddr *)&m_minionSockAddr[minionIndex], size), "sendto failed");

    struct ReplyPacket repPack;
    memset(&repPack, 0, sizeof(repPack));
    HandleErrorIfExists(recvfrom(m_socket.GetFd(), &repPack, sizeof(repPack), 0, 
             (struct sockaddr *)&m_minionSockAddr[minionIndex], &size), "Failed recvfrom");

    std::cout << "repPack.status = " << static_cast<int>(repPack.status) << std::endl;
    std::cout << "repPack.type = " << static_cast<int>(repPack.type) << std::endl;
    std::cout << "repPack.uid = " << repPack.uid << std::endl;
    std::cout << "repPack.data = " << repPack.data << std::endl;

    ReplyPacketRead repPacketToProxy;
    CreateReplyPacketRead(&repPacketToProxy, &repPack);
    m_proxy.ReplyRead(repPacketToProxy);
}

void Master::onWrite(const RequestPacketWrite& pk)
{
    std::cout << "onWrite function\n";

    struct RequestPacket reqPack;

    CreateWriteRequestPacket(&reqPack, &pk);
    std::cout << "After CreateWriteRequestPacket\n";
    m_requests[pk.uid] = pk.len;
    size_t minionIndex = LocateMinionIndexByOffset(pk.offset);
    socklen_t size = sizeof(m_minionSockAddr[minionIndex]);
    std::cout << "before handleerrors\n";
    HandleErrorIfExists(sendto(m_socket.GetFd(), &reqPack, sizeof(reqPack), 0,
                 (struct sockaddr *)&m_minionSockAddr[minionIndex], size), "Failed to send");
    std::cout << "after sendto handleerrors\n";             

    struct ReplyPacket repPack;
    std::cout << "struct ReplyPacket repPack\n";
    memset(&repPack, 0, sizeof(repPack));
    std::cout << "After memset\n";
    std::cout << "inside onwrite before recvfrom\n";
    HandleErrorIfExists(recvfrom(m_socket.GetFd(), &repPack, sizeof(repPack), 0, 
             (struct sockaddr *)&m_minionSockAddr[minionIndex], &size), "Failed recvfrom");
    std::cout << "inside onwrite after recvfrom\n";
    std::cout << "repPack.status = " << static_cast<int>(repPack.status) << std::endl;
    std::cout << "repPack.type = " << static_cast<int>(repPack.type) << std::endl;
    std::cout << "repPack.uid = " << repPack.uid << std::endl;
    std::cout << "repPack.data = " << repPack.data << std::endl; 

    ReplyPacketWrite repPacketToProxy;
    CreateReplyPacketWrite(&repPacketToProxy, &repPack);
    m_proxy.ReplyWrite(repPacketToProxy);
}

size_t Master::LocateMinionIndexByOffset(uint64_t offset)
{
    return (offset / (s_NUM_OF_BLOCKS * BLOCK_SIZE));
}

size_t Master::LocateLocalBlockIndexByOffset(uint64_t offset)
{
    return ((offset % ((s_NUM_OF_BLOCKS * BLOCK_SIZE))) / BLOCK_SIZE);
}

void Master::CreateReadRequestPacket(RequestPacket *reqPack, 
                                     RequestPacketRead const *pkFromProxy)
{
    reqPack->type = 0;
    reqPack->uid = htobe64(pkFromProxy->uid);
    reqPack->blockIndex = htobe64(LocateLocalBlockIndexByOffset(pkFromProxy->offset));
}

void Master::CreateWriteRequestPacket(RequestPacket *reqPack, 
                                      RequestPacketWrite const *pkFromProxy)
{
    reqPack->type = 1;
    reqPack->uid = htobe64(pkFromProxy->uid);
    reqPack->blockIndex = htobe64(LocateLocalBlockIndexByOffset(pkFromProxy->offset));
    std::cout << "Inside CreateWriteRequestPacket before copy data from vector\n";
    std::copy(pkFromProxy->data.begin(), pkFromProxy->data.end(), reqPack->data);
    std::cout << "After copy\n";
}

void Master::CreateReplyPacketRead(ReplyPacketRead *repPack, 
                                   ReplyPacket *ResponseFromMinion)
{
    CreateReplyPacketExceptForData(repPack, ResponseFromMinion);
    CopyReadDataIntoPacket(repPack, ResponseFromMinion);
}

void Master::CreateReplyPacketWrite(ReplyPacketWrite *repPack, 
                                    ReplyPacket *ResponseFromMinion)
{
    CreateReplyPacketExceptForData(repPack, ResponseFromMinion);
}

template <typename packetType>
void Master::CreateReplyPacketExceptForData(packetType *repPack, 
                                            ReplyPacket *ResponseFromMinion)
{
    std::cout << "!!!!!!!!!!!!!!Inside CreateReplyPacketExceptForData\n";
    repPack->uid = be64toh(ResponseFromMinion->uid);
    repPack->status = ResponseFromMinion->status;
    std::cout << "status = " << static_cast<int>(repPack->status) << "\n";
    repPack->len = m_requests[repPack->uid];
    std::cout << "len = " << repPack->len << "\n";
}                                    

void Master::CopyReadDataIntoPacket(ReplyPacketRead *repPacketToProxy,
                                    ReplyPacket *ResponseFromMinion)
{
    std::cout << "Inside CopyReadDataIntoPacket\n";
    std::copy(ResponseFromMinion->data, 
              ResponseFromMinion->data + repPacketToProxy->len, 
              std::back_inserter(repPacketToProxy->data));
}

void Master::AddMinion(std::string ipAddress)
{
    sockaddr_in minionAddr;

    minionAddr.sin_family = AF_INET;
    minionAddr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
    minionAddr.sin_port = htons(MINION_PORT);

    m_minionSockAddr.push_back(minionAddr);
}
} // end of namespace ilrd