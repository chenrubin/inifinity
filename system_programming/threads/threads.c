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

int arr[100000] = {0};

/* this function is run by the second thread */
void *printArr(void *x_void_ptr)
{
	/* increment x to 100 */
	int *index = (int *)x_void_ptr;

	arr[*index] = *index;

	/* the function must return something - NULL will do */
	return NULL;
}

int main()
{
	pthread_t thread;
	int i = 0;
	
	for (i = 0; i < 100000; ++i)
	{
		if(pthread_create(&thread, NULL, printArr, &i)) 
		{
			fprintf(stderr, "Error creating thread\n");
			
			return 1;
		}
		
		pthread_join(thread, NULL);
	}
	
	for (i = 0; i < 100000; ++i)
	{
		printf("arr [%d] = %d\n", i,i);
	}
	
	return 0;
}
