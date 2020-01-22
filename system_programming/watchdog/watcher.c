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
#include "wd_utils.h"
#define WD_IMAGE 0 
#define APP_IMAGE 1

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
int IsDNR = 0;

static void *ThreadFunctionRoutineIMP(void *vars);
int IsAliveRoutine(void *action_func_param);
int SignalSenderRoutine(void *action_func_param);
int SchedulerStop(void *action_func_param);
static void ResetCounterHandlerIMP(int sig);
int CheckDNRFlagRoutine(void *action_func_param);

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
	
	thread_status = sem_open("/thread_status", O_CREAT, 0777, 0);
	if(SEM_FAILED == thread_status)
	{
		return FAIL;
	}
	
	status = pthread_create(&thread, NULL, ThreadFunctionRoutineIMP, &variables);
	if (FAIL == status)
	{
		return status;
	}
	
	pthread_detach(thread);

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
	pid_t own_pid = getpid();
	char own_pid_str[10] = {0};
	char interval_str[5] = {0};
	char dead_time_str[5] = {0};
	char *wd_env = {0};
	struct sigaction counter_handle = {0};
	int interval = 0;
	int dead_time = 0;
	char *argv = NULL;
	int retval = 0;
	status_t *status = NULL;
	thread_vars *variables = vars;
	scheduler_t *new_sched = NULL;
	char *watcher_argv[] = {"./wd.out", NULL};
	is_alive_param_t live_param = {0};
	printf("app end of variable declaration\n");

	printf("app end of declaration\n");
	interval = variables -> interval;
	printf("123interval = %d\n", interval);
	dead_time = variables -> dead_time;
	status = variables -> status;
	
	sprintf(own_pid_str, "%d\n", (int)own_pid);
	setenv("APP_ENV", own_pid_str, 0);
	sprintf(interval_str, "%d\n", (int)interval);
	setenv("ENV_INTERVAL", interval_str, 0);
	sprintf(dead_time_str, "%d\n", (int)dead_time);
	setenv("ENV_DEAD_TIME", dead_time_str, 0);

	live_param.dead_time = dead_time;
	
	thread_status = sem_open("/thread_status", O_CREAT, 0777, 0);
	thread_ready = sem_open("/thread_ready", O_CREAT, 0777, 0);
	wtchdg_ready = sem_open("/wtchdg_ready", O_CREAT, 0777, 0);
	
	counter_handle.sa_flags = 0;
	counter_handle.sa_handler = ResetCounterHandlerIMP;
	
	sigaction(SIGUSR1, &counter_handle, NULL);
	
	/* scheduler creation */
	printf("app start of creation\n");
	new_sched = SchedCreate();
	live_param.scheduler = new_sched;
	if (NULL == new_sched)
	{
		ReturnFail(thread_status);
	}
	if (UIDIsBad(SchedAdd(new_sched, (time_t)interval, SignalSenderRoutine, &wd_pid)))
	{
		printf("app SignalSenderRoutine failed to be added\n");
		
		ReturnFail(thread_status);
	}
	if (UIDIsBad(SchedAdd(new_sched, (time_t)interval, IsAliveRoutine, &live_param)))
	{
		printf("app IsAliveRoutine failed to be added\n");
		
		ReturnFail(thread_status);
	}
	if (UIDIsBad(SchedAdd(new_sched, (time_t)interval, CheckDNRFlagRoutine, new_sched)))
	{
		printf("app CheckDNRFlagRoutine failed to be added\n");
		
		ReturnFail(thread_status);
	}

	/* end of creation*/
	printf("app end of creation\n");
	if (NULL == getenv("WD_ENV"))
	{
		printf("inside if (NULL == getenv(WD_ENV))\n");
		printf("app right before fork\n");
		wd_pid = CreateProcess("./wd.out", variables -> argv);
		if (FAIL == *status)
		{
			ReturnFail(thread_status);
		}
	}	
	
	while (!IsDNR)
	{
		printf("app inside !IsDNR\n");
		printf("app dead_time = %d\n", dead_time);
		printf("app counter = %d\n", counter);
		if (dead_time == counter)
		{
			printf("inside if (dead_time == counter)\n");
			printf("app right before fork\n");
			wd_pid = CreateProcess("./wd.out", watcher_argv);
			if (FAIL == *status)
			{
				ReturnFail(thread_status);
			}
			printf("app inside dead_time == counter. counter = %d\n", counter);	
			counter = 0;
		}
		printf("app before sem_post\n");
		sem_post(thread_ready);
		printf("app before sem_wait\n");
		sem_wait(wtchdg_ready);
		printf("app after sem_wait\n");
	
		printf("app before sem_post(thread_status);\n");
		printf("sempost return %d\n", sem_post(thread_status));
		printf("app right before schedrun\n");
		SchedRun(new_sched);
		printf("				!!!!!!!!!!	app after schedrun	!!!!!!!!!\n");
	}
	
	printf("app exited loop due to dnr\n");
	/* app sends signal to wd to start rtermiante gracefully */
	printf("sending SIGUSR2 to pid %d\n", wd_pid);
	kill(wd_pid, SIGUSR2);
	
	sem_close(thread_status);
	sem_close(thread_ready);
	sem_close(wtchdg_ready);
	SchedDestroy(new_sched);
	pthread_exit(&retval);
	
	return NULL;
}

void DNR()
{
	printf("invoked DNR\n");
	
	IsDNR = 1;
}

int IsAliveRoutine(void *action_func_param)
{
	printf("app This is IsAlive routine\n");
	printf("app counter = %d\n", counter);
	++counter;
	
	if (((is_alive_param_t *)action_func_param) -> dead_time == counter)
	{
		printf("app inside counter reached limit\n");
		SchedStop(((is_alive_param_t *)action_func_param) -> scheduler);
	}
	
	return 1;
}

int SignalSenderRoutine(void *action_func_param)
{
	printf("app SignalSender to %d\n", *(pid_t *)action_func_param);
	
	kill(*(pid_t *)action_func_param, SIGUSR1);
	
	return 1;
}

int SchedulerStop(void *action_func_param)
{
	int x = 0;
	printf("app Sched stop\n");
	x= 2/0;
	
	return 0;
}

int CheckDNRFlagRoutine(void *action_func_param)
{
	printf("app CheckDNRFlagRoutine\n");
	printf("IsDNR = %d\n", IsDNR);
	if (IsDNR)
	{
		SchedStop((scheduler_t *)action_func_param);
	}	
	
	return 1;
}

static void ResetCounterHandlerIMP(int sig)
{
	UNUSED(sig);
	printf("app this is counter handler\n");
	
	counter = 0; 
}
