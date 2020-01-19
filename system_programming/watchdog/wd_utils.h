#ifndef ILRD_WATCHDOG_H
#define ILRD_WATCHDOG_H

typedef enum 
{
	SUCCESS = 0,
	FAIL = 1
}status_t;

void ReturnFail(sem_t *thread_status);
int CreateProcess(pid_t *pid, char **argv, int isApp);
/*int IsAliveRoutine(void *action_func_param);
int SignalSenderRoutine(void *action_func_param);
int SchedulerStop(void *action_func_param);*/

#endif
