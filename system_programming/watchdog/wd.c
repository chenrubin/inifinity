#define _POSIX_C_SOURCE 200112L
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
#include "scheduler.h"
#include "uid.h"
#include "MyUtils.h"
#include "wd.h"
#include "wd_utils.h"
#define WD_IMAGE 0 
#define APP_IMAGE 1

int IsAliveRoutine(void *action_func_param);
int SignalSenderRoutine(void *action_func_param);
int SchedulerStop(void *action_func_param);
static void ResetCounterHandlerIMP(int sig);


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

typedef struct time_spec 
{
	time_t tv_sec;
	long tv_nsec;
}sem_time_t;

int counter = 0;

int main(int argc, char* argv[])
{
	printf("I am watchdog\n");
	/*
	
	*/
	WD((void **)argv);
	
	return 0;
}

void WD(void **vars)
{
	sem_t *thread_status = NULL;
	sem_t *thread_ready = NULL;
	sem_t *wtchdg_ready = NULL;
	pid_t app_pid = {0};
	pid_t own_pid = getpid();
	char own_pid_str[10] = {0};
	char *interval_str = NULL;
	char *dead_time_str = NULL;
	char *app_env = {0};
	struct sigaction counter_handle = {0};
	int interval = 0;
	int dead_time = 0;

/*	char *argv = NULL;*/
/*	int argv_len = 0;*/
	status_t *status = NULL;
	char **args_images = (char **)vars;
	/*thread_vars **variables = vars;*/
	scheduler_t *new_sched = NULL;
	is_alive_param_t live_param = {0};
	int IsDNR = 0;
	printf("wd end of declaration\n");
	
	interval_str = getenv("ENV_INTERVAL");
	dead_time_str = getenv("ENV_DEAD_TIME");
	interval = atoi(interval_str);
	dead_time = atoi(dead_time_str);
	
	printf("wd after convert of env variables\n");
	printf("wd start doing stuff\n");
	/*convert pid_t to string*/
	sprintf(own_pid_str, "%d\n", (int)own_pid);
	printf("setenv WD_ENV to %s\n", own_pid_str);
	setenv("WD_ENV", own_pid_str, 0);
	/* may fail*/
/*	interval = variables -> interval;
	dead_time = variables -> dead_time;
	status = variables -> status;
/*	ts.tv_sec = 2 * dead_time;
	ts.tv_nsec = 0;    
*/
	
	live_param.dead_time = dead_time;
	
/*	argv_len = strlen(*(variables -> argv));
	argv = (char *)malloc(argv_len);
	if (NULL == argv)
	{
		*status = FAIL;
		return NULL;
	}
	
	memcpy(argv, *(variables -> argv), argv_len);
*/	
	thread_status = sem_open("thread_status", O_CREAT, 0777, 0);
	thread_ready = sem_open("thread_ready", O_CREAT, 0777, 0);
	wtchdg_ready = sem_open("wtchdg_ready", O_CREAT, 0777, 0);
	
	counter_handle.sa_flags = 0;
	counter_handle.sa_handler = ResetCounterHandlerIMP;
	
	sigaction(SIGUSR1, &counter_handle, NULL);
	
	/* scheduler creation */
	new_sched = SchedCreate();
	live_param.scheduler = new_sched;
	if (NULL == new_sched)
	{
		ReturnFail(thread_status);
	}
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
/*	if (UIDIsBad(SchedAdd(new_sched, 10, SchedulerStop,new_sched)))
	{
		printf("wd IsAliveRoutine faild to be added\n");
		
		ReturnFail(thread_status);
	}
*/	/* end of creation*/
/*	printf("wd end of creation, WD_ENV = %s\n", getenv("WD_ENV"));
	if (NULL == getenv("WD_ENV"))
	{
		printf("wd inside WD_ENV = NULL, right before fork\n");
		*status = CreateProcess(&app_pid, variables -> argv, APP_IMAGE);
		if (FAIL == *status)
		{
			ReturnFail(thread_status);
		}
	}	
*/	
	while (!IsDNR)
	{
		printf("wd inside while !isdnr\n");
		if (dead_time == counter)
		{
			printf("wd inside dead_time == counter right before fork\n");
			printf("args_images = %s\n", args_images[0]);
			*status = CreateProcess(&app_pid, args_images, APP_IMAGE);
			printf("wd after create process\n");
			if (FAIL == *status)
			{
				printf("FAIL omg\n");
				ReturnFail(thread_status);
			}			
			counter = 0;
		}
		printf("wd before sem_post\n");
		sem_post(wtchdg_ready);
		printf("wd before sem_wait\n");
		sem_wait(thread_ready);
		printf("wd after sem_wait\n");
	/*	if (-1 == sem_timedwait(wtchdg_ready, &ts))
		{
			printf ("fail to timed wait\n");
			
			ReturnFail(thread_status);
		}
	*/	/*convert string to pid*/
		if (NULL != getenv("APP_ENV"))
		{
			printf("wd before app_env = getenv(APP_ENV); \n");
			app_env = getenv("APP_ENV");
			printf("wd before app_pid = atoi(app_env) \n");
			app_pid = atoi(app_env);
			/* may fail*/ 
		} 
		
		printf("wd before schedrun and after app_pid = atoi(app_env); which is %d \n", (int)app_pid);
		SchedRun(new_sched);
		printf("wd after schedrun \n");
	}
	
/*	free(argv);*/
	
/*	return NULL;*/
}

int IsAliveRoutine(void *action_func_param)
{
	printf("wd This is IsAlive routine\n");
	printf("counter = %d\n", counter);
	++counter;
	
	if (((is_alive_param_t *)action_func_param) -> dead_time == counter)
	{
		SchedStop(((is_alive_param_t *)action_func_param) -> scheduler);
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
	/*SchedStop((scheduler_t *)action_func_param);*/
	int x = 0;
	printf("wd Sched stop\n");
	
	x = 2/0;
	return 0;
}

static void ResetCounterHandlerIMP(int sig)
{
	UNUSED(sig);
	
	counter = 0; 
}
/*
int ReturnFail(sem_t *thread_status)
{
	int retval = 0;
	
	sem_post(thread_status);
	pthread_exit(&retval);
	
	return FAIL;
}*/
