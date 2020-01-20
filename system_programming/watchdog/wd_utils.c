#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>

#include "wd_utils.h"

pid_t CreateProcess(char *image, char *argv[])
{
	pid_t pid = fork();
	if (0 > pid)
	{
		return 0;
	}	

	if (0 == pid)
	{
		if (-1 == execvp(image, argv))
		{
			puts("exec failed\n");
			return 0;
		}
		puts("jwieofw");
	}
	
	return pid;
}

void ReturnFail(sem_t *thread_status)
{
	int retval = 0;
	
	sem_post(thread_status);
	pthread_exit(&retval);
}
