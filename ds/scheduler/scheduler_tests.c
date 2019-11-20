#include <stdio.h> /* printf */
#include <string.h> /* strlen */
#include <unistd.h> /* access */

#include "scheduler.h"
#include "uid.h"
#include "MyUtils.h" /* MAX2,MIN2 */

void TestSchedulerCreate();
void TestSchedulerAddRemove();
void TestClear();
void TestRun();
void TestStop();
void TestWithFiles();

int MyTaskFunction1(void *action_func_param);
int MyTaskFunction2(void *action_func_param);
int MyTaskFunction3(void *action_func_param);
int MyTaskFunction4(void *action_func_param);

int main()
{
/*	TestSchedulerCreate();
	TestSchedulerAddRemove();
	TestClear();	
	TestRun();
	TestStop();
*/	TestWithFiles();	
	
	return 0;
}

void TestSchedulerCreate()
{
	scheduler_t *new_sched = SchedCreate();
	printf("Create scheduler\n");
	printf("Destroy scheduler\n");
	SchedDestroy(new_sched);
	
	printf("\n\n");
}

void TestSchedulerAddRemove()
{
	time_t interval = 5;
	ilrd_uid_t uid = {0};
	int action_func_param = 0;
	
	scheduler_t *new_sched = SchedCreate();
	printf("Create scheduler\n");

	PRINTTESTRESULTS("TestSchedulerAdd_Size",1, 0 == SchedSize(new_sched));
	printf("Add task");
	uid = SchedAdd(new_sched, interval, MyTaskFunction1, &action_func_param);
	PRINTTESTRESULTS("TestSchedulerAdd_Size",2, 1 == SchedSize(new_sched));	
	
	PRINTTESTRESULTS("TestSchedulerAdd_Remove",2, 
					  0 == SchedRemove(new_sched, uid));
	PRINTTESTRESULTS("TestSchedulerAdd_Size",3, 0 == SchedSize(new_sched));

	printf("Destroy scheduler\n");
	SchedDestroy(new_sched);
	
	printf("\n\n");
}

void TestClear()
{
	time_t interval = 3;
	time_t interval1 = 1;
	size_t action_func_param = 50;
	
	scheduler_t *new_sched = SchedCreate();
	printf("Create scheduler\n");
	
	PRINTTESTRESULTS("TestClear_IsEmpty",1, 1 == SchedIsEmpty(new_sched));
	printf("Add Task\n");
	SchedAdd(new_sched, interval, MyTaskFunction1, &action_func_param);
	PRINTTESTRESULTS("TestClear_IsEmpty",2, 0 == SchedIsEmpty(new_sched));
	printf("Add Task\n");
	SchedAdd(new_sched, interval1, MyTaskFunction1, &action_func_param);
	PRINTTESTRESULTS("TestClear_IsEmpty",3, 0 == SchedIsEmpty(new_sched));
	printf("Add Task\n");
	SchedAdd(new_sched, interval1, MyTaskFunction1, &action_func_param);
	PRINTTESTRESULTS("TestClear_IsEmpty",4, 0 == SchedIsEmpty(new_sched));
	printf("Add Task\n");
	SchedAdd(new_sched, interval1, MyTaskFunction1, &action_func_param);
	PRINTTESTRESULTS("TestClear_IsEmpty",5, 0 == SchedIsEmpty(new_sched));
	PRINTTESTRESULTS("TestClear_Size",6, 4 == SchedSize(new_sched));
	printf("Clear Scheduler\n");
	SchedClear(new_sched);
	PRINTTESTRESULTS("TestClear_IsEmpty",7, 1 == SchedIsEmpty(new_sched));
	PRINTTESTRESULTS("TestClear_Size",8, 0 == SchedSize(new_sched));

	printf("Destroy scheduler\n");
	SchedDestroy(new_sched);
	
	printf("\n\n");
}

void TestRun()
{
	time_t interval1 = 3;
	time_t interval2 = 1;
	size_t action_func_param = 10;
	
	scheduler_t *new_sched = SchedCreate();
	printf("Create scheduler\n");
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval1, MyTaskFunction1, &action_func_param);
	PRINTTESTRESULTS("TestRun_Size",1, 1 == SchedSize(new_sched));
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval2, MyTaskFunction2, &action_func_param);
	PRINTTESTRESULTS("TestRun_Size",2, 2 == SchedSize(new_sched));
	
	PRINTTESTRESULTS("TestSchedulerAdd_Size",3, 0 == SchedRun(new_sched));

	printf("Destroy scheduler\n");
	SchedDestroy(new_sched);
	
	printf("\n\n");
}

void TestStop()
{
	time_t interval1 = 3;
	time_t interval2 = 1;
	time_t interval3 = 10;	
	size_t action_func_param = 50;
	
	scheduler_t *new_sched = SchedCreate();
	printf("Create scheduler\n");
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval1, MyTaskFunction1, &action_func_param);
	PRINTTESTRESULTS("TestStop_Size",1, 1 == SchedSize(new_sched));
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval2, MyTaskFunction2, &action_func_param);
	PRINTTESTRESULTS("TestStop_Size",2, 2 == SchedSize(new_sched));
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval3, MyTaskFunction3, new_sched);
	PRINTTESTRESULTS("TestStop_Size",3, 3 == SchedSize(new_sched));
	
	PRINTTESTRESULTS("TestStop_run",4, 0 == SchedRun(new_sched));

	printf("Destroy scheduler\n");
	SchedDestroy(new_sched);
	
	printf("\n\n");
}

void TestWithFiles()
{
	time_t interval1 = 1;
	size_t action_func_param = 100;
	
	scheduler_t *new_sched = SchedCreate();
	printf("Create scheduler\n");
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval1, MyTaskFunction4, &action_func_param);
	PRINTTESTRESULTS("TestWithFiles_Size",1, 1 == SchedSize(new_sched));
	
	PRINTTESTRESULTS("TestWithFiles_run",4, 0 == SchedRun(new_sched));

	printf("Destroy scheduler\n");
	SchedDestroy(new_sched);
	
	printf("\n\n");
}

int MyTaskFunction1(void *action_func_param)
{
	static size_t counter = 0;

	if (*(size_t *)action_func_param == counter)
	{
		printf("My very first task no reaccurance = %ld\n", counter);
		return 0;
	}
	else
	{
		printf("My very first task with reaccurance = %ld\n", counter);
		++counter;
		
		return 1;
	}
}

int MyTaskFunction2(void *action_func_param)
{
	static size_t counter = 0;
	
	if (*(size_t *)action_func_param == counter)
	{
		printf("My second task no reaccurance = %ld\n", counter);
		return 0;
	}
	else
	{
		printf("My second task with reaccurance = %ld\n", counter);
		++counter;
		return 1;
	}
}

int MyTaskFunction3(void *action_func_param)
{
	printf("Stop\n");
	
	SchedStop((scheduler_t *)action_func_param);
	
	return 1;
}

int MyTaskFunction4(void *action_func_param)
{
	printf("check for file\n");
	
	if (0 == access("testFile.txt", F_OK))
	{
		printf("stop\n");
		SchedStop((scheduler_t *)action_func_param);
	}
	
	return 1;
}
