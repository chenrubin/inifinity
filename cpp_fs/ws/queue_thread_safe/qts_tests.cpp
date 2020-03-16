#include <pthread.h> /* fork */
#include <iostream> /* cout */

#include "queue_thread_safe.hpp"

using namespace ilrd;

template <typename T>
struct thread_param
{
	Queue<T> q;
	size_t element_size;
};

template <typename T>
void *ThreadEnqueue(void *param);

void TestEnqueue();

int main()
{
	TestEnqueue();

   	return 0;
}

template <typename T>
void *ThreadEnqueue(void *param)
{
	((thread_param<T> *)param)->q.Enqueue(((thread_param<T> *)param)->element_size);
	
    return 0;
}

void TestEnqueue()
{
	int arr[] = {1,2,3,4,5,6,7,8,9,10};
	size_t len = sizeof(arr) / sizeof(arr[0]);
	size_t capacity = 10;
	Queue<int> new_q(capacity);

	/* changing len to len + 1 result in loop getting stuck
	as expected */
	for (size_t i = 0; i < len; ++i)
	{
		new_q.Enqueue(arr[i]);
	}
	std::cout << "finished enqueuing" << std::endl;

	for (size_t i = 0; i < len; ++i)
	{
		new_q.DEqueue();
	}

	std::cout << "finished dequeuing" << std::endl;
/*	pthread_t thread_en1;
	pthread_t thread_en2;
	int thread_arg = 0;
	size_t capacity = 20;
	size_t element_size = 4;

	Queue<int> new_q(capacity);
	thread_param<int> tp;
	tp.q = new_q;
	tp.element_size = element_size;

	pthread_create(&thread_en1, NULL, ThreadEnqueue, &tp);
	pthread_create(&thread_en2, NULL, ThreadEnqueue, &tp);
*/
	
}
