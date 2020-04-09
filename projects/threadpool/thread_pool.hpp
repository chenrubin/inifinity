// Thread Pool - Header file
// Last update: 2/4/2020
// Author: 
/*----------------------------------------------------------------------------*/
#ifndef __THREAD_POOL_HPP__ 
#define __THREAD_POOL_HPP__
/*----------------------------------------------------------------------------*/
#include <boost/function.hpp>       // boost::function
#include <boost/thread.hpp>         // boost::thread
#include <boost/chrono.hpp>         // boost::chrono

#include "MyUtils.hpp"              // class Uncopyable
#include "waitable_queue.hpp"       // class WaitableQueue
/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
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

    explicit ThreadPool(size_t numOfThreads_ /* = default optimum number of threads from boost */);
    ~ThreadPool();

    // throw bad_alloc
    void AddTask(boost::function<void(void)> func, Priority_t priority_);

    // It is the user's responsibilty to choose an endTime long enough in order 
    // for the current tasks to be over. Otherwise behavior is undifined and 
    // memory leek is possible
    void Stop(boost::chrono::milliseconds endTime_);

    // Set number of threads in thread pool. Can be changed in run_time
    void SetThreadsNum(size_t numOfThreads_);

    // Get number of threads in thread pool
    size_t GetThreadsNum() const;

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
        void InvokeFunction();
        static void BadApple(ThreadPool *this_);

        class CompareFunc
        {
        public:
            bool operator()(Task task1_, Task task2_);
        };

    private:
        boost::function<void(void)> m_func;
        int m_priority;
    };

    class MyQueue : private std::priority_queue<Task, 
                                                std::vector<Task>, 
                                                Task::CompareFunc>
    {
    public:
        typedef Task& reference;
        typedef const Task& const_reference;
        using std::priority_queue<Task, std::vector<Task>, Task::CompareFunc>::push;
        using std::priority_queue<Task, std::vector<Task>, Task::CompareFunc>::pop;
        using std::priority_queue<Task, std::vector<Task>, Task::CompareFunc>::empty;

        Task front();
    };    
    
    void IncreaseThreadNumIMP(size_t num);
    void DecreaseThreadNumIMP(size_t num);
    void ThreadRoutineIMP();
    bool IsSizeMatchIMP(size_t size);
    void EmptyEntireQueueIMP();
    void AddTaskIMP(boost::function<void(void)> func, 
                    Extended_prio_t priority_);

    WaitableQueue<MyQueue> m_queue;
    boost::atomic<bool> m_IsRunning;
    std::map<boost::thread::id, boost::shared_ptr<boost::thread> > m_idThreadContainer;
    mutable boost::mutex m_mutex;
    boost::mutex m_mutex_pool;
    boost::condition_variable_any m_cond;
};

/*----------------------------------------------------------------------------*/
}   // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif  // __THREAD_POOL_HPP__