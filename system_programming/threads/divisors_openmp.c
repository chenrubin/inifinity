 #define _POSIX_C_SOURCE 200809L
	
#include <stdio.h>  /* printf */
/*#include <pthread.h> /*pthread*/
#include <omp.h>

#define NUM_OF_THREADS 64
typedef struct nums
{
	size_t start;
	size_t num_of_checks;
	size_t full_number;
/*	size_t counter;*/
}numbers_t;

numbers_t struct_arr[NUM_OF_THREADS];
size_t sum_arr[NUM_OF_THREADS];

/*pthread_t pthreads_arr[NUM_OF_THREADS];*/
	
/* this function is run by the second thread */
void *CheckDivide(void *param)
{
	size_t i = 0;
	size_t sum = 0;
	size_t num = 0;
	
	for (i = 1; i <= ((numbers_t *)param) -> num_of_checks; ++i)
	{
		num = ((numbers_t *)param) -> start + i;
		if (0 == (((numbers_t *)param) -> full_number) % num)
		{
			sum += num;
		}
	}

	return (void *)sum;
}

int main()
{
	int i = 0;
	size_t num = 4800000000;
	size_t num_per_thread = 0;
	int res = 0;
	void *ret_val = NULL;
	size_t num_of_divisors = 0;
	size_t thread_num = 0;

	num_per_thread = num / NUM_OF_THREADS;

	#pragma omp parallel num_threads(NUM_OF_THREADS) private(thread_num)
	{
		thread_num = omp_get_thread_num();
		struct_arr[thread_num].start = num_per_thread * thread_num;
		struct_arr[thread_num].num_of_checks = num_per_thread;
		struct_arr[thread_num].full_number = num;
		sum_arr[thread_num] = (size_t)(CheckDivide(&struct_arr[thread_num]));
	}
	
	for (i = 0; i < NUM_OF_THREADS; ++i)
	{	
		num_of_divisors += sum_arr[i];
	} 

	printf("num of divisors for %ld = %ld\n", num, num_of_divisors);	
	
	return 0;
}
