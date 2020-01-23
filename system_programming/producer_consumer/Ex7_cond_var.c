#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../../ds/cbuff/cbuff.h"

#define NUM_OF_CONSUMERS 8
int global_var = 0;
int prod_lock = 0;
int cons_lock = 0;

void *producer(void *args);
void *consumer(void *args);

pthread_mutex_t lock;
int counter_g = 0;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

typedef struct wrapper
{
  cbuff_t *cbuff;
  char *arr;
  size_t counter;
} wrap_t;

int main()
{
  int i = 0;
  int param = 0;
  pthread_t threads[NUM_OF_CONSUMERS + 1] = {0};

  pthread_create(threads, NULL, producer, &param);
  for (i = 1; i < NUM_OF_CONSUMERS + 1; ++i)
  {
  	 pthread_create(threads + i, NULL, consumer, &param);
  }		    
	
  for (i = 0; i < 10; ++i)
  {
      sleep(1);
  }
	
  for (i = 0; i < NUM_OF_CONSUMERS + 1; ++i)
  {
  	 pthread_join(threads[i], NULL);
  }	

  return 0;  
}

void *producer(void *args)
{       
    int i = 0;
    int j = 0;
    sem_t *sem_consumer = sem_open("sem_cons", O_CREAT, 0777, 0);
	
    for (i = 0; i < 100; ++i)
    {
    	for (j = 0; j < NUM_OF_CONSUMERS; ++j)
  		{
			sem_wait(sem_consumer);        
		}
		
		++counter_g;
		pthread_mutex_lock(&lock);
		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&lock);
    }
    
    sem_close(sem_consumer);

    return 0;
}

void *consumer(void *args)
{
    int i = 0;
    sem_t *sem_consumer = sem_open("sem_cons", O_CREAT, 0777, 0);

    for (i = 0; i < 100; ++i)
    {
  		pthread_mutex_lock(&lock);
  		sem_post(sem_consumer);
  		pthread_cond_wait(&cond,&lock);
  		pthread_mutex_unlock(&lock);
  		printf("counter = %d\n", counter_g);
    }
    
    sem_close(sem_consumer);

    return 0;
}
