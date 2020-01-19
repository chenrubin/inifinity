#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>

#include "wd_utils.h"

int CreateProcess(pid_t *pid, char **argv, int is_app)
{
	char *image_name = NULL;
	char *gh[] = {"./a.out", NULL};
	*pid = fork();
	
/*	if (is_app)
	{
		image_name = argv[0];
		printf("argv[0] = %s\n", argv[0]);
	}
	else
	{
		image_name = "./wd.out";
	}
*/	
	if (0 < *pid)
	{
		/* Put the parent to sleep for 2 seconds--let the child finished executing */
		sleep(2);
		printf("inside parent!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
/*		if (is_app)
		{
			printf("inside parent WD");
		}
		else
		{
			printf("inside parent app");
		}
*/		
	}
	else/*if (0 == *pid)*/
	{
		if (is_app)
		{
			printf("wd inside childghlik before exec\n");
			printf("wd inside childghlik before exec2\n");
			execvp("./a.out", gh);
		}	
		else
		{
			printf("app inside child before exec\n");	
			execvp("./wd.out", argv);
		}
				
		printf("wd inside childghlik before exec3\n");
/*		if (-1 == execvp(image_name, argv))
		{
			printf("exec failed\n");
			return 1;
		}
*/
		printf("wd inside childghlik before exec4\n");
		printf("app inside child after exec\n");
	}
	
	
	return 0;
}

void ReturnFail(sem_t *thread_status)
{
	int retval = 0;
	
	sem_post(thread_status);
	pthread_exit(&retval);
}
/*
if (UIDIsBad(SchedAdd(new_sched, (time_t)interval, SignalSenderRoutine, &app_pid)))
{
	printf("SignalSenderRoutine faild to be added\n");
	
	ReturnFail(thread_status);
}
if (UIDIsBad(SchedAdd(new_sched, (time_t)interval, IsAliveRoutine, &live_param)))
{
	printf("wd IsAliveRoutine faild to be added\n");
	
	ReturnFail(thread_status);
}

int CreateScheduler(scheduler_t *new_sched, time_t interval, )
{
	
}

int IsAliveRoutine(void *action_func_param)
{
	printf("wd This is IsAlive routine\n");
	++((is_alive_param_t *)action_func_param) -> counter;
	printf("counter = %d\n", counter);
	if (((is_alive_param_t *)action_func_param) -> dead_time == counter)
	{
		SchedStop(((is_alive_param_t *)action_func_param) -> scheduler);
		
		return 0;
	}
	
	return 1;
}

int SignalSenderRoutine(void *action_func_param)
{
	printf("wd This is SignalSender routine\n");
	
	kill(*(pid_t *)action_func_param, SIGUSR1);
	
	return 1;
}

int SchedulerStop(void *action_func_param)
{
	SchedStop((scheduler_t *)action_func_param);
	printf("wd Sched stop\n");
	
	return 0;
}*/
