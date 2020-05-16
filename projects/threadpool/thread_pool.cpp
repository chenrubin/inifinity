// Thread Pool - cpp file
// Last update: 9/4/2020
// Author: chen
// Reviewer: Tsisana
/*----------------------------------------------------------------------------*/
#include <boost/bind.hpp>           /* boost::bind */
#include <boost/shared_ptr.hpp>     /* boost::shared_ptr */
#include <cstdio>                   /* perror*/

#include "thread_pool.hpp"
//#include "thread_pool_old.hpp"

namespace ilrd
{
/*-----------------------------------------------------------------------------
ThreadPool functions
------------------------------------------------------------------------------*/
ThreadPool::ThreadPool(size_t numOfThreads_)
    : popTimeout(10)
    , stopTimeout(60)
    , m_queue()
    , m_IsRunning(true)
    , m_idThreadContainer()
    , m_containerMutex()
    , m_cond()
{
    IncreaseThreadNumIMP(numOfThreads_);
}

ThreadPool::~ThreadPool()
{    
    Stop(stopTimeout);
}

void ThreadPool::AddTask(boost::function<void(void)> func, Priority_t priority_)
{
    AddTaskIMP(func, static_cast<Extended_prio_t>(priority_));
}

void ThreadPool::SetThreadsNum(size_t numOfThreads_)
{
    boost::unique_lock<boost::mutex> lock(m_threadsMutex);
    size_t size = GetThreadsNum();

    if (numOfThreads_ >= size)
    {
        IncreaseThreadNumIMP(numOfThreads_ - size);
    }
    else
    {
        DecreaseThreadNumIMP(size - numOfThreads_);
    }
}

size_t ThreadPool::GetThreadsNum() const NOEXCEPT
{
    boost::unique_lock<boost::mutex> lock(m_containerMutex);

    return m_idThreadContainer.size();
}

void ThreadPool::Stop(boost::chrono::milliseconds endTime_)
{
    m_IsRunning.store(false);
    boost::this_thread::sleep_for(endTime_);

    std::map<boost::thread::id, boost::shared_ptr<boost::thread> >::iterator iter;

    boost::unique_lock<boost::mutex> lock(m_containerMutex);
    for (iter = m_idThreadContainer.begin(); 
         iter != m_idThreadContainer.end(); 
         ++iter)
    {
        pthread_cancel(iter->second->native_handle());
    }
}

bool ThreadPool::CompareFunc::operator()(const Task &task1_, const Task &task2_)
{
    return (task1_.GetPriority() > task2_.GetPriority()) ? false : true;
}

void ThreadPool::DecreaseThreadNumIMP(size_t num)
{
    size_t size = GetThreadsNum();
    
    for (size_t i = 0; i < num; ++i)
    {
        AddTaskIMP(boost::bind(&ThreadPool::BadAppleIMP, this),  BAD_APPLE);
    }

    boost::unique_lock<boost::mutex> lock(m_containerMutex);
    m_cond.wait(lock, boost::bind(&ThreadPool::IsSizeMatchIMP, this, size - num));
}

bool ThreadPool::IsSizeMatchIMP(size_t size)
{
    return (m_idThreadContainer.size() == size);
}

void ThreadPool::IncreaseThreadNumIMP(size_t num)
{
    for (size_t i = 0; i < num; ++i)
    {
        boost::shared_ptr<boost::thread> thread_shared_ptr
            (new boost::thread(boost::bind(&ThreadPool::ThreadRoutineIMP, this)));

        boost::unique_lock<boost::mutex> lock(m_containerMutex);
        m_idThreadContainer[thread_shared_ptr->get_id()] = thread_shared_ptr;
    }
}

void ThreadPool::AddTaskIMP(boost::function<void(void)> func, 
                            Extended_prio_t priority_)
{
    m_queue.Push(Task(func, priority_));
}

void ThreadPool::ThreadRoutineIMP()
{
    boost::function<void(void)> dummy_func;

    if (0 != pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))
    {
        std::perror("pthread_setcanceltype");
    }
    while (m_IsRunning.load())
    {
        Task task(dummy_func, static_cast<Extended_prio_t>(LOW));
        if (m_queue.Pop(task, popTimeout))
        {
            task.InvokeFunction();
        }
    }
}

void ThreadPool::BadAppleIMP()
{
    boost::unique_lock<boost::mutex> lock(m_containerMutex);
    boost::shared_ptr<boost::thread> temp_ptr = 
            m_idThreadContainer[boost::this_thread::get_id()];

    m_idThreadContainer.erase(boost::this_thread::get_id());
    m_cond.notify_one();
    temp_ptr->interrupt();
    
    boost::this_thread::interruption_point();
}
/*-----------------------------------------------------------------------------
Task functions
------------------------------------------------------------------------------*/
ThreadPool::Task::Task(boost::function<void(void)> func_, 
                       Extended_prio_t priority_)
    : m_func(func_)
    , m_priority(static_cast<int> (priority_))
{}

void ThreadPool::Task::InvokeFunction()
{
    m_func();
}

int ThreadPool::Task::GetPriority() const
{
    return m_priority;
}
/*-----------------------------------------------------------------------------
MyQueue - Wrapper for waitable queue
------------------------------------------------------------------------------*/
ThreadPool::Task ThreadPool::MyQueue::front()
{
    return top();
}
} // end of namespace ilrd