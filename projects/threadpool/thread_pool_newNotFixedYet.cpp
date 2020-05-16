// Thread Pool - cpp file
// Last update: 9/4/2020
// Author: chen
// Reviewer: Tsisana
/*----------------------------------------------------------------------------*/
#include <boost/bind.hpp>           /* boost::bind */
#include <boost/shared_ptr.hpp>     /* boost::shared_ptr */
#include <cstdio>                   /* perror*/

#include "thread_pool.hpp"

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
    , m_numOfThreads(0)
    , m_containerMutex()
    , m_cond()
{
    IncreaseThreadNumIMP(numOfThreads_);
}

ThreadPool::~ThreadPool()
{    
   std::cout << "inside dtor\n";
    Stop(stopTimeout);
/*
   std::map<boost::thread::id, boost::shared_ptr<boost::thread> >::iterator iter;
    boost::unique_lock<boost::mutex> lock(m_containerMutex);
    for (iter = m_idThreadContainer.begin(); 
         iter != m_idThreadContainer.end(); 
         ++iter)
    {
        iter->second->join();   
    }*/
}

void ThreadPool::AddTask(boost::function<void(void)> func, Priority_t priority_)
{
    AddTaskIMP(func, static_cast<Extended_prio_t>(priority_));
}

void ThreadPool::SetThreadsNum(size_t numOfThreads_)
{
    boost::unique_lock<boost::mutex> lock(m_threadsMutex);

    if (numOfThreads_ >= m_numOfThreads)
    {
        IncreaseThreadNumIMP(numOfThreads_ - m_numOfThreads);
    }
    else
    {
        DecreaseThreadNumIMP(m_numOfThreads - numOfThreads_);
    }
}

size_t ThreadPool::GetThreadsNum() const NOEXCEPT
{
    return m_numOfThreads.load();
}

void ThreadPool::Stop(boost::chrono::milliseconds endTime_)
{
    m_IsRunning.store(false);
    boost::chrono::system_clock::time_point current = 
                                 boost::chrono::system_clock::now();

    boost::chrono::system_clock::time_point stop_time = 
            boost::chrono::time_point_cast<boost::chrono::milliseconds>(current + endTime_);

    while (boost::chrono::system_clock::now() < stop_time)
    {
        boost::unique_lock<boost::mutex> lock(m_containerMutex);
        if (m_idThreadContainer.empty())
        {
            std::cout << "><><><><><><><><><>num = 0><><><><><><><><><>\n";
            break;
        }
    }

    std::map<boost::thread::id, boost::shared_ptr<boost::thread> >::iterator iter;

    boost::unique_lock<boost::mutex> lock(m_containerMutex);
    std::cout << "size = " << m_idThreadContainer.size() << std::endl;
    for (iter = m_idThreadContainer.begin(); 
         iter != m_idThreadContainer.end(); 
         ++iter)
    {
        std::cout << "pcancel\n";
        pthread_cancel(iter->second->native_handle());
        std::cout << "before join\n";
        iter->second->detach();
        std::cout << "after join\n";
    }
    
    m_idThreadContainer.clear();
}

bool ThreadPool::CompareFunc::operator()(const Task &task1_, const Task &task2_)
{
    return (task1_.GetPriority() > task2_.GetPriority()) ? false : true;
}

void ThreadPool::DecreaseThreadNumIMP(size_t num)
{
    for (size_t i = 0; i < num; ++i)
    {
        AddTaskIMP(boost::bind(&ThreadPool::BadAppleIMP, this),  BAD_APPLE);
        --m_numOfThreads;
    }
}

void ThreadPool::IncreaseThreadNumIMP(size_t num)
{
    for (size_t i = 0; i < num; ++i)
    {
        boost::shared_ptr<boost::thread> thread_shared_ptr
            (new boost::thread(boost::bind(&ThreadPool::ThreadRoutineIMP, this)));

        boost::unique_lock<boost::mutex> lock(m_containerMutex);
        m_idThreadContainer[thread_shared_ptr->get_id()] = thread_shared_ptr;
        ++m_numOfThreads;                    
    }
}

void ThreadPool::AddTaskIMP(boost::function<void(void)> func, 
                            Extended_prio_t priority_)
{
    m_queue.Push(Task(func, priority_));
}

void ThreadPool::Task::Wrapper(bool *isTerminated)
{
    if (m_priority > NUM_OF_PRIORITIES)
    {
        *isTerminated = true;
    }

    m_func();
}

void ThreadPool::ThreadRoutineIMP()
{
    bool isTerminated = false;
    boost::function<void(void)> dummy_func;

    if (0 != pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))
    {
        std::perror("pthread_setcanceltype");
    }
    while (m_IsRunning.load() && (false == isTerminated))
    {
        Task task(dummy_func, static_cast<Extended_prio_t>(LOW));
        if (m_queue.Pop(task, popTimeout))
        {
            task.InvokeFunction(&isTerminated);
        }
    }
    boost::unique_lock<boost::mutex> lock(m_containerMutex);
    m_idThreadContainer[boost::this_thread::get_id()]->detach(); 
    /*if (!isTerminated)
    {
        m_idThreadContainer.erase(boost::this_thread::get_id());
    }*/
    m_idThreadContainer.erase(boost::this_thread::get_id());
    
}

void ThreadPool::BadAppleIMP()
{
    std::cout << "BadApple" << std::endl;

    boost::unique_lock<boost::mutex> lock(m_containerMutex);
    m_idThreadContainer[boost::this_thread::get_id()]->detach(); 
}

void ThreadPool::UpdateNumOfThreadsIMP(size_t num)
{
    m_numOfThreads.store(num);
}
/*-----------------------------------------------------------------------------
Task functions
------------------------------------------------------------------------------*/
ThreadPool::Task::Task(boost::function<void(void)> func_, 
                       Extended_prio_t priority_)
    : m_func(func_)
    , m_priority(static_cast<int> (priority_))
{}

void ThreadPool::Task::InvokeFunction(bool *isTerminated)
{
    Wrapper(isTerminated);
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