#include <boost/bind.hpp>           /* boost::bind */
#include <boost/shared_ptr.hpp>     /* boost::shared_ptr */
#include <iostream>

#include "thread_pool.hpp"

#define BAD_APPLE_PRIO (10)
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
    boost::chrono::milliseconds mili(50);
    Stop(mili);
    std::cout << "Dtor , size = " << GetThreadsNum() << "\n";
 //   std::cout << "end of Dtor\n";
}

void ThreadPool::AddTask(boost::function<void(void)> func, Priority_t priority_)
{
    Task task(func, priority_);
    m_queue.Push(task);
}

void ThreadPool::SetThreadsNum(size_t numOfThreads_)
{
 //   std::cout << "inside setthread num before lock!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    boost::unique_lock<boost::mutex> lock(m_mutex_pool);
 //   std::cout << "inside setthread num after lock!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";

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
    boost::unique_lock<boost::recursive_mutex> lock(m_mutex);
    return m_idThreadContainer.size();
}

void ThreadPool::Stop(boost::chrono::milliseconds endTime_)
{
 //   std::cout << "Inside stop before lock\n";
   // boost::unique_lock<boost::mutex> lock1(m_mutex_pool);
 //   std::cout << "Inside stop after lock!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n";
    m_IsRunning.store(false,  boost::memory_order_release);
    boost::this_thread::sleep_for(endTime_);

     std::map<boost::thread::id, boost::shared_ptr<boost::thread> >::iterator iter;
    //EmptyEntireQueueIMP();
//    std::cout << "size = " << GetThreadsNum() << "\n";
    boost::unique_lock<boost::recursive_mutex> lock2(m_mutex);
    for (iter = m_idThreadContainer.begin(); 
         iter != m_idThreadContainer.end(); 
         ++iter)
    {
        pthread_cancel(iter->second->native_handle());
    }
}

void ThreadPool::EmptyEntireQueueIMP()
{
    Task task(boost::bind(Task::BadApple, this), LOW);
    while (!m_queue.IsEmpty())
    {
        m_queue.Pop(task);
    }
}

void ThreadPool::DecreaseThreadNumIMP(size_t num)
{
    size_t size = 0;
 //   std::cout << "!!!!!!!!!!!!inside Decrease!!!!!!!!!!!!!!!!!!!!!!11" << std::endl;
    {
        //boost::unique_lock<boost::recursive_mutex> lock(m_mutex);
        size = GetThreadsNum(); //m_idThreadContainer.size(); 
    }
    
    for (size_t i = 0; i < num; ++i)
    {
        std::cout << "push bad apple to queue i = " << i << std::endl;
        AddTask(boost::bind(Task::BadApple, this), NUM_OF_PRIORITIES);
    }

    boost::unique_lock<boost::recursive_mutex> lock(m_mutex);
    m_cond.wait(lock, boost::bind(&ThreadPool::IsSizeMatchIMP, this, size - num));

//    std::cout << "after wait\n";
}

bool ThreadPool::IsSizeMatchIMP(size_t size)
{
//    std::cout << "size = " << m_idThreadContainer.size() << std::endl;
    return (m_idThreadContainer.size() == size);
}

void ThreadPool::IncreaseThreadNumIMP(size_t num)
{
        boost::unique_lock<boost::recursive_mutex> lock(m_mutex);
    for (size_t i = 0; i < num; ++i)
    {
        boost::shared_ptr<boost::thread> thread_shared_ptr
        (new boost::thread(boost::bind(&ThreadPool::ThreadRoutineIMP, this)));
        {
       //     boost::unique_lock<boost::recursive_mutex> lock(m_mutex);
            m_idThreadContainer[thread_shared_ptr->get_id()] = thread_shared_ptr;
        }
    }
}

void ThreadPool::ThreadRoutineIMP()
{
    Priority_t prio = LOW;
    boost::function<void(void)> func;
    boost::chrono::milliseconds timeout(/*1000*/30);
    static size_t counter_catch = 0;
    static size_t counter_while = 0;
    static size_t counter_try_after_while = 0;
    try
    {
        std::cout << "m_Isruninng = " << m_IsRunning.load(boost::memory_order_acquire) << "\n";
        while (true == m_IsRunning.load(boost::memory_order_acquire))
        {
            ++counter_while;
            std::cout << "counter_while = " << counter_while << "\n";
            Task task(func, prio);
            if (m_queue.Pop(task, timeout))
            {
                std::cout << "about to invoke function inside thread routine!!!!!!!!!!!!!\n";
                task.InvokeFunction();
            }
        }

        ++counter_try_after_while;
            std::cout << "counter_try_after_while = " << counter_try_after_while << "\n";
    }
    catch(boost::thread_interrupted &)
    {
        ++counter_catch;
            std::cout << "counter_catch = " << counter_catch << "\n";
        boost::unique_lock<boost::recursive_mutex> lock(m_mutex);
        m_idThreadContainer.erase(boost::this_thread::get_id());
        m_cond.notify_one();
    //    std::cout << "after notify one\n";
    }
}
/*-----------------------------------------------------------------------------
Task functions
------------------------------------------------------------------------------*/
ThreadPool::Task::Task(boost::function<void(void)> func_, Priority_t priority_)
    : m_func(func_)
    , m_priority(static_cast<int> (priority_))
{}

void ThreadPool::Task::InvokeFunction()
{
    m_func();
}

void ThreadPool::Task::BadApple(ThreadPool *this_)
{
 //   std::cout << "inside BadApple before interrupt" << std::endl;
    
    {
     //   boost::unique_lock<boost::recursive_mutex> lock(this_->m_mutex);
        this_->m_idThreadContainer[boost::this_thread::get_id()]->interrupt();
    }
    
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
ExEnum functions
------------------------------------------------------------------------------*/
/*ThreadPool::Task::ExEnum(bool isHighest, Priority_t priority_)
    : m_enum(isHighest ? static_cast<int> priority_ : BAD_APPLE_PRIO)
{}

ThreadPool::Task::~ExEnum()
{}*/
/*-----------------------------------------------------------------------------
MyQueue - Wrapper for waitable queue
------------------------------------------------------------------------------*/
ThreadPool::Task ThreadPool::MyQueue::front()
{
    return this->top();
}
} // end of namespace ilrd