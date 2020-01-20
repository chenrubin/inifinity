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
/*	if (0 < pid)
	{
		printf("inside parent!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		if (is_app)
		{
			puts("inside parent WD");
		}
		else
		{
			puts("inside parent app");
		}
		
	}*/
	if (0 == pid)/*if (0 == *pid)*/
	{
		printf("before calling execvp on image %s\n", image);
		printf("argv = %s and %s\n", argv[0], argv[1]);
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
