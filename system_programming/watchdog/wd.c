#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>         
#include <sys/stat.h>     
#include <semaphore.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <time.h>
#include <errno.h>

#include "watcher.h"
/*#include "scheduler.h"*/
/*#include "uid.h"*/
/*#include "MyUtils.h"*/

int main()
{
	sem_t *wtchdg_ready = sem_open("moshe", 0);
	printf("I am watchdog\n");
	
	return 0;
}
/*
static void *WD(void *vars)
{
	sem_t *thread_status = NULL;
	sem_t *thread_ready = NULL;
	sem_t *wtchdg_ready = NULL;
	pid_t wd_pid = {0};
	pid_t own_pid = getpid();
	char own_pid_str[10] = {0};
	char *wd_env = {0};
	struct sigaction counter_handle = {0};
	int interval = 0;
	int dead_time = 0;
	char *argv = NULL;
	int argv_len = 0;
	status_t *status = NULL;
	thread_vars *variables = vars;
	scheduler_t *new_sched = NULL;
	ilrd_uid_t task_uid = {0};
	is_alive_param_t live_param = {0};
	int IsDNR = 0;
	int counter = 0;
	struct timespec ts = {0};
	
	/*convert pid_t to string*/
/*	sprintf(own_pid_str, "%d\n", (int)own_pid);
	setenv("APP_ENV", own_pid_str, 0);
	/* may fail*/
/*	
	interval = variables -> interval;
	dead_time = variables -> dead_time;
	status = variables -> status;
	ts.tv_sec = 2 * dead_time;
	ts.tv_nsec = 0;    

	live_param.scheduler = new_sched;
	live_param.dead_time = dead_time;
	
	argv_len = strlen(*(variables -> argv));
	argv = (char *)malloc(argv_len);
	if (NULL == argv)
	{
		*status = FAIL;
		return NULL;
	}
	
	memcpy(argv, *(variables -> argv), argv_len);
	
	thread_status = sem_open("thread_status", O_CREAT, 0777, 0);
	thread_ready = sem_open("thread_ready", O_CREAT, 0777, 0);
	wtchdg_ready = sem_open("wtchdg_ready", O_CREAT, 0777, 0);
	
	counter_handle.sa_flags = 0;
	counter_handle.sa_handler = ResetCounterHandlerIMP;
	
	sigaction(SIGUSR1, &counter_handle, NULL);
	
	/* scheduler creation */
/*	new_sched = SchedCreate();
	if (NULL == new_sched)
	{
		ReturnFail(thread_status);
	}
	if (UIDIsBad(SchedAdd(new_sched, (time_t)interval, SignalSenderRoutine, &wd_pid)))
	{
		printf("SignalSenderRoutine faild to be added\n");
		
		ReturnFail(thread_status);
	}
	if (UIDIsBad(SchedAdd(new_sched, (time_t)interval, IsAliveRoutine, &live_param)))
	{
		printf("IsAliveRoutine faild to be added\n");
		
		ReturnFail(thread_status);
	}
	if (UIDIsBad(SchedAdd(new_sched, 30, SchedulerStop,new_sched)))
	{
		printf("IsAliveRoutine faild to be added\n");
		
		ReturnFail(thread_status);
	}
	/* end of creation*/
	
/*	if (NULL == getenv("WD_ENV"))
	{
		wd_pid = fork();
		if (0 == wd_pid)
		{
			printf("inside child before exec\n");
			if (-1 == execvp("./wd.out", variables -> argv))
			{
				printf("exec failed\n");
			}
			printf("inside child after exec\n");
		}
	}	
	
	while (!IsDNR)
	{
		if (dead_time == counter)
		{
			wd_pid = fork();
			if (0 == wd_pid)
			{
				printf("inside child before exec\n");
				if (-1 == execvp("./wd.out", variables -> argv))
				{
					printf("exec failed\n");
				}
				printf("inside child after exec\n");
			}
			
			counter = 0;
		}
		
		sem_post(thread_ready);
		sem_wait(wtchdg_ready);
	/*	if (-1 == sem_timedwait(wtchdg_ready, &ts))
		{
			printf ("fail to timed wait\n");
			
			ReturnFail(thread_status);
		}
	*/	/*convert string to pid*/
/*		wd_env = getenv("WD_ENV");
		wd_pid = atoi(wd_env);
		/* may fail*/ 
/*		SchedRun(new_sched);
	}

	sem_post(thread_status);
	
	free(argv);
	
	return NULL;
}*/
