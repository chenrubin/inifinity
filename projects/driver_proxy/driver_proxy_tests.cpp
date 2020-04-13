
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <iostream>

#include "reactor.hpp"
#include "driver_proxy.hpp"

using namespace ilrd;
/*const*/ void onRead(const RequestPacketRead& pk);
/*const*/ void onWrite(const RequestPacketWrite& pk);
void Callback(int fd);

void TestCtorDtor();

int main()
{
    TestCtorDtor();
}

void TestCtorDtor()
{
    Reactor rctr;
//    rctr.AddFd(60, rctr.WRITE, Callback);
    DriverProxy dp(&rctr, onRead, onWrite);
}

void Callback(int fd)
{
    std::cout << "callback\n";
}

/*const*/ void onRead(const RequestPacketRead& pk)
{
    std::cout << "onRead function\n";
}

/*const*/ void onWrite(const RequestPacketWrite& pk)
{
    std::cout << "onWrite function\n";
}