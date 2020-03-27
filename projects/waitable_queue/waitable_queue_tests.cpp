#include <queue>    // queue, priority_queue

#include "waitable_queue.hpp"

using namespace ilrd;
/*
class MyQueue
{
public:
    MyQueue();
    ~MyQueue();

private:
    std::queue<int> myqueue;
};
*/
int main()
{
    WaitableQueue<std::queue<int> > myWaitQueue;
    
    return 0;
}
