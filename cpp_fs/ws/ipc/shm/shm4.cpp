#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h> 
#include <iostream>

int main()
{
    key_t key = ftok("shmfile",65);
    int shmid = shmget(key, 1024, 0666);

    /* creates the shm in an address decided by OS */
    char *str = (char *)shmat(shmid ,(void *)0 ,0);

    std::cout << "str = " << str << std::endl; 
    
    shmdt(str);
}