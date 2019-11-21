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
void TestStopWithFile();
void TestSizeWithFile();
void TestRunWithClearTask();
void TestIsEmptyWithFile();
void TestClearWithFile();
void TestRemoveWithFile();

int MyTaskFunction1(void *action_func_param);
int MyTaskFunction2(void *action_func_param);
int MyTaskFunctionStop(void *action_func_param);
int MyTaskFunctionFileStop(void *action_func_param);
int MyTaskFunctionSize(void *action_func_param);
int MyTaskFunctionCreateFile(void *action_func_param);
int MyTaskFunctionClearNoFile(void *action_func_param);
int MyTaskFunctionremoveFile(void *action_func_param);
int MyTaskFunctionIsEmpty(void *action_func_param);
int MyTaskFunctionClear(void *action_func_param);
int MyTaskFunctionFileRemove(void *action_func_param);
int MyTaskFunctionCreateFile_forRemoveTests(void *action_func_param);
int MyTaskFunctionremoveFile_ForRemoveTests(void *action_func_param);

size_t counter = 0;

typedef struct struct_param 
{
	size_t action_param;
	char *file_name;
	scheduler_t *sched;
} param;

typedef struct remove_param 
{
	ilrd_uid_t uid;
	char *file_name;
	scheduler_t *sched;
} r_param;

int main()
{	
/*	TestSchedulerCreate();
	TestSchedulerAddRemove();
	TestClear();	
	TestRun();
	TestStop();
	TestStopWithFile();
	TestSizeWithFile();	
	TestRunWithClearTask();
	TestIsEmptyWithFile();
	TestClearWithFile();
*/	TestRemoveWithFile();


	
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

	PRINTTESTRESULTS("TestSchedulerAddRemove_Size",1, 0 == SchedSize(new_sched));
	printf("Add task");
	uid = SchedAdd(new_sched, interval, MyTaskFunction1, &action_func_param);
	PRINTTESTRESULTS("TestSchedulerAddRemove_Size",2, 1 == SchedSize(new_sched));	
	PRINTTESTRESULTS("TestSchedulerAddRemove_Remove",2, 
					  0 == SchedRemove(new_sched, uid));
	PRINTTESTRESULTS("TestSchedulerAddRemove_Size",3, 0 == SchedSize(new_sched));

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
	
	PRINTTESTRESULTS("TestRun_Run",3, 2 == SchedRun(new_sched));
	
	printf("Clear Scheduler\n");
	SchedClear(new_sched);
	
	counter = 0;
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval1, MyTaskFunction1, &action_func_param);
	PRINTTESTRESULTS("TestRun_Size",4, 1 == SchedSize(new_sched));
	printf("size = %ld\n", SchedSize(new_sched));
	printf("Add Task\n");
	SchedAdd(new_sched, interval2, MyTaskFunction2, &action_func_param);
	PRINTTESTRESULTS("TestRun_Size",5, 2 == SchedSize(new_sched));
	PRINTTESTRESULTS("TestSchedulerAdd_Run",6, 2 == SchedRun(new_sched));

	printf("Destroy scheduler\n");
	SchedDestroy(new_sched);
	
	printf("\n\n");
}

void TestRunWithClearTask()
{
	time_t interval1 = 3;
	time_t interval2 = 1;
	time_t interval3 = 10;
	size_t action_func_param = 10;
	
	scheduler_t *new_sched = SchedCreate();
	printf("Create scheduler\n");
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval1, MyTaskFunction1, &action_func_param);
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval2, MyTaskFunction2, &action_func_param);
	
	printf("Add clear Task\n");
	SchedAdd(new_sched, interval3, MyTaskFunctionClearNoFile, new_sched);
	
	PRINTTESTRESULTS("TestRunWithClearTask_Run",3, 2 == SchedRun(new_sched));
	
	counter = 0;
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval1, MyTaskFunction1, &action_func_param);
	PRINTTESTRESULTS("TestRunWithClearTask_Size",4, 1 == SchedSize(new_sched));
	printf("size = %ld\n", SchedSize(new_sched));
	printf("Add Task\n");
	SchedAdd(new_sched, interval2, MyTaskFunction2, &action_func_param);
	PRINTTESTRESULTS("TestRunWithClearTask_Size",5, 2 == SchedSize(new_sched));
	
	PRINTTESTRESULTS("TestRunWithClearTask_Run",6, 2 == SchedRun(new_sched));

	printf("Destroy scheduler\n");
	SchedDestroy(new_sched);
	
	printf("\n\n");
}

void TestClearWithFile()
{
	time_t interval1 = 3;
	time_t interval2 = 1;
	time_t interval3 = 2;
	time_t interval4 = 10;
	time_t interval5 = 15;
	time_t interval6 = 20;
	size_t action_func_param = 50;
	scheduler_t *new_sched = SchedCreate();
	param par = {10, "size.txt", NULL};
	par.sched = new_sched;
	printf("Create scheduler\n");	

	counter = 0;
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval1, MyTaskFunction1, &action_func_param);
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval2, MyTaskFunction2, &action_func_param);
	
	printf("Add clear task to function if file is found\n");
	SchedAdd(new_sched, interval3, MyTaskFunctionClear, new_sched);
	
	printf("Add Task of creating file\n");
	SchedAdd(new_sched, interval4, MyTaskFunctionCreateFile, &par);
	
	printf("Add Task of removing file\n");
	SchedAdd(new_sched, interval5, MyTaskFunctionremoveFile, &par);
	
	printf("Add Task of stop\n");
	SchedAdd(new_sched, interval6, MyTaskFunctionStop, new_sched);
	
	PRINTTESTRESULTS("TestClearWithFile_Run",0, 2 == SchedRun(new_sched));
	
	counter = 0;
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval1, MyTaskFunction1, &action_func_param);
	
	printf("Add Task of removing file\n");
	SchedAdd(new_sched, interval5, MyTaskFunctionremoveFile, &par);
	
	PRINTTESTRESULTS("TestClearWithFile_Run",2, 0 == SchedRun(new_sched));

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
	
	counter = 0;
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval1, MyTaskFunction1, &action_func_param);
	PRINTTESTRESULTS("TestStop_Size",1, 1 == SchedSize(new_sched));
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval2, MyTaskFunction2, &action_func_param);
	PRINTTESTRESULTS("TestStop_Size",2, 2 == SchedSize(new_sched));
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval3, MyTaskFunctionStop, new_sched);
	PRINTTESTRESULTS("TestStop_Size",3, 3 == SchedSize(new_sched));
	
	PRINTTESTRESULTS("TestStop_run",4, 0 == SchedRun(new_sched));

	printf("Destroy scheduler\n");
	SchedDestroy(new_sched);
	
	printf("\n\n");
}

void TestStopWithFile()
{
	/* manually add "testFile.txt" and verufy that it causes run to stop 
		stop
	STOPPED_SUCCESSFULLY
	TesStopWithFile_run
	Destroy scheduler
	*/
	time_t interval1 = 1;
	
	scheduler_t *new_sched = SchedCreate();
	printf("Create scheduler\n");
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval1, MyTaskFunctionFileStop, new_sched);
	PRINTTESTRESULTS("TestStopWithFile_Size",1, 1 == SchedSize(new_sched));
	
	PRINTTESTRESULTS("TesStopWithFile_run",2, 0 == SchedRun(new_sched));

	printf("Destroy scheduler\n");
	SchedDestroy(new_sched);
	
	printf("\n\n");
}

void TestSizeWithFile()
{
	/* test creates a file after 10 sec that tells another tasks 
	to print size. Then another task stops run */ 
	time_t interval1 = 3;
	time_t interval2 = 1;
	time_t interval3 = 2;
	time_t interval4 = 10;
	time_t interval5 = 15;
	time_t interval6 = 20;
	size_t action_func_param = 30;
	
	scheduler_t *new_sched = SchedCreate();
	param par = {10, "size.txt", NULL};
	par.sched = new_sched;
	printf("Create scheduler\n");
	
	counter = 0;
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval1, MyTaskFunction1, &action_func_param);
	PRINTTESTRESULTS("TestSizeWithFile_Size",1, 1 == SchedSize(new_sched));
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval2, MyTaskFunction2, &action_func_param);
	PRINTTESTRESULTS("TestSizeWithFile_Size",2, 2 == SchedSize(new_sched));
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval3, MyTaskFunctionSize, &par);
	PRINTTESTRESULTS("TestSizeWithFile_Size",3, 3 == SchedSize(new_sched));
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval4, MyTaskFunctionCreateFile, &par);
	PRINTTESTRESULTS("TestSizeWithFile_Size",4, 4 == SchedSize(new_sched));
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval5, MyTaskFunctionremoveFile, &par);
	PRINTTESTRESULTS("TestSizeWithFile_Size",5, 5 == SchedSize(new_sched));
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval6, MyTaskFunctionStop, new_sched);
	PRINTTESTRESULTS("TestSizeWithFile_Size",6, 6 == SchedSize(new_sched));
	
	PRINTTESTRESULTS("TestSchedulerAdd_Run",7, 0 == SchedRun(new_sched));
	
	printf("Destroy scheduler\n");
	SchedDestroy(new_sched);
	
	printf("\n\n");
}

void TestRemoveWithFile()
{
	/* test creates a file after 10 sec that tells another tasks
	to print size. Then another task stops run */ 
	time_t interval1 = 3;
	time_t interval2 = 1;
	time_t interval3 = 2;
	time_t interval4 = 10;
	time_t interval5 = 15;
	time_t interval6 = 20;
	size_t action_func_param = 30;
	scheduler_t *new_sched = SchedCreate();
	r_param par = {{0}, "test.txt", NULL};
	
	par.sched = new_sched;
	
	printf("Create scheduler\n");
	
	counter = 0;
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval1, MyTaskFunction1, &action_func_param);
	
	printf("Add Task\n");
	par.uid = SchedAdd(new_sched, interval2, MyTaskFunction2, &action_func_param);
	printf("uid.pid = %d, uid.time = %ld, uid.counter = %ld", par.uid.pid, par.uid.time, par.uid.counter);
	printf("Add Task\n");
	SchedAdd(new_sched, interval3, MyTaskFunctionFileRemove, &par);
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval4, MyTaskFunctionCreateFile_forRemoveTests, &par);
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval5, MyTaskFunctionremoveFile_ForRemoveTests, &par);
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval6, MyTaskFunctionStop, new_sched);
	
	PRINTTESTRESULTS("TestSchedulerAdd_Run",7, 0 == SchedRun(new_sched));
	
	printf("Destroy scheduler\n");
	SchedDestroy(new_sched);
	
	printf("\n\n");
}

void TestIsEmptyWithFile()
{
	/* test creates a file after 10 sec that tells another tasks 
	to print isEmpty and then stops. File is removed automatically */
	time_t interval1 = 5;
	time_t interval2 = 2;
	time_t interval3 = 1;
	time_t interval4 = 15;
	time_t interval5 = 20;
	time_t interval6 = 25;
	size_t action_func_param = 40;
	
	scheduler_t *new_sched = SchedCreate();
	param par = {10, "empty.txt", NULL};
	par.sched = new_sched;
	printf("Create scheduler\n");
	
	counter = 0;
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval1, MyTaskFunction1, &action_func_param);
	PRINTTESTRESULTS("TestIsEmptyWithFile_Size",1, 1 == SchedSize(new_sched));
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval2, MyTaskFunction2, &action_func_param);
	PRINTTESTRESULTS("TestIsEmptyWithFile_Size",2, 2 == SchedSize(new_sched));
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval3, MyTaskFunctionIsEmpty, &par);
	PRINTTESTRESULTS("TestIsEmptyWithFile_Size",3, 3 == SchedSize(new_sched));
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval4, MyTaskFunctionCreateFile, &par);
	PRINTTESTRESULTS("TestIsEmptyWithFile_Size",4, 4 == SchedSize(new_sched));
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval5, MyTaskFunctionremoveFile, &par);
	PRINTTESTRESULTS("TestIsEmptyWithFile_Size",5, 5 == SchedSize(new_sched));
	
	printf("Add Task\n");
	SchedAdd(new_sched, interval6, MyTaskFunctionStop, new_sched);
	PRINTTESTRESULTS("TestIsEmptyWithFile_Size",6, 6 == SchedSize(new_sched));
	
	PRINTTESTRESULTS("TestIsEmptyWithFile_Run",7, 0 == SchedRun(new_sched));
	
	printf("Destroy scheduler\n");
	SchedDestroy(new_sched);
	
	printf("\n\n");
}

int MyTaskFunction1(void *action_func_param)
{
	if (*(size_t *)action_func_param == counter)
	{
		printf("FIRST task no reaccurance = %ld\n", counter);
		return 0;
	}
	else
	{
		printf("FIRST task with reaccurance = %ld\n", counter);
		++counter;
		
		return 1;
	}
}

int MyTaskFunction2(void *action_func_param)
{
	if (*(size_t *)action_func_param == counter)
	{
		printf("SECOND task no reaccurance = %ld\n", counter);
		return 0;
	}
	else
	{
		printf("SECOND task with reaccurance = %ld\n", counter);
		++counter;
		return 1;
	}
}

int MyTaskFunctionStop(void *action_func_param)
{
	printf("Stop\n");
	
	SchedStop((scheduler_t *)action_func_param);
	
	return 1;
}	

int MyTaskFunctionFileStop(void *action_func_param)
{
	printf("check for file\n");

	if (0 == access("testFile.txt", F_OK))
	{
		printf("stop\n");
		SchedStop((scheduler_t *)action_func_param);
	}
	
	return 1;
}

int MyTaskFunctionFileRemove(void *action_func_param)
{
	printf("check for file\n");

	if (0 == access(((r_param *)action_func_param) -> file_name, F_OK))
	{
		printf("remove\n");
		SchedRemove(((r_param *)action_func_param) -> sched, ((r_param *)action_func_param) -> uid);
	}
	
	return 0;
}

int MyTaskFunctionIsEmpty(void *action_func_param)
{
	printf("check for file\n");

	if (0 == access(((param *)action_func_param) -> file_name, F_OK))
	{
		printf("found file and returning IsEmpty = %d\n", 
				SchedIsEmpty(((param *)action_func_param) -> sched));
	}
	
	return 1;
}

int MyTaskFunctionSize(void *action_func_param)
{
	printf("check for file\n");

	if (0 == access(((param *)action_func_param) -> file_name, F_OK))
	{
		printf("found file and returning size = %ld\n", 
				SchedSize(((param *)action_func_param) -> sched));
	}
	
	return 1;
}

int MyTaskFunctionClear(void *action_func_param)
{
	printf("check for file\n");

	if (0 == access(((param *)action_func_param) -> file_name, F_OK))
	{
		printf("found file and doing clear"); 
		SchedClear(((param *)action_func_param) -> sched);
	}
	
	return 1;
}

int MyTaskFunctionCreateFile(void *action_func_param)
{
	FILE *fp = fopen(((param *)action_func_param) -> file_name, "w");
		
	if (fp == NULL)
	{
		printf("Couldn't Create file\n");
	}
	else
	{
		printf("Create file\n");
	}
	
	return 0;
}

int MyTaskFunctionCreateFile_forRemoveTests(void *action_func_param)
{
	FILE *fp = fopen(((r_param *)action_func_param) -> file_name, "w");
		
	if (fp == NULL)
	{
		printf("Couldn't Create file\n");
	}
	else
	{
		printf("Create file\n");
	}
	
	return 0;
}

int MyTaskFunctionremoveFile(void *action_func_param)
{
	remove(((param *)action_func_param) -> file_name);
	printf("Remove file\n");
	
	return 0;
}

int MyTaskFunctionremoveFile_ForRemoveTests(void *action_func_param)
{
	remove(((r_param *)action_func_param) -> file_name);
	printf("Remove file\n");
	
	return 0;
}

int MyTaskFunctionClearNoFile(void *action_func_param)
{
	SchedClear((scheduler_t *)action_func_param);
	
	return 0;
}
