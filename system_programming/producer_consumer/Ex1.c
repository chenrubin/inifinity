#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int global_var = 0;
int prod_lock = 0;
int cons_lock = 0;

void *producer(void *args);
void *consumer(void *args);

int main()
{
  pthread_t prod = 0;
  pthread_t cons = 0;
  int args = 0;
  int i = 0;

  printf("start of program\n");
    
  pthread_create(&prod, NULL, producer, &args);
  pthread_create(&cons, NULL, consumer, &args);

  for (i = 0; i < 5; ++i)
  {
      sleep(1);
  }

  return 0;  
}

void *producer(void *args)
{    
    while (1)
    {
        
        while (__sync_lock_test_and_set(&prod_lock, 1))
        {
            ;
        }        

        printf("producer increments global_var\n");
        ++global_var;
        printf("global_var = %d\n", global_var);
        
        __sync_lock_release (&cons_lock);
    }

    return 0;
}

void *consumer(void *args)
{
    while (1)
    {
         while (__sync_lock_test_and_set(&cons_lock, 1))
        {
            ;
        }        
        
  /*      lock = 1;*/

        printf("consumer reads global_var\n");
        printf("global_var = %d\n", global_var);

        
       __sync_lock_release (&prod_lock);
    }

    return 0;
}
