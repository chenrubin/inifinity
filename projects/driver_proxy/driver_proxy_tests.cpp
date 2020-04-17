
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <iostream>
#include <functional>           /* placeholders */
	

#include "reactor.hpp"
#include "driver_proxy.hpp"

#define BLOCK_SIZE 4096

using namespace ilrd;
//void onRead(const RequestPacketRead& pk);
//void onWrite(const RequestPacketWrite& pk);
void Callback(int fd);

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
    char m_buffer[BLOCK_SIZE * 128];
};

Master::Master(Reactor *reactor_)
    : m_reactor(reactor_)
    , m_proxy(m_reactor, boost::bind(&Master::onRead, this, _1), boost::bind(&Master::onWrite, this, _1))
{
   strcpy(m_buffer, "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

Master::~Master()
{}

void TestCtorDtor();

int main()
{
    TestCtorDtor();
}

void TestCtorDtor()
{
//    rctr.AddFd(60, rctr.WRITE, Callback);
 //   DriverProxy dp(&rctr, Master::onRead, Master::onWrite);
    Master master(&rctr);
}

void Callback(int fd)
{
    std::cout << "callback\n";
}

void Master::onRead(const RequestPacketRead& pk)
{
    std::cout << "onRead function\n";

    struct ReplyPacketRead packet;

    packet.uid = pk.uid;
    packet.len = pk.len;
    packet.status = 0;
    std::cout << "offset = " << pk.offset << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    std::cout << "len = " << pk.len << "\n";
    std::copy(m_buffer + pk.offset, m_buffer + pk.offset + pk.len, back_inserter(packet.data));
    std::vector<char>::iterator iter;
    std::cout << "vector size = " << packet.data.size() << "\n";
    std::cout << "buffer = " << m_buffer << "\n";

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