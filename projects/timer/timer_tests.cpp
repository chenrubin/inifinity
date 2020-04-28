#include <boost/thread.hpp>         // boost::thread

#include "reactor.hpp"
#include "timer.hpp"

#define STDIN (0)

using namespace ilrd;

void TestCtorDtor();
void TestScheduleActionRegular();
void TestScheduleActionOpposite();

void Action1IMP();
void Action2IMP();
void Action3IMP();
void Action4IMP();
void Action5IMP();
void Action6IMP();
void Action7IMP();
void Action8IMP();
void StopReactorIMP(int fd_);

Reactor reactor;

typedef boost::chrono::milliseconds milli_t;

int main()
{   //  run either tests 2 or 3
 /*1*/   TestCtorDtor();
 /*2*/ //  TestScheduleActionRegular(); 
 /*3*/   TestScheduleActionOpposite();

    return 0;
}

void TestCtorDtor()
{
    std::cout << "\n!!!!!!!TestCtorDtor!!!!!!!!!!!!!!!!!!\n";
    reactor.AddFd(STDIN, reactor.READ, StopReactorIMP);

    Timer timer(&reactor);
}

void TestScheduleActionRegular()
{
    std::cout << "\n!!!!!!!TestScheduleActionRegular!!!!!!!!!!!!!!!!!!\n";
   // reactor.AddFd(STDIN, reactor.READ, StopReactorIMP);

    Timer timer(&reactor);

    timer.ScheduleAction(milli_t(300), Action1IMP);
    timer.ScheduleAction(milli_t(1500), Action2IMP);
    timer.ScheduleAction(milli_t(3000), Action3IMP);
    timer.ScheduleAction(milli_t(4000), Action4IMP);
    timer.ScheduleAction(milli_t(5000), Action5IMP);
    timer.ScheduleAction(milli_t(6000), Action6IMP);
    timer.ScheduleAction(milli_t(7000), Action7IMP);
    timer.ScheduleAction(milli_t(8000), Action8IMP);

    reactor.Run();
    std::cout << "End f jner\n";
}

void TestScheduleActionOpposite()
{
    std::cout << "\n!!!!!!!TestScheduleActionRegular!!!!!!!!!!!!!!!!!!\n";
 //   reactor.AddFd(STDIN, reactor.READ, StopReactorIMP);

    Timer timer(&reactor);

    timer.ScheduleAction(milli_t(8000), Action8IMP);
    timer.ScheduleAction(milli_t(7000), Action7IMP);
    timer.ScheduleAction(milli_t(6000), Action6IMP);
    timer.ScheduleAction(milli_t(5000), Action5IMP);
    timer.ScheduleAction(milli_t(4000), Action4IMP);
    timer.ScheduleAction(milli_t(3000), Action3IMP);
    timer.ScheduleAction(milli_t(1500), Action2IMP);
    timer.ScheduleAction(milli_t(300), Action1IMP);

    reactor.Run();
    std::cout << "fuck\n";
}

void Action1IMP()
{
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!Action 1" << std::endl;
}

void Action2IMP()
{
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!1Action 2" << std::endl;
}

void Action3IMP()
{
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Action 3" << std::endl;
}

void Action4IMP()
{
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!1Action 4" << std::endl;
}

void Action5IMP()
{
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1Action 5" << std::endl;
}

void Action6IMP()
{
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Action 6" << std::endl;
}

void Action7IMP()
{
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1Action 7" << std::endl;
}

void Action8IMP()
{
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111Action 8" << std::endl;
}

void StopReactorIMP(int fd_)
{
    char buff[20];
    fgets(buff, sizeof(buff), stdin);
    if (0 == strcmp(buff, "stop\n"))
    {
        reactor.Stop();
    }
}