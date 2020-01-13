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
static void MyChildSignalHandler(int sig);

int main()
{
	pid_t pid = 0;
	pid_t child_pid = 0;
	size_t counter = 0;
	struct sigaction act;
	
	pid = fork();

	if (pid > 0)
	{
		act.sa_handler = MyParentSignalHandler;
		sigaction(SIGUSR2, &act, NULL);
	}
	else
	{
		act.sa_handler = MyChildSignalHandler;
		sigaction(SIGUSR1, &act, NULL);
		child_pid = getppid();
	}
	
	while (1)
	{
		++counter;
		if (pid > 0) /* in the parent */
		{
			sleep(1);	
			printf("parent before sending ping\n");
			kill(pid, SIGUSR1);
			printf("parent after sending ping\n");
			pause();
			printf("parent2\n");
			printf("counter = %ld\n", counter);
			printf("parent5\n");
			
		}
		else /* in the child */
		{
			pause();
			printf("counter = %ld\n", counter);
			printf("child before sending pong\n");
			kill(child_pid, SIGUSR2);
			printf("child after sending pong\n");
		}
	}
	
	return 0;
}

static void MyParentSignalHandler(int sig)
{
	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	PARENT handler pong\n");
}

static void MyChildSignalHandler(int sig)
{
	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	CHILD handler of ping\n");
}
