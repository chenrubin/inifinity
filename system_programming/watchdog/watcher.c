#define _POSIX_C_SOURCE

#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>         
#include <sys/stat.h>     
#include <semaphore.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h> 

#include "watcher.h"
#include "scheduler.h"
#include "uid.h"

typedef enum 
{
	SUCCESS = 0,
	FAIL = 1
}status_t;

typedef struct
{
	int interval;
	int dead_time;
	char **argv;
	status_t *status;
}thread_vars;

typedef struct task_struct
{
	scheduler_t *scheduler;
	int dead_time;
}is_alive_param_t;

static void *ThreadFunctionRoutineIMP(void *vars);
int IsAliveRoutine(void *action_func_param);
int SignalSenderRoutine(void *action_func_param);
int SchedulerStop(void *action_func_param);

int main()
{
	int interval = 1;
	int dead_time = 5;
	char *argv[] = {"moshe", NULL};
	int status = MMI(argv, interval, dead_time);
	
	printf("status = %d\n", status);
	
	return 0;
}

int MMI(char *argv[], int interval, int dead_time)
{
	pthread_t thread = {0};
	status_t status = 0;
	sem_t *thread_status = NULL;
	thread_vars variables = {0};
	
	variables.interval = interval;
	variables.dead_time = dead_time;
	variables.argv = argv;
	variables.status = &status;
	
	thread_status = sem_open("thread_status", O_CREAT, 0777, 0);
	if(SEM_FAILED == thread_status)
	{
		return FAIL;
	}
	
	status = pthread_create(&thread, NULL, ThreadFunctionRoutineIMP, (void *)&variables);
	if (FAIL == status)
	{
		return status;
	}

	sem_wait(thread_status);
	sem_close(thread_status);
	
	return status;
}

static void *ThreadFunctionRoutineIMP(void *vars)
{
	sem_t *thread_status = NULL;
	sem_t *thread_ready = NULL;
	sem_t *wtchdg_ready = NULL;
	pid_t wd_pid = {0};
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
	
	interval = variables -> interval;
	dead_time = variables -> dead_time;
	status = variables -> status;
	
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
/*	counter_handle.sa_handler = SetOffCounter;*/
	
	sigaction(SIGUSR1, &counter_handle, NULL);
	
	/* scheduler creation */
	new_sched = SchedCreate();
	/* check fail */
	if (UIDIsBad(SchedAdd(new_sched, (time_t)interval, SignalSenderRoutine,&wd_pid)))
	{
		printf("SignalSenderRoutine faild to be added\n");
	}
	if (UIDIsBad(SchedAdd(new_sched, (time_t)interval, IsAliveRoutine,&live_param)))
	{
		printf("IsAliveRoutine faild to be added\n");
	}
	if (UIDIsBad(SchedAdd(new_sched, 10, SchedulerStop,new_sched)))
	{
		printf("IsAliveRoutine faild to be added\n");
	}
	
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
	else
	{
		printf("I am thread\n");
	}
	
	SchedRun(new_sched);
	
	sem_post(thread_status);
	
	free(argv);
	
	return NULL;
}

int IsAliveRoutine(void *action_func_param)
{
	printf("This is IsAlive routine\n");
	
	return 1;
}

int SignalSenderRoutine(void *action_func_param)
{
	printf("This is SignalSender routine\n");
	
	return 1;
}

int SchedulerStop(void *action_func_param)
{
	SchedStop((scheduler_t *)action_func_param);
	printf("Sched stop\n");
	
	return 0;
}
