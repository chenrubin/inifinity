#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h> 
#include <cstring>

int main()
{
    key_t key = ftok("shmfile",65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    /* creates the shm in an address decided by OS */
    char *str = (char *)shmat(shmid ,(void *)0 ,0);

    strcpy(str , "moshe");
    
  //  int shmdt(str);
}