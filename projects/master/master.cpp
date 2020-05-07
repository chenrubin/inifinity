#include <arpa/inet.h>          // htonl
//#define _BSD_SOURCE
#include <endian.h>             // htobe64
#include <errno.h>

#include "master.hpp"
#include "MyUtils.hpp"       // HandleErrorIfExists

#define BLOCK_SIZE (4096)
#define s_NUM_OF_BLOCKS (64)
#define MAX_UID_NUM (4294770687)

namespace ilrd
{
Master::Master(Reactor *reactor_, 
               std::vector<std::pair<unsigned short, std::string> > &ipPortPairs_,
               Timer *timer_)
    : m_reactor(reactor_)
    , m_proxy(m_reactor, boost::bind(&Master::onRead, this, _1), boost::bind(&Master::onWrite, this, _1))
    , m_reactorRun(boost::bind(&Reactor::Run, m_reactor))
    , m_minionFds()
    , m_minionSock()
    , m_requests()
    , m_masterMinionToNbdUid()
    , m_numOfMinions(ipPortPairs_.size())
    , m_uidMasterMinion(0)
    , m_timer(timer_)
{
 //   std::cout << "pairs size = " << ipPortPairs_.size() << "\n";

    AddMinions(ipPortPairs_);
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

    struct RequestPacket reqPack = {0};
    
    CreateReadRequestPacket(&reqPack, &pk);
    m_requests[reqPack.uid] = pk.len; // packet uid is entered into a container
    size_t minionIndex = LocateMinionIndexByOffset(pk.offset);
    std::cout << "***************Bytes len proxy -> master = " << pk.len << "\n";
    std::cout << "***************corresponding uid in the map  = " << be64toh(reqPack.uid) << "\n";
    std::cout << "***************Bytes offset = " << pk.offset << "\n";
    std::cout << "***************After locating index it = " << minionIndex << "\n";
    socklen_t size = sizeof(m_minionSock[minionIndex]->GetSockAddr());
    int sock_fd = m_minionSock[minionIndex]->GetFd();
    sockaddr_in sockAddress = m_minionSock[minionIndex]->GetSockAddr();
    HandleErrorIfExists(sendto(sock_fd, &reqPack, sizeof(reqPack), 0,
                 (struct sockaddr *)&sockAddress, size), "sendto failed"); 
    /*
        Enter into the timer :
        time = x , 
        callback : 
        if packet uid is still in container send packet
            and also set same task again.
    */
    std::cout << "!-!-!!-!-!!-!-!!-!-!!-!-!Inside TimerCallback  m_requests.size()" << m_requests.size() << "\n";
    std::cout << "}{}{}{}{}{}{}{}{}{reqPack.uid (host form) = " << reqPack.uid << "\n";
    m_timer->ScheduleAction(boost::chrono::milliseconds(1000),
                            boost::bind(&Master::TimerCallback, this, &reqPack, pk.offset));
}

void Master::onWrite(const RequestPacketWrite& pk)
{
    std::cout << "onWrite function\n";

    struct RequestPacket reqPack = {0};

    CreateWriteRequestPacket(&reqPack, &pk);
    std::cout << "After CreateWriteRequestPacket\n";
    m_requests[reqPack.uid] = pk.len;
    size_t minionIndex = LocateMinionIndexByOffset(pk.offset);
    std::cout << "***************Bytes len = " << pk.len << "\n"; 
    std::cout << "***************Bytes offset = " << pk.offset << "\n"; 
    std::cout << "***************After locating index it = " << minionIndex << "\n";
    socklen_t size = sizeof(m_minionSock[minionIndex]->GetSockAddr());
    int sock_fd = m_minionSock[minionIndex]->GetFd();
    std::cout << "before handleerrors\n";
    sockaddr_in sockAddress = m_minionSock[minionIndex]->GetSockAddr();
    HandleErrorIfExists(sendto(sock_fd, &reqPack, sizeof(reqPack), 0,
                 (struct sockaddr *)&sockAddress, size), "Failed to send");
    std::cout << "after sendto handleerrors\n";
    m_timer->ScheduleAction(boost::chrono::milliseconds(1000),
                            boost::bind(&Master::TimerCallback, this, &reqPack, pk.offset));
}

void Master::MyCallback(int fd_)
{
    struct ReplyPacket repPack;
    memset(&repPack, 0, sizeof(repPack));
    size_t minionIndex = m_minionFds[fd_];
    std::cout << "inside MyCallback Minion index = " << minionIndex << "\n";
    socklen_t size = sizeof(m_minionSock[minionIndex]->GetSockAddr());
    sockaddr_in sockAddress = m_minionSock[minionIndex]->GetSockAddr();
    HandleErrorIfExists(recvfrom(fd_, &repPack, sizeof(repPack), 0, 
             (struct sockaddr *)&sockAddress, &size), "Failed recvfrom");

    std::cout << "repPack.status = " << static_cast<int>(repPack.status) << std::endl;
    std::cout << "repPack.type = " << static_cast<int>(repPack.type) << std::endl;
    std::cout << "repPack.uid = " << repPack.uid << std::endl;
    std::cout << "repPack.data = " << repPack.data << std::endl;

    if (repPack.type == 0)
    {
        ReplyPacketRead repPacketToProxy;
        CreateReplyPacketRead(&repPacketToProxy, &repPack);
        std::cout << "Inside MyCallback After CreateReplyPacketRead\n";
        m_proxy.ReplyRead(repPacketToProxy);
        std::cout << "Inside MyCallback After m_proxy.ReplyRead(repPacketToProxy);\n";
    }
    else
    {
        ReplyPacketWrite repPacketToProxy;
        CreateReplyPacketWrite(&repPacketToProxy, &repPack);
        m_proxy.ReplyWrite(repPacketToProxy);
    }
    
    // need to erase uid from all containers
    std::cout << "$#%%$#@%$$%About to EraseSentPacketsInfoFromContainers \n";
    EraseSentPacketsInfoFromContainers(repPack.uid);
}

size_t Master::LocateMinionIndexByOffset(uint64_t offset)
{
    size_t blocksPerMinion = s_NUM_OF_BLOCKS / m_numOfMinions; 

    return (offset / ( blocksPerMinion * BLOCK_SIZE));
}

size_t Master::LocateLocalBlockIndexByOffset(uint64_t offset)
{
    size_t blocksPerMinion = s_NUM_OF_BLOCKS / m_numOfMinions; 

    return ((offset % (blocksPerMinion * BLOCK_SIZE)) / 
                                         BLOCK_SIZE);
}

void Master::CreateReadRequestPacket(RequestPacket *reqPack, 
                                     RequestPacketRead const *pkFromProxy)
{
    reqPack->type = 0;
    uint64_t newUid = CreateUniqueUid();
    reqPack->uid = htobe64(newUid);//htobe64(pkFromProxy->uid);
    std::cout << "packet proxy->master uid  = " << pkFromProxy->uid << "\n";
    std::cout << "packet master->minion uid = " << newUid << "\n";
    m_masterMinionToNbdUid[newUid] = pkFromProxy->uid;
    reqPack->blockIndex = htobe64(LocateLocalBlockIndexByOffset(pkFromProxy->offset));
}

void Master::CreateWriteRequestPacket(RequestPacket *reqPack, 
                                      RequestPacketWrite const *pkFromProxy)
{
    reqPack->type = 1;
    uint64_t newUid = CreateUniqueUid();
    reqPack->uid = htobe64(newUid);//htobe64(pkFromProxy->uid);
    std::cout << "packet proxy->master uid  = " << pkFromProxy->uid << "\n";
    std::cout << "packet master->minion uid = " << newUid << "\n";
    m_masterMinionToNbdUid[newUid] = pkFromProxy->uid;
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
    std::cout << "Inside end of CreateReplyPacketRead\n";
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
    repPack->uid = m_masterMinionToNbdUid[be64toh(ResponseFromMinion->uid)];//be64toh(ResponseFromMinion->uid);
    std::cout << "packet minion->master uid = " << be64toh(ResponseFromMinion->uid) << "\n";
    std::cout << "packet master->proxy uid  = " << repPack->uid << "\n";
    repPack->status = ResponseFromMinion->status;
    std::cout << "status = " << static_cast<int>(repPack->status) << "\n";
    repPack->len = m_requests[ResponseFromMinion->uid]; // problem here
    std::cout << "len = " << repPack->len << "\n";
}                                    

void Master::CopyReadDataIntoPacket(ReplyPacketRead *repPacketToProxy,
                                    ReplyPacket *ResponseFromMinion)
{
    std::cout << "Inside CopyReadDataIntoPacket\n";
    std::cout << "len of data sent from master back to proxy = " << repPacketToProxy->len << "\n";
    std::copy(ResponseFromMinion->data, 
              ResponseFromMinion->data + /*repPacketToProxy->len*/4096, 
              std::back_inserter(repPacketToProxy->data));
    std::cout << "Inside end of  CopyReadDataIntoPacket\n";
}

void Master::AddMinions(std::vector<std::pair<unsigned short, 
                                              std::string> > &ipPortPairs_)
{
    size_t size = ipPortPairs_.size();
     for (size_t i = 0; i < size; ++i)
    {
        std::cout << "!!!!!!!!!1inside minion socket loop!!!!!!!!!!!!!!1\n"; 
//        std::cout << "Start of Ctor\n";
//        std::cout << "ipPortPairs_[/*i*/0].first = " << ipPortPairs_[i].first << "\n";
//        std::cout << "ipPortPairs_[/*i*/0].second = " << ipPortPairs_[/*i*/0].second << "\n";
        UdpSocket *sock = new UdpSocket(ipPortPairs_[i].first, ipPortPairs_[i].second ,false); // dont forget to delete
        
//        std::cout << "After udpSock\n";
//        std::cout << "About to push back\n";
        m_minionSock.push_back(sock);
//        std::cout << "push back\n";
//        std::cout << "before sock.GetFd\n";

//        std::cout << "about tp o,ap\n";
        m_minionFds[sock->GetFd()] = i;
//        std::cout << "before adddFd\n";
        m_reactor->AddFd(sock->GetFd(), Reactor::READ, boost::bind(&Master::MyCallback, this, _1));
//        std::cout << "after adddFd\n";
    }
}

uint64_t Master::CreateUniqueUid()
{
    ++m_uidMasterMinion;

    if (MAX_UID_NUM == m_uidMasterMinion)
    {
        m_uidMasterMinion = 0;
    }

    return m_uidMasterMinion;
}

void Master::EraseSentPacketsInfoFromContainers(uint64_t uid)
{
    std::cout << "!&!&!&!&! About to erase from containers\n";
    if (1 != m_requests.erase(uid))
    {
        perror("Failed to Erase from m_requests");
    }
    if (1 != m_masterMinionToNbdUid.erase(htobe64(uid)))
    {
        perror("Failed to Erase from m_masterMinionToNbdUid");
    }
}

void Master::TimerCallback(struct RequestPacket *reqPack, uint64_t offset)
{
    std::cout << "!-!-!!-!-!!-!-!!-!-!!-!-!Inside TimerCallback  m_requests.size()" << m_requests.size() << "\n";
    std::cout << "!-!-!!-!-!!-!-!!-!-!!-!-!Inside TimerCallback reqPack->uid = " << reqPack->uid << "\n";
    std::cout << "!-!-!!-!-!!-!-!!-!-!!-!-!Inside TimerCallback reqPack->type" << static_cast<int>(reqPack->type) << "\n";  
    std::cout << "m_requests.count(reqPack->uid) = " << m_requests.count(reqPack->uid) << "\n";
    if (1 == m_requests.count(reqPack->uid))
    {
        std::cout << "inside loop of timer\n";
        size_t minionIndex = LocateMinionIndexByOffset(offset);
        socklen_t size = sizeof(m_minionSock[minionIndex]->GetSockAddr());
        int sock_fd = m_minionSock[minionIndex]->GetFd();
        sockaddr_in sockAddress = m_minionSock[minionIndex]->GetSockAddr();

        HandleErrorIfExists(sendto(sock_fd, reqPack, sizeof(*reqPack), 0,
                 (struct sockaddr *)&sockAddress, size), "sendto failed");
        m_timer->ScheduleAction(boost::chrono::milliseconds(1000),
                                boost::bind(&Master::TimerCallback, this, reqPack, offset));
    }
}
} // end of namespace ilrd