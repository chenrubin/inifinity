#define _POSIX_C_SOURCE 200809L
	
#include <stdio.h>  /* printf */
#include <string.h>  /* printf strtok */
#include <stdlib.h>  /* system */
#include <unistd.h>	   /* fork */
#include <signal.h> /* SIGUSR1 */
#include <sys/wait.h>  /* wait */
#include <sys/types.h> /* pid_t */




#define CHILD_PID 0

static void MyParentSignalHandler(int sig, siginfo_t *siginfo, void *context);

pid_t g_pid = 0;
int flag = 1;

int main()
{
	pid_t pid = 0;
	struct sigaction act;
	siginfo_t *siginfo = NULL;
	act.sa_sigaction = MyParentSignalHandler;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);
	printf("ping's pid = %d\n", getpid());
	pause();
/*	pid = siginfo -> si_pid;*/
	while (1)
	{
		if (1 == flag)
		{
			write(0, "handler inside ping\n", 21);
			flag = 0;
		}
		
		kill(g_pid, SIGUSR2);
		sleep(1);
	}
	
	return 0;
}

static void MyParentSignalHandler(int sig, siginfo_t *siginfo, void *context)
{
	(void)sig;
	(void)context;
	
	g_pid = siginfo -> si_pid;
	flag = 1;
}
