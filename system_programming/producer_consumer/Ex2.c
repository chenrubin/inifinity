#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../../ds/dllist/dllist.h"

int global_var = 0;
int prod_lock = 0;
int cons_lock = 0;

void *producer(void *args);
void *consumer(void *args);

pthread_mutex_t lock;

typedef struct wrapper 
{
  int data;
  dl_list_t *dlist;
  sem_t *semaphore;
} wrap_t;

int main()
{
  int i = 0;
  pthread_t prod1 = 0;
  pthread_t prod2 = 0;
  pthread_t cons1 = 0;
  pthread_t cons2 = 0;
  sem_t *empty_list = sem_open("empty_list", O_CREAT, 0777, 0);
  dl_list_t *new_dlist = NULL;
  wrap_t prod_wrap1 = {0};
  wrap_t prod_wrap2 = {0};
  new_dlist = DLListCreate();
  

  prod_wrap1.data = 1;
  prod_wrap1.dlist = new_dlist;
  prod_wrap2.data = 2;
  prod_wrap2.dlist = new_dlist;

  
  printf("start of program\n");
    
  pthread_create(&prod1, NULL, producer, &prod_wrap1);
  pthread_create(&prod2, NULL, producer, &prod_wrap2);
  pthread_create(&cons1, NULL, consumer, new_dlist);
  pthread_create(&cons2, NULL, consumer, new_dlist);

  for (i = 0; i < 20; ++i)
  {
      sleep(1);
  }

  sem_close(empty_list);

  pthread_join(prod1, NULL);
  pthread_join(prod2, NULL);
  pthread_join(cons1, NULL);
  pthread_join(cons2, NULL);

  return 0;  
}

void *producer(void *args)
{       
    int i = 0;
	sem_t *empty_list = sem_open("empty_list", O_CREAT, 0777, 0);

    for (i = 0; i < 100; ++i)
    {
        pthread_mutex_lock(&lock);
		sem_post(empty_list);
        DLListPushFront(((wrap_t *)args) -> dlist, &(((wrap_t *)args) -> data));
        printf("DLListPushFront to %d\n", (int)((wrap_t *)args) -> data);
        printf("dlist size = %ld\n", DLListSize(((wrap_t *)args) -> dlist));
        pthread_mutex_unlock(&lock);
    }

    return 0;
}

void *consumer(void *args)
{
    int i = 0;
	sem_t *empty_list = sem_open("empty_list", O_CREAT, 0777, 0);

    for (i = 0; i < 100; ++i)
    {
	   sem_wait(empty_list);
       pthread_mutex_lock(&lock);
/*       if (!DLListIsEmpty((dl_list_t *)args))
       { */
		   
       DLListPopFront((dl_list_t *)args);
       printf("DLListPopFront to\n");
       printf("dlist size = %ld\n", DLListSize((dl_list_t *)args));
/*       }  */
       pthread_mutex_unlock(&lock);
    }

    return 0;
}
