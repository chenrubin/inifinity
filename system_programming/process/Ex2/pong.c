#define _POSIX_C_SOURCE 200809L
	
#include <stdio.h>  /* printf */
#include <string.h>  /* printf strtok */
#include <stdlib.h>  /* system */
#include <unistd.h>	   /* fork */
#include <signal.h> /* SIGUSR1 */
#include <sys/wait.h>  /* wait */
#include <sys/types.h> /* pid_t */

static void MyChildSignalHandler(int sig);

int main()
{
	pid_t pid = getppid();
	pid_t own_pid = getpid();
	struct sigaction act;
	
	act.sa_handler = MyChildSignalHandler;
	act.sa_flags = 0;
	sigaction(SIGUSR1, &act, NULL);
	pause();
	while (1)
	{
/*		printf("in the child with pid = %d right before pause\n", own_pid);
*/		sleep(1);
	/*	printf("counter = %ld\n", counter);*/
/*		printf("child before sending pong\n");
*/		kill(pid, SIGUSR2);
/*		printf("child after sending pong\n");
*/	}	
}

static void MyChildSignalHandler(int sig)
{
	write(0,"CHILD handler of ping\n",23);
/*	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	CHILD handler of ping\n");*/
}
