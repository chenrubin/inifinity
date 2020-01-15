#define _POSIX_C_SOURCE 200809L
	
#include <stdio.h>  /* printf */
#include <string.h>  /* printf strtok */
#include <stdlib.h>  /* system */
#include <unistd.h>	   /* fork */
#include <signal.h> /* SIGUSR1 */
#include <sys/wait.h>  /* wait */
#include <sys/types.h> /* pid_t */
#include <semaphore.h> /* sem_open */
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <pthread.h>


#define CHILD_PID 0
#define SNAME "kolp"

static void MyParentSignalHandler(int sig);
sig_atomic_t flag = 1;

int main(int argc, char* argv[])
{
	pid_t pid = atoi((char *)argv[1]);
	struct sigaction act;
	sem_t *sem = sem_open(SNAME, 1); 
	int i = 0;
	act.sa_flags = 0;
	act.sa_handler = MyParentSignalHandler;
	
	
	
	sigaction(SIGUSR2, &act, NULL);
	
	while (i < 1000)
	{		
		if (1 == flag)
		{
			write(0, "handler inside 	pong\n", 21);
			flag = 0;
			kill(pid, SIGUSR1);
			
		}
	sem_post(sem);
	++i;
	}
	
	sem_close(sem);
	sem_unlink(SNAME);

	return 0;
}

static void MyParentSignalHandler(int sig)
{
	(void)sig;
	
	flag = 1;
}
