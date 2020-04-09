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
    : m_queue()
    , m_IsRunning(true)
    , m_idThreadContainer()
    , m_mutex()
    , m_cond()
{
    IncreaseThreadNumIMP(numOfThreads_);
}

ThreadPool::~ThreadPool()
{    
    boost::chrono::milliseconds mili(60);
    Stop(mili);
}

void ThreadPool::AddTask(boost::function<void(void)> func, Priority_t priority_)
{
    AddTaskIMP(func, static_cast<Extended_prio_t>(priority_));
}

void ThreadPool::SetThreadsNum(size_t numOfThreads_)
{
    boost::unique_lock<boost::mutex> lock(m_mutex_pool);
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

size_t ThreadPool::GetThreadsNum() const
{
    boost::unique_lock<boost::mutex> lock(m_mutex);
    return m_idThreadContainer.size();
}

void ThreadPool::Stop(boost::chrono::milliseconds endTime_)
{
    m_IsRunning.store(false);
    boost::this_thread::sleep_for(endTime_);

    std::map<boost::thread::id, boost::shared_ptr<boost::thread> >::iterator iter;

    boost::unique_lock<boost::mutex> lock2(m_mutex);
    for (iter = m_idThreadContainer.begin(); 
         iter != m_idThreadContainer.end(); 
         ++iter)
    {
        pthread_cancel(iter->second->native_handle());
    }
}

void ThreadPool::EmptyEntireQueueIMP()
{
    Task task(boost::bind(Task::BadApple, this), static_cast<Extended_prio_t>(LOW));
    while (!m_queue.IsEmpty())
    {
        m_queue.Pop(task);
    }
}

void ThreadPool::DecreaseThreadNumIMP(size_t num)
{
    size_t size = GetThreadsNum();
    
    for (size_t i = 0; i < num; ++i)
    {
        AddTaskIMP(boost::bind(Task::BadApple, this), BAD_APPLE);
    }

    boost::unique_lock<boost::mutex> lock(m_mutex);
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

         m_idThreadContainer[thread_shared_ptr->get_id()] = thread_shared_ptr;
    }
}

void ThreadPool::AddTaskIMP(boost::function<void(void)> func, 
                            Extended_prio_t priority_)
{
    Task task(func, priority_);
    m_queue.Push(task);
}

void ThreadPool::ThreadRoutineIMP()
{
    Priority_t prio = LOW;
    boost::function<void(void)> func;
    boost::chrono::milliseconds timeout(10);

    if (0 != pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))
    {
        std::perror("pthread_setcanceltype");
    }
    while (true == m_IsRunning.load())
    {
        Task task(func, static_cast<Extended_prio_t>(prio));
        if (m_queue.Pop(task, timeout))
        {
            task.InvokeFunction();
        }
    }
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

void ThreadPool::Task::BadApple(ThreadPool *this_)
{
     boost::unique_lock<boost::mutex> lock(this_->m_mutex);
     boost::shared_ptr<boost::thread> temp_ptr = 
     this_->m_idThreadContainer[boost::this_thread::get_id()];

     this_->m_idThreadContainer.erase(boost::this_thread::get_id());
     this_->m_cond.notify_one();
     temp_ptr->interrupt();
    
    boost::this_thread::interruption_point();
}
/*-----------------------------------------------------------------------------
Task compare functions
------------------------------------------------------------------------------*/
bool ThreadPool::Task::CompareFunc::operator()(Task task1_, Task task2_)
{
    return (task1_.m_priority > task2_.m_priority) ? false : true;
}
/*-----------------------------------------------------------------------------
MyQueue - Wrapper for waitable queue
------------------------------------------------------------------------------*/
ThreadPool::Task ThreadPool::MyQueue::front()
{
    return this->top();
}
} // end of namespace ilrd