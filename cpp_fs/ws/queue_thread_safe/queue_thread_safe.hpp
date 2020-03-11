#ifndef __QTS_HPP__
#define __QTS_HPP__

#include "MyUtils.hpp"

namespace ilrd
{
template <typename T>
class Queue: private Uncopyable
{
public:
    explicit Queue(size_t capacity_);
    ~Queue();
    void Enqueue(const T& element_);
    T DEqueue():
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
}
#endif