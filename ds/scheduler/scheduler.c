/************************************
*		Author: ChenR				  *
*		Reviewer: Maoz				  *
*		scheduler					  *
*		22/11/2019					  *
*									  *
************************************/
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <unistd.h> /* sleep */
#include <stdio.h>

#include "../uid/uid.h"
#include "../task/task.h"
#include "../pqheap/pqheap.h"
#include "scheduler.h"
#include "../../chen/MyUtils.h"

#define CURRENT_TIME time(NULL)

/* function to use with PQErase */
static int MyIsMatchIMP(const void *new_data, const void *param);

/* function to use when creating scheduler for PQCreate*/
static int MyCompareFuncIMP(const void *new_data, 
						 const void *src_data, void *param);
						 
/* remove running task */						 
static void RemoveRunningTaskIMP(scheduler_t *scheduler);

/* calculates time to sleep */
static double GetTimeToSleepUntilNextTaskIMP(task_t *task);

struct scheduler
{
	p_queue_t *pq;
	int continue_running;
	task_t *running_task;
	int is_removing_itself;
};

scheduler_t *SchedCreate(void)
{
	void *priority_param = NULL;
	scheduler_t *new_sched = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL == new_sched)
	{
		return NULL;
	}
	
	new_sched -> pq = PQCreate(priority_param, MyCompareFuncIMP);
	if (NULL == (new_sched -> pq))
	{
		free(new_sched);
		new_sched = NULL;
		
		return NULL;
	}
	
	new_sched -> continue_running = 1;
	new_sched -> running_task = NULL;
	new_sched -> is_removing_itself = 0;
	
	return new_sched;
}

void SchedDestroy(scheduler_t *scheduler)
{
	assert(scheduler);
	
	SchedClear(scheduler);
	PQDestroy(scheduler -> pq);
	free(scheduler);
	scheduler = NULL;
}

ilrd_uid_t SchedAdd(scheduler_t *scheduler, time_t interval, action_func action,
 					void *action_func_param)
{
	task_t *new_task = NULL;
	int is_failed = 0;
	
	assert(scheduler);
 	assert(action);
 	
	new_task = TaskCreate(interval, action, action_func_param);
	if (NULL == new_task)
	{
		return BAD_UID;
	}
	
	is_failed = PQEnqueue(scheduler -> pq, new_task);
	if (is_failed)
	{
		TaskRemove(new_task);
		
		return BAD_UID;
	}
	
	return TaskGetID(new_task);
}

int SchedRemove(scheduler_t *scheduler, ilrd_uid_t event_id)
{
	task_t *task_to_remove = NULL;
	int status = 0;
	
	assert(scheduler);
	
	task_to_remove = PQErase(&event_id, scheduler -> pq, MyIsMatchIMP);
	if (NULL == task_to_remove)
	{
		if (NULL != (scheduler -> running_task))
		{
			if (TaskIsMatchByID(event_id, scheduler -> running_task))
			{
				scheduler -> is_removing_itself = 1;
			}
			else
			{
				status = 1;
			}
		}
		else
		{
			status = 1;
		}
	}
	else
	{
		TaskRemove(task_to_remove);
	}
	
	return status;
}				

void SchedClear(scheduler_t *scheduler)
{
	assert(scheduler);
	
	while (!SchedIsEmpty(scheduler))
	{
		if ((NULL != scheduler->running_task) && 1 == SchedSize(scheduler)) 
		{
			scheduler -> is_removing_itself = 1;
			break;
		}
		
		TaskRemove(PQDequeue(scheduler->pq));	
	}
}

int SchedIsEmpty(const scheduler_t *scheduler)
{
	assert(scheduler);
	
	if (scheduler -> running_task)
	{
		return 0;
	}
	else
	{
		return (PQIsEmpty(scheduler -> pq));
	}
}

size_t SchedSize(const scheduler_t *scheduler)
{
	size_t result = 0;
	
	assert(scheduler);
	
	result = PQSize(scheduler -> pq);
	
	if (scheduler -> running_task)
	{
		result += 1;
	}
	
	return result;
}

enum result_status SchedRun(scheduler_t *scheduler)
{
	int is_repeat = 0;
	int is_failed = 0;
	scheduler -> continue_running = 1;
	while ((!SchedIsEmpty(scheduler)) && (scheduler -> continue_running))
	{
		scheduler -> running_task = PQDequeue(scheduler -> pq);
		sleep(GetTimeToSleepUntilNextTaskIMP(scheduler -> running_task));
		is_repeat = TaskRun(scheduler -> running_task);

		if (!(scheduler -> is_removing_itself) && is_repeat)
		{
			TaskUpdateTimeToRun(scheduler -> running_task);
			is_failed = PQEnqueue(scheduler -> pq, scheduler -> running_task);
			
			if (1 == is_failed)
			{
				return ENQUEUE_FAILED;
			}
			
			scheduler -> running_task = NULL;
		}
		else
		{
			RemoveRunningTaskIMP(scheduler);
		}
	}
	
	if (0 == SchedIsEmpty(scheduler))
	{
		return STOPPED_SUCCESSFULLY;
	}
	else
	{
		return SCHEDULER_EMPTY;
	}
}

void SchedStop(scheduler_t *scheduler)
{
	assert(scheduler);
	
	scheduler -> continue_running = 0;
}

static int MyIsMatchIMP(const void *new_data, const void *param)
{
	return (TaskIsMatchByID(*((ilrd_uid_t *)param), (task_t *)new_data));
}

static int MyCompareFuncIMP(const void *new_data, const void *src_data, void *param)
{
	time_t src_data_time = 0;
	time_t new_data_time = 0;
	
	src_data_time = TaskGetTimeToRun((task_t *)src_data);
	new_data_time = TaskGetTimeToRun((task_t *)new_data);
	
	(void)param;
	
	if (src_data_time < new_data_time)
	{
		return -1;
	}
	else if (src_data_time == new_data_time)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

static double GetTimeToSleepUntilNextTaskIMP(task_t *task)
{
    return (MAX2(0, difftime(TaskGetTimeToRun(task), CURRENT_TIME))); 
}

static void RemoveRunningTaskIMP(scheduler_t *scheduler)
{
	TaskRemove(scheduler -> running_task);
	scheduler -> is_removing_itself = 0;
	scheduler -> running_task = NULL;
}
