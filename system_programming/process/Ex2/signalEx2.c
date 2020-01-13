#define _POSIX_C_SOURCE 200809L
	
#include <stdio.h>  /* printf */
#include <string.h>  /* printf strtok */
#include <stdlib.h>  /* system */
#include <unistd.h>	   /* fork */
#include <signal.h> /* SIGUSR1 */
#include <sys/wait.h>  /* wait */
#include <sys/types.h> /* pid_t */




#define CHILD_PID 0

static void MyParentSignalHandler(int sig);

int main()
{
	pid_t pid = 0;
	struct sigaction act;
	int status = 0;
	char *str[] = {"pong.c",NULL}; 
	size_t counter = 0;
	
	pid = fork();

	if (pid > 0)
	{
		printf("inside parent, pid = %d\n", getpid());
		act.sa_handler = MyParentSignalHandler;
		act.sa_flags = 0;
		sigaction(SIGUSR2, &act, NULL);
	}
	else
	{
		printf("inside child before execvp, pid = %d\n", getpid());
		if (-1 == execvp("./pong.out", str))
		{
			printf("exec failed\n");
		}
		printf("inside child after execvp, pid = %d\n", getpid());
	}
	
	while (1)
	{
		++counter;
/*		if (pid > 0) /* in the parent */
		{
			sleep(3);	
/*			printf("parent before sending ping to pid = %d\n", pid);
*/			kill(pid, SIGUSR1);
/*			printf("parent after sending ping to pid = %d and before pause\n", pid);
*/			pause();
/*			printf("counter = %ld\n", counter);
			printf("parent after pause\n");
*/			
		}
	}
	
	return 0;
}

static void MyParentSignalHandler(int sig)
{
	write(0,"CHILD handler of pong\n",23);
}
