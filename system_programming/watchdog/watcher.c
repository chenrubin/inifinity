#define _POSIX_C_SOURCE

#include <pthread.h>
#include <fcntl.h>         
#include <sys/stat.h>     
#include <semaphore.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

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

static void *ThreadFunctionRoutineIMP(void *vars);

int main()
{
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
	pid_t pid_to_signal = {0};
	struct sigaction counter_handle = {0};
	int interval = 0;
	int dead_time = 0;
	char *argv = NULL;
	int argv_len = 0;
	status_t *status = NULL;
	thread_vars *variables = vars;
	scheduler_t *new_sched = NULL;
	ilrd_uid_t task_uid = {0};
	
	interval = variables -> interval;
	dead_time = variables -> dead_time;
	status = variables -> status;
	
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
	
	ilrd_uid_t SchedAdd(scheduler_t *scheduler, time_t interval, action_func action,
 void *action_func_param);
	
	free(argv);
	
	return NULL;
}
