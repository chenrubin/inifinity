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


#define CHILD_PID 0
#define SNAME "/mysem"

static void MyParentSignalHandler(int sig);
int flag = 0;

int main(int argc, char* argv[])
{
	pid_t pid = atoi((char *)argv[1]);
	struct sigaction act;
	sem_t *sem = sem_open(SNAME, 1); 
	act.sa_flags = 0;
	act.sa_handler = MyParentSignalHandler;
	
	
	sigaction(SIGUSR2, &act, NULL);
	
	while (1)
	{		
		if (1 == flag)
		{
			write(0, "handler inside 	pong\n", 21);
			flag = 0;
		}

		kill(pid, SIGUSR1);
		sem_post(sem);
	}

	return 0;
}

static void MyParentSignalHandler(int sig)
{
	(void)sig;
	
	flag = 1;
}
