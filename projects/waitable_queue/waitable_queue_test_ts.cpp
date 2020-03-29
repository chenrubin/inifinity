#include <queue>  // std::queue 
#include <iostream> // std::cout, std::endl
#include <pthread.h> 
#include <sys/stat.h>
#include <fcntl.h> 
#include <cerrno>
#include <cstdio>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#include "waitable_queue.hpp"

#define NUM_OF_THREADS (10)

using namespace ilrd;
/*----------------------------------------------------------------------------*/
namespace
{
boost::mutex mutexForTest;
WaitableQueue <std::queue<int> > queue;
/*----------------------------------------------------------------------------*/
void *PushTest1IMP(void *arg);
void *PushTest2IMP(void *arg);
void *PushTest3IMP(void *arg);
void *PopIMP(void *arg);
void *PopTest3IMP(void *arg);
void Test1();
void Test2();
void Test3();
/*----------------------------------------------------------------------------*/
struct box
{
	size_t threadPush;
	size_t threadPop;
};
/*----------------------------------------------------------------------------*/
} // end anonymus namespace
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
int main()
{
	std::cout <<"test1-----------------------------\n";
	Test1();
	
	std::cout <<"test2-----------------------------\n";
	Test2();
	
	std::cout <<"test3-----------------------------\n";
	Test3();
	
	return 0;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
namespace
{
/*----------------------------------------------------------------------------*/
void Test2()
{
	struct box box[NUM_OF_THREADS];
	
	pthread_t threadPush[1] = { 0 };
	box[0].threadPush = 1;
	if (0 != pthread_create(threadPush, NULL, PushTest2IMP, box))
	{
		perror("");
	}
	
	pthread_t threadPop[NUM_OF_THREADS] = { 0 };
	for (size_t i = 0; i < NUM_OF_THREADS; ++i)
	{
		box[i].threadPop = i;
		
		if (0 != pthread_create(threadPop + i, NULL, PopIMP, box + i))
		{
			perror("");
		}
	}
	
	
	for (size_t i = 0; i < NUM_OF_THREADS; ++i)
	{
		pthread_join(threadPop[i], NULL);
	}
	pthread_join(threadPush[0], NULL);
}
/*----------------------------------------------------------------------------*/
void Test1()
{
	pthread_t threadPop[NUM_OF_THREADS] = { 0 };
	struct box box[NUM_OF_THREADS];

	for (size_t i = 0; i < NUM_OF_THREADS; ++i)
	{
		box[i].threadPop = i;
		
		if (0 != pthread_create(threadPop + i, NULL, PopIMP, box + i))
		{
			perror("");
		}
	}
	
	pthread_t threadPush[1] = { 0 };
	box[0].threadPush = 1;
	if (0 != pthread_create(threadPush, NULL, PushTest1IMP, box))
	{
		perror("");
	}
	
	for (size_t i = 0; i < NUM_OF_THREADS; ++i)
	{
		pthread_join(threadPop[i], NULL);
	}
	pthread_join(threadPush[0], NULL);
}
/*----------------------------------------------------------------------------*/
void Test3()
{
	pthread_t threadPop[NUM_OF_THREADS] = { 0 };
	struct box box[NUM_OF_THREADS];

	for (size_t i = 0; i < NUM_OF_THREADS; ++i)
	{
		box[i].threadPop = i;
		
		if (0 != pthread_create(threadPop + i, NULL, PopTest3IMP, box + i))
		{
			perror("");
		}
	}
	
	pthread_t threadPush[1] = { 0 };
	box[0].threadPush = 1;
	if (0 != pthread_create(threadPush, NULL, PushTest3IMP, box))
	{
		perror("");
	}
	
	for (size_t i = 0; i < NUM_OF_THREADS; ++i)
	{
		pthread_join(threadPop[i], NULL);
	}
	pthread_join(threadPush[0], NULL);
}
/*----------------------------------------------------------------------------*/
void *PushTest1IMP(void *arg)
{ 
	static int i = 0;
	
	static int counter1 = 5;
	while(counter1)
	{
		queue.Push(50 + i);
		
	/*	mutexForTest.lock();
		std::cout << "pushed element is\t" << 50 + i<<std::endl;
		mutexForTest.unlock();
	*/	
		--counter1;
		++i;
	}
	
	static int counter2 = 5;
	while(counter2)
	{
		queue.Push(40 + i);
		
	/*	mutexForTest.lock();
		std::cout << "pushed element is\t" << 40 + i<<std::endl;
		mutexForTest.unlock();
	*/	
		--counter2;
		++i;
	}
	
	return NULL;
		 
}
/*----------------------------------------------------------------------------*/
void *PushTest2IMP(void *arg)
{ static int i = 0;
	
	static int counter1 = 5;
	while(counter1)
	{
		queue.Push(50 + i);
		
	/*	mutexForTest.lock();
		std::cout << "pushed element is\t" << 50 + i<<std::endl;
		mutexForTest.unlock();
	*/	
		--counter1;
		++i;
	}
	
	static int counter2 = 5;
	while(counter2)
	{
		queue.Push(40 + i);
		
	/*	mutexForTest.lock();
		std::cout << "pushed element is\t" << 40 + i<<std::endl;
		mutexForTest.unlock();
	*/	
		--counter2;
		++i;
	}
	
	return NULL;
		 
}
/*----------------------------------------------------------------------------*/
void *PopIMP(void *arg)
{
	int someTemp = 0;
	queue.Pop(someTemp);
	if (0 != someTemp)
	{
		mutexForTest.lock();
//		std::cout << "poped element is\t";
//		std::cout << someTemp<<std::endl;
		mutexForTest.unlock();
	}

	return NULL;
}
/*----------------------------------------------------------------------------*/
void *PushTest3IMP(void *arg)
{ 
	static int i = 0;
	
	static int counter1 = 2;
	while(counter1)
	{
		queue.Push(50 + i);
		
		mutexForTest.lock();
	//	std::cout << "pushed element is\t" << 50 + i<<std::endl;
		mutexForTest.unlock();
		
		--counter1;
		++i;
	}
	
	static int counter2 = 2;
	while(counter2)
	{
		queue.Push(40 + i);
		
		mutexForTest.lock();
	//	std::cout << "pushed element is\t" << 40 + i<<std::endl;
		mutexForTest.unlock();
		
		--counter2;
		++i;
	}
	
	return NULL;
	
	return NULL;
}
/*----------------------------------------------------------------------------*/
void *PopTest3IMP(void *arg)
{
	boost::chrono::milliseconds timeout(1000);
	int someTemp = 0;
	bool result = queue.Pop(someTemp, timeout);
	
	mutexForTest.lock();
//	std::cout<<"result bool = "<<result<<std::endl;
	if (0 != someTemp)
	{
//		std::cout << "poped element is\t";
//		std::cout << someTemp<<std::endl;
	}
	mutexForTest.unlock();

	return NULL;
}
/*----------------------------------------------------------------------------*/
} // end anonymus namespace

