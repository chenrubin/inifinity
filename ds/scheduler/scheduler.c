/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		scheduler					  *
*		7/11/2019					  *
*									  *
************************************/
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <unistd.h> /* sleep */
#include <stdio.h> /* printf */

#include "uid.h"
#include "task.h"
#include "pq.h"
#include "scheduler.h"

#define CURRENT_TIME time(NULL)

static int MyIsMatch(const void *new_data, const void *param);
static int MyCompareFunc(const void *new_data, 
						 const void *src_data, void *param);

struct scheduler
{
	p_queue_t *pq;
	int continue_running;
};

scheduler_t *SchedCreate(void)
{
	int priority_param = 0;
	
	scheduler_t *new_sched = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL == new_sched)
	{
		return NULL;
	}
	
	new_sched -> pq = PQCreate(&priority_param, MyCompareFunc);
	if (NULL == (new_sched -> pq))
	{
		free(new_sched);
		new_sched = NULL;
		
		return NULL;
	}
	
	new_sched -> continue_running = 1;
	
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
	task_t *new_task = TaskCreate(interval, action, action_func_param);
	PQEnqueue(scheduler -> pq, new_task);
	
	return (TaskGetID(PQPeek(scheduler -> pq)));
}

int SchedRemove(scheduler_t *scheduler, ilrd_uid_t event_id)
{
	void *res = PQErase(&event_id, scheduler -> pq, MyIsMatch);
	if (NULL == res)
	{
		return 1;
	}
	else
	
	TaskRemove(res);
	
	return 0;
}				

void SchedClear(scheduler_t *scheduler)
{
	assert(scheduler);
	
	while (!SchedIsEmpty(scheduler))
	{
		TaskRemove(PQDequeue(scheduler -> pq));
	}
}

int SchedIsEmpty(const scheduler_t *scheduler)
{
	assert(scheduler);
	
	return (PQIsEmpty(scheduler -> pq));
}

size_t SchedSize(const scheduler_t *scheduler)
{
	return (PQSize(scheduler -> pq));
}

enum result_status SchedRun(scheduler_t *scheduler)
{
	void *task = NULL;
	time_t time_to_run = 0;
	while (scheduler -> continue_running)
	{
		task = PQPeek(scheduler -> pq);
		time_to_run = TaskGetTimeToRun(task);
		sleep(time_to_run - CURRENT_TIME);

		if (!TaskRun((task_t *)task))
		{
			if (1 == SchedRemove(scheduler, TaskGetID((task_t *)task)))
			{
				return SCHEDULER_EMPTY;
			}
			else
			{
				return SUCCESS;
			}
		}
		else
		{
			TaskUpdateTimeToRun((task_t *)task);
			if (1 == PQEnqueue(scheduler -> pq, PQDequeue(scheduler -> pq)))
			{
				return SCHEDULER_EMPTY;
			}
		}
	}
	
	scheduler -> continue_running = 1;
	
	return SCHEDULER_EMPTY;
}

void SchedStop(scheduler_t *scheduler)
{
	scheduler -> continue_running = 0;
}

static int MyIsMatch(const void *new_data, const void *param)
{
	return (TaskIsMatchByID(*((ilrd_uid_t *)param), (task_t *)new_data));
}

static int MyCompareFunc(const void *new_data, const void *src_data, void *param)
{
	(void)param;
	
	if ((TaskGetTimeToRun((task_t *)src_data/*new_data*/)) < 
		(TaskGetTimeToRun((task_t *)new_data/*src_data*/)))
	{
		return -1;
	}
	else if ((TaskGetTimeToRun((task_t *)new_data)) == 
		(TaskGetTimeToRun((task_t *)src_data)))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
