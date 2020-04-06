#include <queue>    // queue, priority_queue
#include <pthread.h> /* pthread */
#include <boost/chrono.hpp>

#include "waitable_queue.hpp"
#include "MyUtils.hpp"

#define ARGS ((ThreadArgs *)param)
#define ARGS_SUPER ((ThreadArgsSuper *)param)
#define SUPER_NUM_OF_POP_THREADS 4
#define SUPER_NUM_OF_PUSH_THREADS 2
#define SUPER_POP_ITERATIONS 5
#define SUPER_NUM_OF_ELEMENTS 10

using namespace ilrd;

struct ThreadArgs
{
    WaitableQueue<std::queue<int> > *queue;
    int element;
    boost::chrono::milliseconds timeout;
};

struct ThreadArgsSuper
{
    WaitableQueue<std::queue<int> > *queue;
    int *element_arr;
    size_t arr_size;
};

class CompareFunc
{
public:
    bool operator()(int num1, int num2) const;
};

class MyQueue : private std::priority_queue<int, std::vector<int>, CompareFunc>
{
public:
    typedef int& reference;
    typedef const int& const_reference;
    using std::priority_queue<int, std::vector<int>, CompareFunc>::push;
    using std::priority_queue<int, std::vector<int>, CompareFunc>::pop;
    using std::priority_queue<int, std::vector<int>, CompareFunc>::empty;
    
    int front();

private:
};

void TestPushPop();
void TestPushPopWTimer();
void TestPushPopSuper();
void TestPushPQ();

void *ThreadPushRoutineIMP(void *param);
void *ThreadPopRoutineIMP(void *param);
void *ThreadPopWTimerRoutineIMP(void *param);
void *ThreadPopRoutineSuperIMP(void *param);
void *ThreadPushRoutineSuperIMP(void *param);

int main()
{
    TestPushPop();
    TestPushPopWTimer();
    TestPushPopSuper(); 
    TestPushPQ();

    return 0;
}

void TestPushPop()
{
    boost::chrono::milliseconds timeout(500);
    size_t i = 0;
    const size_t num_of_elements = 14;
    WaitableQueue<std::queue<int> > *myWaitQueue = new WaitableQueue<std::queue<int> >;
    int num_arr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};
    ThreadArgs thread_args[num_of_elements];
    
    for (i = 0; i < num_of_elements; ++i)
    {
        thread_args[i].queue = myWaitQueue;
        thread_args[i].element = num_arr[i];
        thread_args[i].timeout = timeout;
    }

    pthread_t thread_arr[num_of_elements];

    for (i = 0; i < num_of_elements; ++i)
    {
        myWaitQueue->Push(num_arr[i]);
    }

    PRINTTESTRESULTS("TestPushPop - Push",1, false == myWaitQueue->IsEmpty());

    for (i = 0; i < num_of_elements; ++i)
    {
        pthread_create(&thread_arr[i], NULL, ThreadPopRoutineIMP, &thread_args[i]);
    }

    for (i = 0; i < num_of_elements; ++i)
    {
        int res;
        void *ret_val = &res;
        pthread_join(thread_arr[i], &ret_val);
        std::cout << *static_cast<int *>(ret_val) << ", ";
    }
    std::cout << std::endl;

    PRINTTESTRESULTS("TestPushPop - Pop",2, true == myWaitQueue->IsEmpty());

    delete myWaitQueue;

    std::cout << "\n\n";
}

void TestPushPopWTimer()
{
    boost::chrono::milliseconds timeout(3000);
    size_t i = 0;
    const size_t num_of_elements = 14;
    WaitableQueue<std::queue<int> > *myWaitQueue = new WaitableQueue<std::queue<int> >;
    int num_arr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};
    ThreadArgs thread_args[num_of_elements];
    
    for (i = 0; i < num_of_elements; ++i)
    {
        thread_args[i].queue = myWaitQueue;
        thread_args[i].element = num_arr[i];
        thread_args[i].timeout = timeout;
    }

    pthread_t thread_arr[num_of_elements];

    for (i = 0; i < num_of_elements - 1; ++i)
    {
        myWaitQueue->Push(num_arr[i]);
    }

    PRINTTESTRESULTS("TestPushPopWTimer - Push",1, false == myWaitQueue->IsEmpty());

    for (i = 0; i < num_of_elements; ++i)
    {
        pthread_create(&thread_arr[i], NULL, ThreadPopWTimerRoutineIMP, &thread_args[i]);
    }

    for (i = 0; i < num_of_elements; ++i)
    {
        int res;
        void *ret_val = &res;
        pthread_join(thread_arr[i], &ret_val);
        std::cout << *static_cast<int *>(ret_val) << ", ";
    }
    std::cout << std::endl;

    PRINTTESTRESULTS("TestPushPopWTimer - Pop",2, true == myWaitQueue->IsEmpty());

    delete myWaitQueue;

    std::cout << "\n\n";
}

void TestPushPopSuper()
{
    boost::chrono::milliseconds timeout(500);
    size_t i = 0;
    size_t j = 0;
    WaitableQueue<std::queue<int> > *myWaitQueue = new WaitableQueue<std::queue<int> >;
    int num_arr[SUPER_NUM_OF_PUSH_THREADS][SUPER_NUM_OF_ELEMENTS];
    for (i = 0; i < SUPER_NUM_OF_PUSH_THREADS; ++i)
    {
        for (j = 0; j < SUPER_NUM_OF_ELEMENTS; ++j)
        {
            num_arr[i][j] = i * SUPER_NUM_OF_PUSH_THREADS + j;
        }
    }
    
    ThreadArgsSuper thread_args_super[SUPER_NUM_OF_PUSH_THREADS];
    ThreadArgs thread_args[SUPER_NUM_OF_POP_THREADS];
    
    for (i = 0; i < SUPER_NUM_OF_PUSH_THREADS; ++i)
    {
        thread_args_super[i].queue = myWaitQueue;
        thread_args_super[i].element_arr = num_arr[i];
        thread_args_super[i].arr_size = 10;
    }

    for (i = 0; i < SUPER_NUM_OF_POP_THREADS; ++i)
    {
        thread_args[i].queue = myWaitQueue;
        thread_args[i].element = 700;
        thread_args[i].timeout = timeout;
    }

    pthread_t pop_thread_arr[SUPER_NUM_OF_POP_THREADS];
    pthread_t push_thread_arr[SUPER_NUM_OF_PUSH_THREADS];

    for (i = 0; i < SUPER_NUM_OF_PUSH_THREADS; ++i)
    {
        pthread_create(&push_thread_arr[i], NULL, ThreadPushRoutineSuperIMP, &thread_args_super[i]);
    }

    for (i = 0; i < SUPER_NUM_OF_POP_THREADS; ++i)
    {
        pthread_create(&pop_thread_arr[i], NULL, ThreadPopRoutineSuperIMP, &thread_args[i]);
    }

    for (i = 0; i < SUPER_NUM_OF_PUSH_THREADS; ++i)
    {
        int res;
        void *ret_val = &res;
        pthread_join(push_thread_arr[i], &ret_val);
     //   std::cout << *static_cast<int *>(ret_val) << ", ";
    }

    for (i = 0; i < SUPER_NUM_OF_POP_THREADS; ++i)
    {
        int res;
        void *ret_val = &res;
        pthread_join(pop_thread_arr[i], &ret_val);
     //   std::cout << *static_cast<int *>(ret_val) << ", ";
    }
    std::cout << std::endl;

    PRINTTESTRESULTS("TestPushPopSuper - Pop",2, true == myWaitQueue->IsEmpty());

    delete myWaitQueue;

    std::cout << "\n\n";
}

void TestPushPQ()
{
    WaitableQueue<MyQueue> *myWaitQueue = new WaitableQueue<MyQueue>;
    int arr[] = {5,0,-6,25,19};

    for (int i = 0; i < 5; ++i)
    {
        myWaitQueue->Push(arr[i]);
    }

    int element;
    myWaitQueue->Pop(element);
    PRINTTESTRESULTS("TestPushPQ",1, 25 == element);
    myWaitQueue->Pop(element);
    PRINTTESTRESULTS("TestPushPQ",1, 19 == element);
    myWaitQueue->Pop(element);
    PRINTTESTRESULTS("TestPushPQ",1, 5 == element);
    myWaitQueue->Pop(element);
    PRINTTESTRESULTS("TestPushPQ",1, 0 == element);
    myWaitQueue->Pop(element);
    PRINTTESTRESULTS("TestPushPQ",1, -6 == element);

    delete myWaitQueue;

    std::cout << "\n\n";
}

void *ThreadPushRoutineIMP(void *param)
{
    ARGS->queue->Push(ARGS->element);

    return 0;
}

void *ThreadPushRoutineSuperIMP(void *param)
{
     for (size_t i = 0; i < ARGS_SUPER->arr_size; ++i)
     {
         ARGS_SUPER->queue->Push(ARGS_SUPER->element_arr[i]);
         sleep(1);
     }

    return 0;
}

void *ThreadPopRoutineIMP(void *param)
{
    ARGS->queue->Pop(ARGS->element);
    
    return (&(ARGS->element));
}

void *ThreadPopRoutineSuperIMP(void *param)
{
    for (size_t i = 0; i < SUPER_POP_ITERATIONS; ++i)
    {
        ARGS->queue->Pop(ARGS->element);
        sleep(1);
    }

    return (&(ARGS->element));
}

void *ThreadPopWTimerRoutineIMP(void *param)
{
    ARGS->queue->Pop(ARGS->element, ARGS->timeout);

    return (&(ARGS->element));
}

bool CompareFunc::operator()(int num1, int num2) const
{
    return (num1 < num2);
}

int MyQueue::front()
{
    return this->top();
}