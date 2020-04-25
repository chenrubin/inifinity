
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>         
#include <iostream>             
#include <functional>           /* placeholders */

#include "logger.hpp"
	

#include "reactor.hpp"
#include "driver_proxy.hpp"

#define BLOCK_SIZE 4096

using namespace ilrd;

Reactor rctr;

class Master
{
public:
    explicit Master(Reactor *reactor_);
    ~Master();

    void onRead(const RequestPacketRead& packet_);
    void onWrite(const RequestPacketWrite& packet_);

private:
    Reactor *m_reactor;
    DriverProxy m_proxy;
    boost::thread m_reactorRun;
    char m_buffer[BLOCK_SIZE * 128];
};

Master::Master(Reactor *reactor_)
    : m_reactor(reactor_)
    , m_proxy(m_reactor, boost::bind(&Master::onRead, this, _1), boost::bind(&Master::onWrite, this, _1))
    , m_reactorRun(boost::bind(&Reactor::Run, m_reactor))
{
    memset(m_buffer, 0, BLOCK_SIZE * 128);
    strcpy(m_buffer, "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

Master::~Master()
{
    m_reactorRun.join();
}

void TestCtorDtor();

int main()
{
    TestCtorDtor();
}

void TestCtorDtor()
{
 //   int status = setenv("LOGGER_FILE", "/home/chenr/git/projects/driver_proxy/logger.txt", 1);
    //std::cout << "status !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!= " << status << "\n";
    
    Master master(&rctr);
//    LOG_DEBUG("Inside OnRequestIMP");
}

void Master::onRead(const RequestPacketRead& pk)
{
    std::cout << "onRead function\n";

    struct ReplyPacketRead packet;

    packet.uid = pk.uid;
    packet.len = pk.len;
    packet.status = 0;
    std::cout << "buffer size = " << sizeof(m_buffer) << "\n";
    std::cout << "offset = " << pk.offset << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    std::cout << "len = " << pk.len << "\n";
    std::copy(m_buffer + pk.offset, m_buffer + pk.offset + pk.len, back_inserter(packet.data));
    std::vector<char>::iterator iter;
    std::cout << "vector size = " << packet.data.size() << "\n";
//    std::cout << "buffer = " << m_buffer << "\n";

    m_proxy.ReplyRead(packet);
    std::cout << "end of onRead after m_proxy.ReplyRead(packet);\n";
}

void Master::onWrite(const RequestPacketWrite& pk)
{
    std::cout << "onWrite function\n";

    struct ReplyPacketWrite packet;

    packet.uid = pk.uid;
    packet.len = pk.len;
    packet.status = 0;

    m_proxy.ReplyWrite(packet);
    std::cout << "end of onWrite function\n";
}