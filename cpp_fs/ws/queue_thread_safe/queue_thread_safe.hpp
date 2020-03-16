#ifndef __QTS_HPP__
#define __QTS_HPP__

#include <stddef.h> /* size_t */
#include <semaphore.h> /* sem_t */
#include <pthread.h> /* mutex */
#include <fcntl.h>  /* O_CREAT */
//#include <sys/types.h>

#include "MyUtils.hpp"

pthread_mutex_t mutex;

namespace ilrd
{
template <typename T>
class Queue : private Uncopyable
{
public:
    explicit Queue(size_t capacity_);
    ~Queue();
    void Enqueue(const T& element_);
    T DEqueue();
private:
    bool IsEmpty() const;
    bool IsFull() const;

    const size_t m_capacity;
    const size_t m_elementSize;
    T *m_queue;
    size_t m_start;
    size_t m_end;
    sem_t *semEnqueue;
    sem_t *semDequeue;
};

template <typename T>
Queue<T>::Queue(size_t capacity_)
    : m_capacity(capacity_)
    , m_elementSize(sizeof(T))
    , m_queue(new T[m_capacity + 1])
    , m_start(0)
    , m_end(0)
    , semEnqueue(sem_open("Enq", O_CREAT, 0777, m_capacity))
    , semDequeue(sem_open("Deq", O_CREAT, 0777, 0))
{}

template <typename T>
Queue<T>::~Queue()
{
    delete[] m_queue;
    sem_close(semEnqueue);
    sem_close(semDequeue);
}

template <typename T>
void Queue<T>::Enqueue(const T& element_)
{
    pthread_mutex_lock(&mutex);
    sem_wait(semEnqueue);
    m_queue[m_start] = element_;
    sem_post(semDequeue);
    m_start = (m_start + 1) % (m_capacity + 1);
    pthread_mutex_unlock(&mutex);
}

template <typename T>
T Queue<T>::DEqueue()
{
    pthread_mutex_lock(&mutex);
    sem_wait(semDequeue);
    T t = m_queue[m_end];
    m_end = ((m_end + 1) % (m_capacity + 1));
    sem_post(semEnqueue);
    pthread_mutex_unlock(&mutex);
    
    return t;
}
/*
bool Queue<T>::IsEmpty() const
{
    return  (m_start == m_end);
}

bool Queue<T>::IsFull() const
{
    return  ((m_start + 1) % (m_capacity + 1) == m_end);
}
*/
} // end of namespace ilrd
#endif