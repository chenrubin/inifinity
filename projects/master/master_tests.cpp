#include "master.hpp"
#include "proxy_packet.hpp"
#include "reactor.hpp"

using namespace ilrd;

void TestCommunicationWriteAndRead();
void TestMasterWithProxy();

int main()
{
 //   TestCommunicationWriteAndRead();
    TestMasterWithProxy();

    return 0;
}
/*
void TestCommunicationWriteAndRead()
{
    // This test takes under consideration that there are two minions 
    // each of which consists of 32KB (32*4096)  
    std::vector<std::pair<unsigned short, std::string> > myPairs;
    std::pair<unsigned short, std::string> pair1 = std::make_pair(12344, "127.0.0.1");
    std::pair<unsigned short, std::string> pair2 = std::make_pair(12345, "127.0.0.1"/*"192.168.1.20"*//*);

    myPairs.push_back(pair1);
    myPairs.push_back(pair2);

    Reactor Reactor;
    Master master(&Reactor, myPairs);

    RequestPacketWrite reqPackWrite;
    char myData[] = "123456789abcdefghijklmnopqrstuvwxyz";
    reqPackWrite.len = 1;
    reqPackWrite.offset = 131071;
    reqPackWrite.uid = 666;
    std::copy(myData, myData + reqPackWrite.len, std::back_inserter(reqPackWrite.data));

    std::cout << "\n\n!!!Write to first minion!!!\n\n";
    master.onWrite(reqPackWrite);

 //   sleep(5);

    char myData1[] = "ABCDEFGHIJKLMNOP";
    reqPackWrite.len = 1;
    reqPackWrite.offset = 131072;
    reqPackWrite.uid = 667;
    std::copy(myData1, myData1 + reqPackWrite.len, std::back_inserter(reqPackWrite.data));

    std::cout << "\n\n!!!Write to second minion!!!\n\n";
    master.onWrite(reqPackWrite);

    RequestPacketRead reqPackRead; // proxy to master
    reqPackRead.len = 1;
    reqPackRead.offset = 131071;
    reqPackRead.uid = 123;

    master.onRead(reqPackRead);
   
}
*/
void TestMasterWithProxy()
{
    std::vector<std::pair<unsigned short, std::string> > myPairs;
    std::pair<unsigned short, std::string> pair1 = std::make_pair(12344, "127.0.0.1");
    std::pair<unsigned short, std::string> pair2 = std::make_pair(12345, "127.0.0.1"/*"192.168.1.20"*/);

    myPairs.push_back(pair1);
    myPairs.push_back(pair2);

    Reactor reactor;
    Timer timer(&reactor);
    Master master(&reactor, myPairs, &timer);
}