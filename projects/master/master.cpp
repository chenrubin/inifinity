#include <arpa/inet.h>          // htonl
#include <endian.h>             // htobe64

#include "master.hpp"
#include "MyUtils.hpp"       // HandleErrorIfExists

#define BLOCK_SIZE (4096)
#define MASTER_PORT (11111)
#define MINION_PORT (12345)

namespace ilrd
{

Master::Master(Reactor *reactor_)
    : m_reactor(reactor_)
//    , m_proxy(m_reactor, boost::bind(&Master::onRead, this, _1), boost::bind(&Master::onWrite, this, _1))
 //   , m_reactorRun(boost::bind(&Reactor::Run, m_reactor))
    , m_socket(MASTER_PORT, INADDR_ANY, SO_REUSEADDR, false)
{
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
    
    reqPack.type = 0; // READ
    reqPack.uid = htobe64(pk.uid);
    reqPack.blockIndex = htobe64(pk.offset) / BLOCK_SIZE;

    /* prepare sockaddr of minion */
    sockaddr_in minionAddr;
    minionAddr.sin_family = AF_INET;
    //minionAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    minionAddr.sin_addr.s_addr = inet_addr("192.168.1.20");
    minionAddr.sin_port = htons(MINION_PORT);

    socklen_t size = sizeof(minionAddr);
    /**/
    std::cout << sendto(m_socket.GetFd(), &reqPack, sizeof(reqPack), 0,
                      (struct sockaddr *)&minionAddr, size);

    sleep(2);
    struct ReplyPacket repPack;
    memset(&repPack, 0, sizeof(repPack));

    HandleErrorIfExists(recvfrom(m_socket.GetFd(), &repPack, sizeof(repPack), 0, 
             (struct sockaddr *)&minionAddr, &size), "Failed recvfrom");

    std::cout << "repPack.status = " << repPack.status << std::endl;
    std::cout << "repPack.type = " << repPack.type << std::endl;
    std::cout << "repPack.uid = " << repPack.uid << std::endl;
    std::cout << "repPack.data = " << repPack.data << std::endl;
}

void Master::onWrite(const RequestPacketWrite& pk)
{
    std::cout << "onWrite function\n";

    struct RequestPacket reqPack;
    reqPack.type = 1;
    reqPack.uid = htobe64(pk.uid);
    reqPack.blockIndex = htobe64(pk.offset) / BLOCK_SIZE;
    std::copy(pk.data.begin(), pk.data.end(), reqPack.data);

    /* prepare sockaddr of minion */
    sockaddr_in minionAddr;
    minionAddr.sin_family = AF_INET;
    //minionAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    minionAddr.sin_addr.s_addr = inet_addr("192.168.1.20");
    minionAddr.sin_port = htons(MINION_PORT);

    socklen_t size = sizeof(minionAddr);
    /**/

    std::cout << sendto(m_socket.GetFd(), &reqPack, sizeof(reqPack), 0,
                      (struct sockaddr *)&minionAddr, size);
    std::cout << "After sendto\n";                  

    sleep(2);
    struct ReplyPacket repPack;
    memset(&repPack, 0, sizeof(repPack));
    std::cout << "before recvfrom\n";
    HandleErrorIfExists(recvfrom(m_socket.GetFd(), &repPack, sizeof(repPack), 0, 
             (struct sockaddr *)&minionAddr, &size), "Failed recvfrom");
    std::cout << "After recvfrom\n";
    std::cout << "repPack.status = " << repPack.status << std::endl;
    std::cout << "repPack.type = " << repPack.type << std::endl;
    std::cout << "repPack.uid = " << repPack.uid << std::endl;
    std::cout << "repPack.data = " << repPack.data << std::endl;                  

}
} // end of namespace ilrd