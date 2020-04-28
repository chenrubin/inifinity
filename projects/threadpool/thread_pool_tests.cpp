#include <iostream>         /* cout */

//#include "thread_pool.hpp"
#include "thread_pool_old.hpp"
#include "MyUtils.hpp"      /* test result */

using namespace ilrd;

void TestAddTaskAndStop();
void TestDecreaseThreads();
void TestIncreaseAndDecreaseThreads();
void TestStop();
void TestCtor();
void TestEndlesslyThread();
void TestCrazy();

bool PrintHelloNumberHigh(int num);
bool PrintHelloNumberMedium(int num);
bool PrintHelloNumberLow(int num);
bool PrintHelloNumberHighStop(int num);
bool PrintHelloNumberMediumStop(int num);
bool PrintHelloNumberLowStop(int num);

void PrintEndlessly();

boost::mutex mutex;

int main()
{
    TestAddTaskAndStop();
    TestDecreaseThreads();
    TestIncreaseAndDecreaseThreads();
   TestCrazy();
    TestStop();
    TestCtor();
    TestEndlesslyThread();

    return 0;
}

void TestAddTaskAndStop()
{
    ThreadPool tp(1);
    boost::chrono::milliseconds t(10000);
    std::cout << "num = " << tp.GetThreadsNum() << std::endl;
    PRINTTESTRESULTS("TestAddTaskAndStop",1, 1 == tp.GetThreadsNum());

    for (size_t i = 0; i < 2; ++i)
    {
        tp.AddTask(boost::bind(PrintHelloNumberLow, 10), ThreadPool::LOW);
        tp.AddTask(boost::bind(PrintHelloNumberMedium, 20), ThreadPool::MEDIUM);
        tp.AddTask(boost::bind(PrintHelloNumberHigh, 30), ThreadPool::HIGH);
    }

    tp.SetThreadsNum(2);
    PRINTTESTRESULTS("TestAddTaskAndStop",2, 2 == tp.GetThreadsNum());

    for (size_t i = 0; i < 10; ++i)
    {
        tp.AddTask(boost::bind(PrintHelloNumberLow, 10), ThreadPool::LOW);
        tp.AddTask(boost::bind(PrintHelloNumberMedium, 20), ThreadPool::MEDIUM);
        tp.AddTask(boost::bind(PrintHelloNumberHigh, 30), ThreadPool::HIGH);
    }

    tp.Stop(t);
}

void TestDecreaseThreads()
{
    ThreadPool tp(5);
    boost::chrono::milliseconds t(5000);
    PRINTTESTRESULTS("TestDecreaseThreads",1, 5 == tp.GetThreadsNum());

    for (size_t i = 0; i < 20; ++i)
    {
        tp.AddTask(boost::bind(PrintHelloNumberLow, 10), ThreadPool::LOW);
        tp.AddTask(boost::bind(PrintHelloNumberMedium, 20), ThreadPool::MEDIUM);
        tp.AddTask(boost::bind(PrintHelloNumberHigh, 30), ThreadPool::HIGH);
    }

    std::cout << "Decrease number of threads\n";
    tp.SetThreadsNum(2);

    std::cout << "NumOFThreads = " << tp.GetThreadsNum() << std::endl;
    PRINTTESTRESULTS("TestDecreaseThreads",2, 2 == tp.GetThreadsNum());

    for (size_t i = 0; i < 3; ++i)
    {
        tp.AddTask(boost::bind(PrintHelloNumberLow, 10), ThreadPool::LOW);
        tp.AddTask(boost::bind(PrintHelloNumberMedium, 20), ThreadPool::MEDIUM);
        tp.AddTask(boost::bind(PrintHelloNumberHigh, 30), ThreadPool::HIGH);
    }

    tp.Stop(t);
}

void TestIncreaseAndDecreaseThreads()
{
    ThreadPool tp(100);
    boost::chrono::milliseconds t(5000);
    PRINTTESTRESULTS("TestIncreaseAndDecreaseThreads",1, 100 == tp.GetThreadsNum());

    for (size_t i = 0; i < 100; ++i)
    {
        tp.AddTask(boost::bind(PrintHelloNumberLow, 10), ThreadPool::LOW);
        tp.AddTask(boost::bind(PrintHelloNumberMedium, 20), ThreadPool::MEDIUM);
        tp.AddTask(boost::bind(PrintHelloNumberHigh, 30), ThreadPool::HIGH);
    }

    std::cout << "Decrease number of threads\n";
    tp.SetThreadsNum(53);

    std::cout << "NumOFThreads = " << tp.GetThreadsNum() << std::endl;
    PRINTTESTRESULTS("TestIncreaseAndDecreaseThreads",2, 53 <= tp.GetThreadsNum());

    for (size_t i = 0; i < 100; ++i)
    {
        tp.AddTask(boost::bind(PrintHelloNumberLow, 10), ThreadPool::LOW);
        tp.AddTask(boost::bind(PrintHelloNumberMedium, 20), ThreadPool::MEDIUM);
        tp.AddTask(boost::bind(PrintHelloNumberHigh, 30), ThreadPool::HIGH);
    }

   // sleep(20);

    tp.Stop(t);
}

void TestStop()
{
    ThreadPool tp(3);
    boost::chrono::milliseconds t(1000);
    PRINTTESTRESULTS("TestStop",1, 3 == tp.GetThreadsNum());

    for (size_t i = 0; i < /*200*/1; ++i)
    {
        tp.AddTask(boost::bind(PrintHelloNumberLowStop, 10), ThreadPool::LOW);
        tp.AddTask(boost::bind(PrintHelloNumberMediumStop, 20), ThreadPool::MEDIUM);
        tp.AddTask(boost::bind(PrintHelloNumberHighStop, 30), ThreadPool::HIGH);
    }

    tp.Stop(t);
}

void TestCtor()
{
    ThreadPool tp(3);
}

void TestEndlesslyThread()
{
    ThreadPool tp(3);
    boost::chrono::milliseconds t(5000);
    PRINTTESTRESULTS("TestEndlesslyThread",1, 3 == tp.GetThreadsNum());

    for (size_t i = 0; i < /*2*/1; ++i)
    {
        tp.AddTask(boost::bind(PrintHelloNumberLow, 10), ThreadPool::LOW);
        tp.AddTask(boost::bind(PrintHelloNumberMedium, 20), ThreadPool::MEDIUM);
        tp.AddTask(PrintEndlessly, ThreadPool::HIGH);
    }

    tp.Stop(t);
}

void TestCrazy()
{
    ThreadPool tp(100);
    boost::chrono::milliseconds t(5000);
    PRINTTESTRESULTS("TestCrazy",1, 100 == tp.GetThreadsNum());


    for (size_t i = 0; i < 20; ++i)
    {
        tp.AddTask(boost::bind(PrintHelloNumberLowStop, 10), ThreadPool::LOW);
        tp.AddTask(boost::bind(PrintHelloNumberMediumStop, 20), ThreadPool::MEDIUM);
        tp.AddTask(boost::bind(PrintHelloNumberHighStop, 30), ThreadPool::HIGH);
    }

    boost::shared_ptr<boost::thread> thread_shared_ptr1
    (new boost::thread(boost::bind(&ThreadPool::SetThreadsNum, &tp, 10)));
    
    boost::shared_ptr<boost::thread> thread_shared_ptr2
    (new boost::thread(boost::bind(&ThreadPool::GetThreadsNum, &tp)));

    std::cout << "before stop\n";
    tp.Stop(t);

    thread_shared_ptr1->join();
    thread_shared_ptr2->join();
}

bool PrintHelloNumberHigh(int num)
{
 //   std::cout << "Hello number " << num << " high" << std::endl;

    return 0;
}

bool PrintHelloNumberMedium(int num)
{
   // std::cout << "Hello number " << num << " medium" << std::endl;

    return 0;
}

bool PrintHelloNumberLow(int num)
{
 //   std::cout << "Hello number " << num << " low" << std::endl;

    return 0;
}

bool PrintHelloNumberHighStop(int num)
{
//    mutex.lock();
 //   std::cout << "Hello number " << num << " high stop" << std::endl;

    return 0;
}

bool PrintHelloNumberMediumStop(int num)
{
 //   mutex.lock();
 //   std::cout << "Hello number " << num << " medium stop" << std::endl;

    return 0;
}

bool PrintHelloNumberLowStop(int num)
{
//   mutex.lock();
//    std::cout << "Hello number " << num << " low stop" << std::endl;

    return 0;
}

void PrintEndlessly()
{
    while (true)
    {
        //std::cout << "continue print\n";
    }
}