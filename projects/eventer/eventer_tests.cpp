#include <boost/thread.hpp>                         // boost::thread
#include <iostream>                                 // std::cout
#include <boost/bind.hpp>                           // boost::bind

#include "eventer.hpp"                              // class eventer
#include "../../cpp_fs/ws/reactor/reactor.hpp"      // class reactor

#define EVENT_ID1 (1)
#define EVENT_ID2 (2)
#define EVENT_ID3 (3)
#define EVENT_ID4 (4)
#define EVENT_ID5 (5)

using namespace ilrd;

void TestBasic();
void Callback1IMP();
void Callback2IMP();
void Callback3IMP();

class DummyMaster
{
public:
    DummyMaster(Eventer *eventer_);
    ~DummyMaster();

    void YaanuIncryptionIMP(int eventID);

private:
    Eventer *m_eventer;
};


DummyMaster::DummyMaster(Eventer *eventer_)
    : m_eventer(eventer_)
{}

DummyMaster::~DummyMaster()
{}

void DummyMaster::YaanuIncryptionIMP(int eventID)
{
    std::cout << "waiting and then print hello!!!\n";
    int numOfSeconds = eventID;
    sleep(5 - numOfSeconds);
    std::cout << "!!! hello !!!\n";
    
    m_eventer->SignalEvent(eventID);
}

int main()
{
    TestBasic();

    return 0;
}

void TestBasic()
{
    Reactor reactor;
    boost::thread reactorThread(boost::bind(&Reactor::Run, &reactor));
    
    Eventer eventer(&reactor);
    DummyMaster dMaster(&eventer);

    eventer.SetEvent(EVENT_ID1, boost::bind(Callback1IMP));
    eventer.SetEvent(EVENT_ID2, boost::bind(Callback2IMP));
    eventer.SetEvent(EVENT_ID3, boost::bind(Callback3IMP));

    boost::thread poolThread1(boost::bind(&DummyMaster::YaanuIncryptionIMP, &dMaster, 1));
    boost::thread poolThread2(boost::bind(&DummyMaster::YaanuIncryptionIMP, &dMaster, 2));
    boost::thread poolThread3(boost::bind(&DummyMaster::YaanuIncryptionIMP, &dMaster, 3));

    sleep(10);

    poolThread1.join();
    poolThread2.join();
    poolThread3.join();
}

void Callback1IMP()
{
    std::cout << "This is Callback1 of eventID1 - Like sending the incrypted packet\n";
}

void Callback2IMP()
{
    std::cout << "This is Callback2 of eventID2 - Like sending the incrypted packet\n";
}

void Callback3IMP()
{
    std::cout << "This is Callback3 of eventID3 - Like sending the incrypted packet\n";
}