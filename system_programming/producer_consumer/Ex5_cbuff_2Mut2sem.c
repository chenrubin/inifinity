#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../../ds/cbuff/cbuff.h"

#define CAPACITY 4

int global_var = 0;
int prod_lock = 0;
int cons_lock = 0;

void *producer(void *args);
void *consumer(void *args);

pthread_mutex_t write_lock;
pthread_mutex_t read_lock;

typedef struct wrapper
{
  cbuff_t *cbuff;
  char *arr;
  size_t counter;
} wrap_t;

int main()
{
  int i = 0;
  pthread_t prod1 = 0;
  pthread_t prod2 = 0;
  pthread_t cons1 = 0;
  pthread_t cons2 = 0;
  char src1[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','1', '2', '3', '4', '5' ,'6', '7' ,'8', '9', '0'};
  char src2[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J','K', 'L', 'M', 'N', 'O' ,'P', 'Q' ,'R', 'S', 'T'};
  char dst1[50] = {0};
  char dst2[50] = {0};
  cbuff_t *new_cbuff = CBuffCreate(CAPACITY);
	
  wrap_t prod_wrap1 = {0};
  wrap_t prod_wrap2 = {0};
  wrap_t cons_wrap1 = {0};
  wrap_t cons_wrap2 = {0};

  prod_wrap1.cbuff = new_cbuff;
  prod_wrap1.arr = src1;
  prod_wrap1.counter = 1;
  
  prod_wrap2.cbuff = new_cbuff;
  prod_wrap2.arr = src2;
  prod_wrap2.counter = 2;

  cons_wrap1.cbuff = new_cbuff;
  cons_wrap1.arr = dst1;
  cons_wrap1.counter = 3;
  
  cons_wrap2.cbuff = new_cbuff;
  cons_wrap2.arr = dst2;
  cons_wrap2.counter = 4;

  printf("start of program\n");
    
  pthread_create(&prod1, NULL, producer, &prod_wrap1);
  pthread_create(&prod2, NULL, producer, &prod_wrap2);
  pthread_create(&cons1, NULL, consumer, &cons_wrap1);
  pthread_create(&cons2, NULL, consumer, &cons_wrap2);
	
	
  for (i = 0; i < 5; ++i)
  {
      sleep(1);
  }
	
  pthread_join(prod1, NULL);
  pthread_join(prod2, NULL);
  pthread_join(cons1, NULL);
  pthread_join(cons2, NULL);
printf("HI\n");
  return 0;  
}

void *producer(void *args)
{       
    int i = 0;
    sem_t *sem_write = NULL;
	sem_t *sem_read = NULL;

	sem_write = sem_open("sem_write", O_CREAT, 0777, CAPACITY);
	sem_read = sem_open("sem_read", O_CREAT, 0777, 0);
	
    for (i = 0; i < 100; ++i)
    {
		sem_wait(sem_write);        
		pthread_mutex_lock(write_lock);
		CBuffWrite(((wrap_t *)args) -> cbuff, ((wrap_t *)args) -> arr, 1);
        printf("CBuffWrite %ld\n", ((wrap_t *)args) -> counter);
        printf("size =  %ld\n", CBuffSpaceLeft(((wrap_t *)args) -> cbuff));
        pthread_mutex_unlock(write_lock);
        sem_post(sem_read);
    }
    
    sem_close(sem_write);
  	sem_close(sem_read);

    return 0;
}

void *consumer(void *args)
{
    int i = 0;
    sem_t *sem_write = NULL;
	sem_t *sem_read = NULL;
	
	sem_write = sem_open("sem_write", O_CREAT, 0777, CAPACITY);
	sem_read = sem_open("sem_read", O_CREAT, 0777, 0);

    for (i = 0; i < 100; ++i)
    {
	  	sem_wait(sem_read);        
		pthread_mutex_lock(read_lock);
		CBuffRead(((wrap_t *)args) -> cbuff, ((wrap_t *)args) -> arr, 1);
		printf("CBuffRead %ld\n", ((wrap_t *)args) -> counter);
		printf("size =  %ld\n", CBuffSpaceLeft(((wrap_t *)args) -> cbuff));
        pthread_mutex_unlock(read_lock);
        sem_post(sem_write);
    }
    
    sem_close(sem_write);
  	sem_close(sem_read);

    return 0;
}
