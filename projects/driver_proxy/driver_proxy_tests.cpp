
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <iostream>

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
    explicit Master(Reactor *reactor_/*, DriverProxy *proxy_*/);
    ~Master();

    static void onRead(const RequestPacketRead& packet_);
    static void onWrite(const RequestPacketRead& packet_);

private:
    Reactor *m_reactor;
    static DriverProxy *m_proxy;
    static char m_buffer[BLOCK_SIZE];
};

Master::Master(Reactor *reactor_/*, DriverProxy *proxy_*/)
    : m_reactor(reactor_)
 //   , m_proxy(proxy_)
//    , m_buffer("123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ")
{
    m_proxy(m_reactor, onRead, onWrite);
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
    DriverProxy dp(&rctr, Master::onRead, Master::onWrite);
    Master master(&rctr, &dp);
}

void Callback(int fd)
{
    std::cout << "callback\n";
}

void Master::onRead(const RequestPacketRead& pk)
{
    std::cout << "onRead function\n";
    assert(pk.offset < BLOCK_SIZE);
    assert(pk.len < BLOCK_SIZE - pk.offset);

    struct ReplyPacketRead packet;

    packet.uid = pk.uid;
    packet.len = pk.len;
    packet.status = 0;

    std::copy(packet.data.begin(), packet.data.end(), m_buffer + pk.offset); 

    m_proxy->ReplyRead(packet);
}

void Master::onWrite(const RequestPacketWrite& pk)
{
    std::cout << "onWrite function\n";
}