// Thread Pool - Header file
// Last update: 9/4/2020
// Author: chen
// Reviewer: Tsisana
/*----------------------------------------------------------------------------*/
#ifndef __THREAD_POOL_HPP__ 
#define __THREAD_POOL_HPP__
/*----------------------------------------------------------------------------*/
#include <boost/function.hpp>       // boost::function
#include <boost/thread.hpp>         // boost::thread
#include <boost/chrono.hpp>         // boost::chrono

#include "MyUtils.hpp"              // class Uncopyable
#include "waitable_queue.hpp"       // class WaitableQueue
namespace ilrd
{
/*----------------------------------------------------------------------------*/
/* ThreadPool class designed for decrease the time of the creation and destroying
 * threads in a program. With threadPool, one can create number of threads in 
 * the beginning of the program and add tasks for them in runtime. The number of
 * threads can also changed in runtime. Every added task should have priority 
 * rate from type Priority_t. When ThreadPool not used anymore, the user should
 * call the function Stop, Closing the program without it is unspecified and 
 * could cause problems like memory-leaks and runtime-errors. */

/* compile with:
 * -lboost_chrono
 * -lboost_thread
 * -lboost_system */
class ThreadPool: private Uncopyable
{
public:
    enum Priority_t
    {
        LOW = 0,
        MEDIUM,
        HIGH,
        NUM_OF_PRIORITIES
    };

    // throw bad_alloc
    explicit ThreadPool(size_t numOfThreads_ = 
                        boost::thread::hardware_concurrency());

    // throw (boost::thread_interrupted) 
    ~ThreadPool();

    // throw bad_alloc
    // Add tasks to threadpool, a function with its priority
    void AddTask(boost::function<void(void)> func, Priority_t priority_);

    // It is the user's responsibilty to choose an endTime long enough in order 
    // for the current tasks to be over. Otherwise behavior is undefined and 
    // memory leek is possible
    // user must not set timeout to less than 10 miliseconds
    // throw (boost::thread_interrupted)
    void Stop(boost::chrono::milliseconds endTime_);

    // throw bad_alloc
    // Set number of threads in thread pool. Can be changed in run_time
    void SetThreadsNum(size_t numOfThreads_);

    // Get number of threads in thread pool
    size_t GetThreadsNum() const NOEXCEPT;

private:
    enum Extended_prio_t
    {
        EXT_OF_PRIORITIES = NUM_OF_PRIORITIES,
        BAD_APPLE = 100,
        Max_EXT_ENUM = 200
    };

    class Task
    {
    public:
        Task(boost::function<void(void)> func_, Extended_prio_t priority_);
        void InvokeFunction(bool *isTerminated);
        int GetPriority() const;

    private:
        boost::function<void(void)> m_func;
        int m_priority;

        void Wrapper(bool *isTerminated);
    };

    class CompareFunc
    {
    public:
        bool operator()(const Task &task1_, const Task &task2_);
    };

    typedef std::priority_queue<Task, std::vector<Task>, CompareFunc> queue_t;
    class MyQueue : private queue_t
    {
    public:
        typedef Task& reference;
        typedef const Task& const_reference;
        
        using queue_t::push;
        using queue_t::pop;
        using queue_t::empty;

        Task front();
    };    
    
    void IncreaseThreadNumIMP(size_t num);
    void DecreaseThreadNumIMP(size_t num);
    void ThreadRoutineIMP();
    bool IsSizeMatchIMP(size_t size);
    void EmptyEntireQueueIMP();
    void AddTaskIMP(boost::function<void(void)> func, 
                    Extended_prio_t priority_);
    void BadAppleIMP();
    void UpdateNumOfThreadsIMP(size_t num);

    const boost::chrono::milliseconds popTimeout;
    const boost::chrono::milliseconds stopTimeout;
    WaitableQueue<MyQueue> m_queue;
    boost::atomic<bool> m_IsRunning;
    std::map<boost::thread::id, boost::shared_ptr<boost::thread> > m_idThreadContainer;
    std::map<boost::thread::id, boost::shared_ptr<boost::thread> > m_FinishedThreads;
    boost::atomic<size_t> m_numOfThreads;
    mutable boost::mutex m_containerMutex;
    boost::mutex m_threadsMutex;
    boost::condition_variable_any m_cond;
};

/*----------------------------------------------------------------------------*/
}   // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif  // __THREAD_POOL_HPP__