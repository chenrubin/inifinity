#include "master.hpp"
#include "../driver_proxy/proxy_packet.hpp"
#include "../../cpp_fs/ws/reactor/reactor.hpp"

using namespace ilrd;

void TestCommunicationWriteAndRead();

int main()
{
    TestCommunicationWriteAndRead();

    return 0;
}

void TestCommunicationWriteAndRead()
{
    Reactor Reactor;
    Master master(&Reactor);

    RequestPacketWrite reqPackWrite;
    char myData[] = "123456789abcdefghijklmnopqrstuvwxyz";
    reqPackWrite.len = 40;
    reqPackWrite.offset = 0;
    reqPackWrite.uid = 666;
    std::copy(myData, myData + reqPackWrite.len, std::back_inserter(reqPackWrite.data));

    master.onWrite(reqPackWrite);

    RequestPacketRead reqPackRead; // proxy to master
    reqPackRead.len = 20;
    reqPackRead.offset = 0;
    reqPackRead.uid = 123;

    master.onRead(reqPackRead);
}