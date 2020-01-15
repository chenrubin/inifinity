#define _POSIX_C_SOURCE 200809L
	
#include <stdio.h>  /* printf */
#include <pthread.h> /*pthread*/
/*#include <string.h>  /* printf strtok */
/*#include <stdlib.h>  /* system */
/*#include <unistd.h>	   /* fork */
/*#include <signal.h> /* SIGUSR1 */
/*#include <sys/wait.h>  /* wait */
/*#include <sys/types.h> /* pid_t */
/*#include <pthread.h> /*pthread*/

#define NUM_OF_THREADS 64
typedef struct nums
{
	size_t start;
	size_t num_of_checks;
	size_t full_number;
/*	size_t counter;*/
}numbers_t;

numbers_t struct_arr[NUM_OF_THREADS];
pthread_t pthreads_arr[NUM_OF_THREADS];
	
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
	pthread_t thread;
	int i = 0;
	size_t num = 4800000000;
	size_t num_per_thread = 0;
	int res = 0;
	void *ret_val = NULL;
	size_t num_of_divisors = 0;
	
/*	printf("Please enter a 12 digit number\n");
	scanf("%ld", &num);	
*/	num_per_thread = num / NUM_OF_THREADS;
	
	for (i = 0; i < NUM_OF_THREADS; ++i)
	{
		struct_arr[i].start = num_per_thread * i;
		struct_arr[i].num_of_checks = num_per_thread;
		struct_arr[i].full_number = num;
		
		res = pthread_create(&thread, NULL, CheckDivide, &struct_arr[i]);
		if(0 != res) 
		{
			res = pthread_create(&thread, NULL, CheckDivide, &struct_arr[i]);
		}
		pthreads_arr[i] = thread;
	}
	
	for (i = 0; i < NUM_OF_THREADS; ++i)
	{
		pthread_join(pthreads_arr[i], &ret_val);
		num_of_divisors += (size_t)ret_val;
	} 
	
	printf("num of divisors for %ld = %ld\n", num, num_of_divisors);	
	
	return 0;
}
