#inlcude "queue_thread_safe.hpp"
#include <pthread.h> /* fork */

using namespace ilrd;

void *ThreadEnqueue(void *param);

int main()
{
	Queue q(10);
//	pthread_t 
   	return 0;
}
/*
template <typename T>
void *ThreadEnqueue<T>(void *param) 
{ 
    //std::cout << "Enqueue\n";
	(Queue *)param<T>
	
    return NULL;
} 
*/