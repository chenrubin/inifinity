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
int flag = 0;

int main(int argc, char* argv[])
{
/*	printf("argv[1] = %d\n", atoi((char *)argv[1]));*/
	pid_t pid = atoi((char *)argv[1]);
	struct sigaction act;
	act.sa_flags = 0;
	act.sa_handler = MyParentSignalHandler;
	sigaction(SIGUSR2, &act, NULL);
	
	while (1)
	{
		sleep(1);
		
		if (1 == flag)
		{
			write(0, "handler inside pong\n", 21);
			flag = 0;
		}
		
		kill(pid, SIGUSR1);
	}

	return 0;
}

static void MyParentSignalHandler(int sig)
{
	(void)sig;
	
	flag = 1;
}
