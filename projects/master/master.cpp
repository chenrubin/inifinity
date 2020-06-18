#include <arpa/inet.h>  // htonl
#include <endian.h>     // htobe64
#include <errno.h>
#include <algorithm>    // std::for_each

#include "master.hpp"
#include "../../cpp_fs/utils/MyUtils.hpp" // HandleErrorIfExists

#define BLOCK_SIZE (4096)
#define s_NUM_OF_BLOCKS (96)
#define MAX_UID_NUM (4294770687)
#define MAX_TIMER_ITERATION (5)
#define COPIES_PER_PACKET (2)

namespace ilrd
{
Master::Master(Reactor *reactor_,
               std::vector<std::pair<unsigned short, std::string> > &ipPortPairs_,
               Timer *timer_,
               ThreadPool *threadPool_,
               Eventer *eventer_)
    : m_reactor(reactor_)
    , m_proxy(m_reactor, boost::bind(&Master::onRead, this, _1), boost::bind(&Master::onWrite, this, _1))
    , m_reactorRun(boost::bind(&Reactor::Run, m_reactor))
    , m_minionFds()
    , m_minionSock()
    , m_requests()
    , m_masterMinionToNbdUid()
    , m_uidToLength()
    , m_numOfMinions(ipPortPairs_.size())
    , m_uidMasterMinion(0)
    , m_timer(timer_)
    , m_threadPool(threadPool_)
    , m_eventer(eventer_)
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
void Master::onRead(const RequestPacketRead &pk)
{
    std::cout << "onRead function\n";

    m_uidToLength[pk.uid] = pk.len;
    std::cout << "After m_uidToLength[pk.uid] = pk.len;\n";
    struct RequestPacket reqPack = {0};
    

    CreateReadRequestPacket(&reqPack, &pk);
    size_t minionIndex = LocateMinionIndexByOffset(pk.offset);
    if (m_minionSock[minionIndex].second == true)
    {
        int status1 = SetParametersAndSendRequestIMP(minionIndex, &reqPack);
        m_timer->ScheduleAction(boost::chrono::milliseconds(1000),
                            boost::bind(&Master::TimerCallback, this, &reqPack, 
                                        minionIndex, MAX_TIMER_ITERATION));
    }
    else
    {
        minionIndex = (minionIndex + 1) % m_numOfMinions;
        if (m_minionSock[minionIndex].second == true)
        {
            reqPack.blockIndex += htobe64(s_NUM_OF_BLOCKS / m_numOfMinions);
            int status1 = SetParametersAndSendRequestIMP(minionIndex, &reqPack);
            m_timer->ScheduleAction(boost::chrono::milliseconds(1000),
                            boost::bind(&Master::TimerCallback, this, &reqPack, 
                                        minionIndex,  MAX_TIMER_ITERATION));
        }
    }                                                             
}

int Master::SetParametersAndSendRequestIMP(size_t minionIndex, RequestPacket *reqPack)
{
    std::cout << "Inside SetParametersAndSendRequestIMP before sizeof GetSockAddr()\n"; 
    std::cout << "minionIndex = " << minionIndex << "\n";
    socklen_t size = sizeof(m_minionSock[minionIndex].first->GetSockAddr());
    std::cout << "Inside SetParametersAndSendRequestIMP before GetFd()\n"; 
    std::cout << "minion index = " << minionIndex << "\n";
    int sock_fd = m_minionSock[minionIndex].first->GetFd();
    std::cout << "Inside SetParametersAndSendRequestIMP before GetSockAddr()\n"; 
    sockaddr_in sockAddress = m_minionSock[minionIndex].first->GetSockAddr();
    std::cout << "Inside SetParametersAndSendRequestIMP before sendto\n"; 
    return (sendto(sock_fd, reqPack, sizeof(*reqPack), 0,
                  (struct sockaddr *)&sockAddress, size));
    std::cout << "Inside SetParametersAndSendRequestIMP after sendto\n";                  
}

void Master::onWrite(const RequestPacketWrite &pk)
{
    std::cout << "onWrite function\n";
    m_uidToLength[pk.uid] = pk.len;
    // enter uid and appropriate variant request packets to container m_requests
    // save pk in the m_requests map<uid, pair of variant of RequestPacketWrite/RequestPacketRead
    // and reply packet from minion>
    requestOrReplyPacket_t reqPackFromProxy = pk;
 //   ReplyPacket repPack = {0};

    uint64_t eventId = CreateUniqueUid();
/*    std::pair<requestOrReplyPacket_t, size_t> packetAndCounter = 
                    std::make_pair(reqPackFromProxy, COPIES_PER_PACKET);
 */ m_requests[eventId/*host form*/] = /*packetAndCounter*/reqPackFromProxy;
 //   std::cout << "eventId = "<< eventId << " and m_requests[eventId].counter = " << m_requests[eventId].second << "\n";
        // setEvent in Eventer
        // Add task to threadpool
    m_eventer->SetEvent(eventId, boost::bind(&Master::CallbackSendPacketToMinion, this, eventId));
    m_threadPool->AddTask(boost::bind(&Master::IncryptionFunc, this, eventId), 
                                                            ThreadPool::HIGH);
}

void Master::CallbackSendPacketToMinion(uint64_t eventId)
{
    std::cout << "Inside CallbackSendPacketToMinion\n";
    struct RequestPacket reqPack = {0};

    RequestPacketWrite pk = boost::get<RequestPacketWrite>(m_requests[eventId/*host form*/]/*.first*/);

    CreateWriteRequestPacket(&reqPack, &pk, eventId);
    size_t minionIndex = LocateMinionIndexByOffset(pk.offset);
    if (m_minionSock[minionIndex].second == true)
    {
        int status1 = SetParametersAndSendRequestIMP(minionIndex, &reqPack);
        m_timer->ScheduleAction(boost::chrono::milliseconds(1000),
                                boost::bind(&Master::TimerCallback, this, 
                                            &reqPack, minionIndex, 
                                            MAX_TIMER_ITERATION));
    }
    
    minionIndex = (minionIndex + 1) % m_numOfMinions;
    if (m_minionSock[minionIndex].second == true)
    {
        //std::cout << "After SetParametersAndSendRequestIMP status1 = " << status1 << "\n";
        reqPack.blockIndex += htobe64(s_NUM_OF_BLOCKS /  m_numOfMinions);
        std::cout << "@ @   @After reqPack.blockIndex = " << reqPack.blockIndex << "\n";
        int status2 = SetParametersAndSendRequestIMP(minionIndex, &reqPack);
        std::cout << "After extra SetParametersAndSendRequestIMP status2 = " << status2 << "\n";                                                   
        m_timer->ScheduleAction(boost::chrono::milliseconds(1000),
                                boost::bind(&Master::TimerCallback, this, 
                                            &reqPack, minionIndex, 
                                            MAX_TIMER_ITERATION));
    }
    
 /*   if ((-1 == status1) && (-1 == status2))
    {
        perror("Two minions are down");
        throw std::runtime_error("Two minions are down");
    }                                               
 */   
  //  reqPack.blockIndex -= htobe64(s_NUM_OF_BLOCKS /  m_numOfMinions);
//    std::cout << "@ @   @After negative reqPack.blockIndex = " << reqPack.blockIndex << "\n";
/*    m_timer->ScheduleAction(boost::chrono::milliseconds(1000),
                            boost::bind(&Master::TimerCallback, this, 
                                        &reqPack, pk.offset, MAX_TIMER_ITERATION));*/
}

// This function will be invoked by reactor when a response is received
// from minion  
void Master::MyCallback(int fd_)
{
    struct ReplyPacket repPack;
    memset(&repPack, 0, sizeof(repPack));
    size_t minionIndex = m_minionFds[fd_];
//    std::cout << "inside MyCallback Minion index = " << minionIndex << "\n";
    socklen_t size = sizeof(m_minionSock[minionIndex].first->GetSockAddr());
    sockaddr_in sockAddress = m_minionSock[minionIndex].first->GetSockAddr();
    HandleErrorIfExists(recvfrom(fd_, &repPack, sizeof(repPack), 0,
                                 (struct sockaddr *)&sockAddress, &size),
                        "Failed recvfrom");

    if (repPack.type == 0) // Read
    {
        // save repPack in a container map (uid/eventid, ReplyPacket)
        uint64_t packetFromMinionUid = repPack.uid;
        m_requests[be64toh(packetFromMinionUid)/*host form*/]/*.first*/ = repPack;
        // send to decryption function
        // do all the rest below in a callback the master set in eventer

        m_eventer->SetEvent(packetFromMinionUid, boost::bind(&Master::CallbackSendPacketToProxy, this, packetFromMinionUid));
        m_threadPool->AddTask(boost::bind(&Master::DecryptionFunc, this, packetFromMinionUid), 
                                                                     ThreadPool::HIGH);
    }
    else
    {
    //    std::cout << "& ^   &   ^ m_requests[repPack.uid].second = " << m_requests[be64toh(repPack.uid)/*host form*/].second << "\n";
    //    std::cout << "eventId = " << repPack.uid << "\n";
     //   --m_requests[be64toh(repPack.uid)/*host form*/].second;
        
        if (1 == m_requests.count(be64toh(repPack.uid)))
        {
            std::cout << "____!!!!!_____!!!!!___inside if (0 == m_requests[repPack.uid].second)\n";
            ReplyPacketWrite repPacketToProxy;
            CreateReplyPacketWrite(&repPacketToProxy, &repPack);
            // need to invoke this only after both packets (actual and backup)COPIES_PER_PACKET
            // returned 
            m_proxy.ReplyWrite(repPacketToProxy); 
            EraseSentPacketsInfoFromContainers(repPack.uid);
        }        
    }
}

void Master::CallbackSendPacketToProxy(uint64_t eventId)
{
    std::cout << "------!!!!!!!---------Inside CallbackSendPacketToProxy\n";
    ReplyPacketRead repPacketToProxy;
    ReplyPacket pk = boost::get<ReplyPacket>(m_requests[be64toh(eventId)/*host form*/]);

    CreateReplyPacketRead(&repPacketToProxy, &pk);
 //   std::cout << "Inside MyCallback After CreateReplyPacketRead\n";
    m_proxy.ReplyRead(repPacketToProxy);
//    std::cout << "Inside MyCallback After m_proxy.ReplyRead(repPacketToProxy);\n";
     // need to erase uid from all containers
 //   std::cout << "$#%%$#@%$$%About to EraseSentPacketsInfoFromContainers \n";
    EraseSentPacketsInfoFromContainers(eventId);
}

size_t Master::LocateMinionIndexByOffset(uint64_t offset)
{
 /*   std::cout << "offset = " << offset << "\n"; 
    std::cout << "s_NUM_OF_BLOCKS = " << s_NUM_OF_BLOCKS << "\n";
    std::cout << "m_numOfMinions = " << m_numOfMinions << "\n";
 */   size_t blocksPerMinion = s_NUM_OF_BLOCKS / m_numOfMinions;
 //   std::cout << "blocksPerMinion = " << blocksPerMinion << "\n";
 //   std::cout << "going to return " << offset / (blocksPerMinion * BLOCK_SIZE) << "\n";
    return (offset / (blocksPerMinion * BLOCK_SIZE));
}

size_t Master::LocateLocalBlockIndexByOffset(uint64_t offset)
{
    size_t blocksPerMinion = s_NUM_OF_BLOCKS / m_numOfMinions;

    std::cout << "Going to return = " << (offset % (blocksPerMinion * BLOCK_SIZE)) / BLOCK_SIZE << "\n";
    return ((offset % (blocksPerMinion * BLOCK_SIZE)) /
            BLOCK_SIZE);
}

void Master::CreateReadRequestPacket(RequestPacket *reqPack,
                                     RequestPacketRead const *pkFromProxy)
{
    reqPack->type = 0;
    uint64_t newUid = CreateUniqueUid();
    reqPack->uid = htobe64(newUid); //htobe64(pkFromProxy->uid);
 //   std::cout << "packet proxy->master uid  = " << pkFromProxy->uid << "\n";
 //   std::cout << "packet master->minion uid = " << newUid << "\n";
    m_masterMinionToNbdUid[newUid] = pkFromProxy->uid;
    reqPack->blockIndex = htobe64(LocateLocalBlockIndexByOffset(pkFromProxy->offset));
}

void Master::CreateWriteRequestPacket(RequestPacket *reqPack,
                                      RequestPacketWrite const *pkFromProxy,
                                      uint64_t uid)
{
    reqPack->type = 1;
//    uint64_t newUid = CreateUniqueUid();
    reqPack->uid = htobe64(uid); //htobe64(pkFromProxy->uid);
 //   std::cout << "packet proxy->master uid  = " << pkFromProxy->uid << "\n";
 //   std::cout << "packet master->minion uid = " << uid << "\n";
    m_masterMinionToNbdUid[uid] = pkFromProxy->uid;
    reqPack->blockIndex = htobe64(LocateLocalBlockIndexByOffset(pkFromProxy->offset));

 //   std::cout << "Inside CreateWriteRequestPacket before copy data from vector\n";
    std::copy(pkFromProxy->data.begin(), pkFromProxy->data.end(), reqPack->data);
 //   std::cout << "After copy\n";
}

void Master::CreateReplyPacketRead(ReplyPacketRead *repPack,
                                   ReplyPacket *ResponseFromMinion)
{
    CreateReplyPacketExceptForData(repPack, ResponseFromMinion);
    CopyReadDataIntoPacket(repPack, ResponseFromMinion);
 //   std::cout << "Inside end of CreateReplyPacketRead\n";
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
//    std::cout << "!!!!!!!!!!!!!!Inside CreateReplyPacketExceptForData\n";
    repPack->uid = m_masterMinionToNbdUid[be64toh(ResponseFromMinion->uid)]; //be64toh(ResponseFromMinion->uid);
 //   std::cout << "packet minion->master uid = " << be64toh(ResponseFromMinion->uid) << "\n";
//    std::cout << "packet master->proxy uid  = " << repPack->uid << "\n";
    repPack->status = ResponseFromMinion->status;
 //   std::cout << "status = " << static_cast<int>(repPack->status) << "\n";
    repPack->len = m_uidToLength[m_masterMinionToNbdUid[be64toh(ResponseFromMinion->uid)]];//m_requests[ResponseFromMinion->uid]; // problem here
//    std::cout << "len = " << repPack->len << "\n";
}

void Master::CopyReadDataIntoPacket(ReplyPacketRead *repPacketToProxy,
                                    ReplyPacket *ResponseFromMinion)
{
 //   std::cout << "Inside CopyReadDataIntoPacket\n";
 //   std::cout << "len of data sent from master back to proxy = " << repPacketToProxy->len << "\n";
    std::copy(ResponseFromMinion->data,
              ResponseFromMinion->data + repPacketToProxy->len,
              std::back_inserter(repPacketToProxy->data));
 //   std::cout << "Inside end of  CopyReadDataIntoPacket\n";
}

void Master::AddMinions(std::vector<std::pair<unsigned short,
                                              std::string> > &ipPortPairs_)
{
    size_t size = ipPortPairs_.size();
    for (size_t i = 0; i < size; ++i)
    {
        UdpSocket *sock = new UdpSocket(ipPortPairs_[i].first, ipPortPairs_[i].second, false); // dont forget to delete
        std::pair<UdpSocket *, bool> pair = std::make_pair(sock, true);
        m_minionSock.push_back(pair);
        m_minionFds[sock->GetFd()] = i;
        m_reactor->AddFd(sock->GetFd(), Reactor::READ, boost::bind(&Master::MyCallback, this, _1));
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
    std::cout << "*********!!!!!!*****!!!!!!!erase\n";
    assert(1 == m_masterMinionToNbdUid.erase(htobe64(uid)));
    assert(1 == m_requests.erase(be64toh(uid)));
}

void Master::TimerCallback(struct RequestPacket *reqPack, size_t minionIndex,  
                                                          size_t count)
{
    static size_t counterForDebugInsideLoop = 0;
    static size_t counterForDebugInsideTimerCallback = 0;
    ++counterForDebugInsideTimerCallback;
    std::cout << "counterForDebugInsideLoop = " << counterForDebugInsideLoop << "\n";
    std::cout << "counterForDebugInsideTimerCallback = " << counterForDebugInsideTimerCallback << "\n";

 //   size_t minionIndex = LocateMinionIndexByOffset(offset);
    int status1 = 0;
    int status2 = 0;
    std::cout << "m_masterMinionToNbdUid.count(be64toh(reqPack->uid)) = " << m_masterMinionToNbdUid.count(be64toh(reqPack->uid)) << "\n"; 
    std::cout << "m_masterMinionToNbdUid.count(reqPack->uid) = " << m_masterMinionToNbdUid.count(reqPack->uid) << "\n"; 
    if (1 == m_masterMinionToNbdUid.count(be64toh(reqPack->uid)) && (0 < count))
    {  
        ++counterForDebugInsideLoop;
        std::cout << "Inside if (1 == m_masterMinionToNbdUid.count(be64toh(reqPack->uid)) && (0 < count))\n";
        status1 = SetParametersAndSendRequestIMP(minionIndex, reqPack);
 /*       std::cout << "Inside TimerCallback status1 = " << status1 << "\n";
        if (-1 == status1)
        {
            std::cout << "first -1 == status1 , eventId = " << be64toh(reqPack->uid) << "\n";
            --m_requests[be64toh(reqPack->uid)].second;
        }

        reqPack->blockIndex += htobe64(s_NUM_OF_BLOCKS / m_numOfMinions);
        if (1 == reqPack->type) // WRITE
        {   
            if (-1 == SetParametersAndSendRequestIMP((minionIndex + 1) % m_numOfMinions, reqPack))
            {
                --m_requests[be64toh(reqPack->uid)].second;
            }
        }
        else if (-1 == status1)
        {
            status2 = SetParametersAndSendRequestIMP((minionIndex + 1) % m_numOfMinions, reqPack);
        }
        reqPack->blockIndex -= htobe64(s_NUM_OF_BLOCKS / m_numOfMinions);
*/
        m_timer->ScheduleAction(boost::chrono::milliseconds(1000),
                                boost::bind(&Master::TimerCallback, this, reqPack, 
                                            minionIndex, count - 1));
    }
    else
    {
        if (1 == m_masterMinionToNbdUid.count(be64toh(reqPack->uid))) // This means minion is dead
        {
            m_minionSock[minionIndex].second = false;
        }
        else // might be redundant
        {
            m_requests.erase(be64toh(reqPack->uid/*host form*/));
        }
    }
}

void Master::IncryptionFunc(uint64_t eventId)
{
    std::cout << "**********************Incrypt data of packet from proxy \n";
    std::vector<char>& packet_data = boost::get<RequestPacketWrite>(m_requests[eventId/*host*/]).data;
    //std::cout << "First element in vector before incrypt = " << static_cast<int>(boost::get<RequestPacketWrite>(m_requests[eventId]).data[0]) << "\n";
   // std::cout << "First element in vector (packet_data) before incrypt = " << static_cast<int>(packet_data[0]) << "\n";
    std::cout << "Inside incrypt before for each \n";
    std::for_each(packet_data.begin(), packet_data.end(), Incrypt_Algo());

 //  std::cout << "First element in vector after incrypt = " << static_cast<int>(boost::get<RequestPacketWrite>(m_requests[eventId]).data[0]) << "\n";
 //   std::cout << "First element in vector (packet_data) after incrypt = " << static_cast<int>(packet_data[0]) << "\n";
//    std::cout << "!!!   !!! !!! len of std::get.data to decrypt = " << (boost::get<ReplyPacket>(m_requests[eventId]).data).size() << "\n";
    std::cout << "Inside incrypt before SignalEvent\n";
    m_eventer->SignalEvent(eventId);
    std::cout << "end of Inside incrypt after SignalEvent\n";
}

void Master::DecryptionFunc(uint64_t eventId)
{/*    if (1 != m_requests.erase(uid))
    {
        perror("Failed to Erase from m_requests");
    }
 */
    std::cout << "***********************8Decrypt read packet from minion\n";
    char *packet_data = boost::get<ReplyPacket>(m_requests[be64toh(eventId)/*host form*/]).data;
    size_t len = strlen(packet_data);
 //   std::cout << "!!!   !!! !!! len of std::get.data to decrypt = " << strlen(boost::get<ReplyPacket>(m_requests[eventId]).data) << "\n";
 //   std::cout << "!!!   !!! !!! len of packet_data to decrypt = " << len << "\n";
    for (size_t i = 0; i < len; ++i)
    {
        packet_data[i] = DecryptAlgo(packet_data[i]);
    }

    m_eventer->SignalEvent(eventId);
}

void Master::Incrypt_Algo::operator()(char &val)
{
    val = val ^ 0xFF; 
}

char Master::DecryptAlgo(char val)
{
    return (val ^ 0xFF);
}
} // end of namespace ilrd