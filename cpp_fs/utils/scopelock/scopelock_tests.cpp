#include <boost/thread/mutex.hpp> /* boost::mutex */

#include "MyUtils.hpp"
#include "scopelock.hpp"

using namespace ilrd;

std::mutex std_mutex;
boost::mutex boost_mutex;
std::recursive_mutex rec_mutex;
pthread_mutex_t mutex_t = PTHREAD_MUTEX_INITIALIZER;

void Std_mutex_test();
void Ctor_for_std_mutex(std::mutex *std_mutex);
void Dtor_for_std_mutex(std::mutex *std_mutex);

void Boost_mutex_test();
void Ctor_for_boost_mutex(boost::mutex *boost_mutex);
void Dtor_for_boost_mutex(boost::mutex *boost_mutex);

void Pthread_mutex_test();
void Ctor_for_pthread_mutex(pthread_mutex_t *pthread_mutex);
void Dtor_for_pthread_mutex(pthread_mutex_t *pthread_mutex);

int main()
{
//	Std_mutex_test();
//	Boost_mutex_test();
	Pthread_mutex_test();


	return 0;
}

void Std_mutex_test()
{
	Ctor_for_std_mutex(&std_mutex);
	Dtor_for_std_mutex(&std_mutex);
}

void Ctor_for_std_mutex(std::mutex *std_mutex)
{
	ScopeLock<std::mutex> std_mut(std_mutex);
	bool isLocked = std_mutex->try_lock();
	
	PRINTTESTRESULTS("Std_mutex_test - Lock",1, false == isLocked);
}

void Dtor_for_std_mutex(std::mutex *std_mutex)
{
	try
	{
		ScopeLock<std::mutex> std_mut(std_mutex);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	PRINTTESTRESULTS("Std_mutex_test - Unlock",1, true);
}

void Boost_mutex_test()
{
	Ctor_for_boost_mutex(&boost_mutex);
	Dtor_for_boost_mutex(&boost_mutex);
}

void Ctor_for_boost_mutex(boost::mutex *boost_mutex)
{
	ScopeLock<boost::mutex> boost_mut(boost_mutex);
	bool isLocked = boost_mutex->try_lock();
	
	PRINTTESTRESULTS("Boost_mutex_test - Lock",1, false == isLocked);
}

void Dtor_for_boost_mutex(boost::mutex *boost_mutex)
{
	try
	{
		ScopeLock<boost::mutex> boost_mut(boost_mutex);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	PRINTTESTRESULTS("boost_mutex_test - Unlock",1, true);
}

void Pthread_mutex_test()
{
	Ctor_for_pthread_mutex(&mutex_t);
	Dtor_for_pthread_mutex(&mutex_t);
}

void Ctor_for_pthread_mutex(pthread_mutex_t *pthread_mutex)
{
	ScopeLock<pthread_mutex_t> pthread_mut(pthread_mutex);
	int status = pthread_mutex_trylock(pthread_mutex);
	
	PRINTTESTRESULTS("Pthread_mutex_test - Lock",1, 0 != status);
}

void Dtor_for_pthread_mutex(pthread_mutex_t *pthread_mutex)
{
	try
	{
		ScopeLock<pthread_mutex_t> pthread_mut(pthread_mutex);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	PRINTTESTRESULTS("pthread_mutex_test - Unlock",1, true);
}

