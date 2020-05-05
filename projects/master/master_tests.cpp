#include "master.hpp"
#include "../driver_proxy/proxy_packet.hpp"
#include "../../cpp_fs/ws/reactor/reactor.hpp"

using namespace ilrd;

//void TestCommunicationWriteAndRead();
void TestMasterWithProxy();

int main()
{
    //TestCommunicationWriteAndRead();
    TestMasterWithProxy();

    return 0;
}
/*
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
*/
void TestMasterWithProxy()
{
    std::vector<std::pair<unsigned short, std::string> > myPairs;
    std::pair<unsigned short, std::string> pair1 = std::make_pair(12345, "127.0.0.1");

    myPairs.push_back(pair1);

    Reactor Reactor;
    Master master(&Reactor, myPairs);
}