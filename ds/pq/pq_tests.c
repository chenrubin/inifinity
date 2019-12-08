#include <stdio.h> /* printf */

#include "pq.h"
#include "../dllist/dllist.h"
#include "../sortedlist/sortedlist.h"
#include "../../chen/MyUtils.h"

void TestCreate();
void TestEnqueuePeek();
void TestDequeue();
void TestClear();
void TestErase();
void TestBetterEnqueuePeek();
int MyCompareFunc(const void *new_data, const void *src_data, void *param);
int MyBetterCompareFunc(const void *new_data, const void *src_data, void *param);
int IsMatch(const void *data1, const void *data2);

int main()
{
	TestCreate();
	TestEnqueuePeek();
	TestDequeue();
	TestClear();
	TestErase();
	TestBetterEnqueuePeek();			
	

	return 0;
}

void TestCreate()
{
	int priority_param = 0;
	p_queue_t *new_pq = PQCreate(&priority_param, MyCompareFunc);
	
	printf("\n\nCreate priority queue\n");
	printf("Destroy priority queue\n\n");	
	PQDestroy(new_pq);
}

void TestEnqueuePeek()
{
	int n1 = 2;
	int n2 = 4;
	int n3 = 10;
	int n4 = 1;
	int n5 = 2;
	int n6 = 5;
	int priority_param = 0;
	p_queue_t *new_pq = PQCreate(&priority_param, MyCompareFunc);
	printf("Create priority queue\n");
	
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",1, 1 == PQIsEmpty(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",2, 0 == PQEnqueue(new_pq, &n1));
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",3, 0 == PQIsEmpty(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",4, 2 == *(int *)PQPeek(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",5, 0 == PQEnqueue(new_pq, &n2));
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",6, 0 == PQIsEmpty(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",7, 4 == *(int *)PQPeek(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",8, 0 == PQEnqueue(new_pq, &n3));
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",9, 0 == PQIsEmpty(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",10, 10 == *(int *)PQPeek(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",11, 0 == PQEnqueue(new_pq, &n4));
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",12, 0 == PQIsEmpty(new_pq));	
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",13, 10 == *(int *)PQPeek(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",14, 0 == PQEnqueue(new_pq, &n5));
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",15, 0 == PQIsEmpty(new_pq));	
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",16, 10 == *(int *)PQPeek(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",17, 0 == PQEnqueue(new_pq, &n6));
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",18, 0 == PQIsEmpty(new_pq));	
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",19, 10 == *(int *)PQPeek(new_pq));
	
	
	printf("Destroy priority queue\n\n");
	PQDestroy(new_pq);
}

void TestBetterEnqueuePeek()
{
	int n1 = 2;
	int n2 = 12;
	int n3 = 13;
	int n4 = 11;
	int n5 = 27;
	int n6 = 17;
	int priority_param = 0;
	p_queue_t *new_pq = PQCreate(&priority_param, MyBetterCompareFunc);
	printf("Create priority queue\n");
	
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",1, 1 == PQIsEmpty(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",2, 0 == PQEnqueue(new_pq, &n1));
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",3, 0 == PQIsEmpty(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",4, 2 == *(int *)PQPeek(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",12, 0 == PQEnqueue(new_pq, &n2));
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",6, 0 == PQIsEmpty(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",7, 2 == *(int *)PQPeek(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",13, 0 == PQEnqueue(new_pq, &n3));
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",9, 0 == PQIsEmpty(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",10, 13 == *(int *)PQPeek(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",11, 0 == PQEnqueue(new_pq, &n4));
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",12, 0 == PQIsEmpty(new_pq));	
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",13, 13 == *(int *)PQPeek(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",27, 0 == PQEnqueue(new_pq, &n5));
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",15, 0 == PQIsEmpty(new_pq));	
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",16, 27 == *(int *)PQPeek(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",17, 0 == PQEnqueue(new_pq, &n6));
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",18, 0 == PQIsEmpty(new_pq));	
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",19, 27 == *(int *)PQPeek(new_pq));
	
	
	printf("Destroy priority queue\n\n");
	PQDestroy(new_pq);
}

void TestDequeue()
{
	int n1 = 2;
	int n2 = 4;
	int n3 = 10;
	int n4 = 1;
	int n5 = 2;
	int n6 = 5;
	int priority_param = 0;
	p_queue_t *new_pq = PQCreate(&priority_param, MyCompareFunc);
	printf("Create priority queue\n");
	
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",1, 0 == PQEnqueue(new_pq, &n1));
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",2, 0 == PQEnqueue(new_pq, &n2));
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",3, 0 == PQEnqueue(new_pq, &n3));
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",4, 0 == PQEnqueue(new_pq, &n4));	
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",5, 0 == PQEnqueue(new_pq, &n5));	
	PRINTTESTRESULTS("TestEnqueuePeek_Enqueue",6, 0 == PQEnqueue(new_pq, &n6));
	
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",7, 10 == *(int *)PQPeek(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Size",8, 6 == PQSize(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Dequeue",9, 10 == *(int *)PQDequeue(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",10, 0 == PQIsEmpty(new_pq));
	
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",11, 5 == *(int *)PQPeek(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Size",12, 5 == PQSize(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Dequeue",13, 5 == *(int *)PQDequeue(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",14, 0 == PQIsEmpty(new_pq));
	
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",15, 4 == *(int *)PQPeek(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Size",16, 4 == PQSize(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Dequeue",17, 4 == *(int *)PQDequeue(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",18, 0 == PQIsEmpty(new_pq));
	
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",19, 2 == *(int *)PQPeek(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Size",20, 3 == PQSize(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Dequeue",21, 2 == *(int *)PQDequeue(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",22, 0 == PQIsEmpty(new_pq));
	
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",23, 2 == *(int *)PQPeek(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Size",24, 2 == PQSize(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Dequeue",25, 2 == *(int *)PQDequeue(new_pq));	
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",26, 0 == PQIsEmpty(new_pq));
	
	PRINTTESTRESULTS("TestEnqueuePeek_Peek",27, 1 == *(int *)PQPeek(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Size",28, 1 == PQSize(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Dequeue",29, 1 == *(int *)PQDequeue(new_pq));
	PRINTTESTRESULTS("TestEnqueuePeek_Size",30, 0 == PQSize(new_pq));	
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",31, 1 == PQIsEmpty(new_pq));
	
	printf("Destroy priority queue\n\n");
	PQDestroy(new_pq);
}

void TestClear()
{
	int n1 = 2;
	int n2 = 4;
	int n3 = 10;
	int n4 = 1;
	int n5 = 2;
	int n6 = 5;
	int priority_param = 0;
	p_queue_t *new_pq = PQCreate(&priority_param, MyCompareFunc);
	printf("Create priority queue\n");
	
	PRINTTESTRESULTS("TestClear_Enqueue",1, 0 == PQEnqueue(new_pq, &n1));
	PRINTTESTRESULTS("TestClear_Enqueue",2, 0 == PQEnqueue(new_pq, &n2));
	PRINTTESTRESULTS("TestClear_Enqueue",3, 0 == PQEnqueue(new_pq, &n3));
	PRINTTESTRESULTS("TestClear_Enqueue",4, 0 == PQEnqueue(new_pq, &n4));	
	PRINTTESTRESULTS("TestClear_Enqueue",5, 0 == PQEnqueue(new_pq, &n5));	
	PRINTTESTRESULTS("TestClear_Enqueue",6, 0 == PQEnqueue(new_pq, &n6));
	PRINTTESTRESULTS("TestClear_Isempty",7, 0 == PQIsEmpty(new_pq));
	PQClear(new_pq);
	printf("Clear list\n");
	PRINTTESTRESULTS("TestEnqueuePeek_Isempty",9, 1 == PQIsEmpty(new_pq));
	
	
	
	printf("Destroy priority queue\n\n");
	PQDestroy(new_pq);
}

void TestErase()
{
	int n1 = 2;
	int n2 = 4;
	int n3 = 10;
	int n4 = 1;
	int n5 = 2;
	int n6 = 5;
	int data_to_remove = 4;
	int priority_param = 0;
	p_queue_t *new_pq = PQCreate(&priority_param, MyCompareFunc);
	printf("Create priority queue\n");
	
	PRINTTESTRESULTS("TestErase_Enqueue",1, 0 == PQEnqueue(new_pq, &n1));
	PRINTTESTRESULTS("TestErase_Enqueue",2, 0 == PQEnqueue(new_pq, &n2));
	PRINTTESTRESULTS("TestErase_Enqueue",3, 0 == PQEnqueue(new_pq, &n3));
	PRINTTESTRESULTS("TestErase_Enqueue",4, 0 == PQEnqueue(new_pq, &n4));	
	PRINTTESTRESULTS("TestErase_Enqueue",5, 0 == PQEnqueue(new_pq, &n5));	
	PRINTTESTRESULTS("TestErase_Enqueue",6, 0 == PQEnqueue(new_pq, &n6));
	PRINTTESTRESULTS("TestErase_Size",7, 6 == PQSize(new_pq));
	PRINTTESTRESULTS("TestErase_Erase",8, 4 == *(int *)PQErase(&data_to_remove, new_pq, IsMatch));
	PRINTTESTRESULTS("TestErase_Size",9, 5 == PQSize(new_pq));
	
	PRINTTESTRESULTS("TestErase_Dequeue",10, 10 == *(int *)PQDequeue(new_pq));
	PRINTTESTRESULTS("TestErase_Dequeue",11, 5 == *(int *)PQDequeue(new_pq));
	PRINTTESTRESULTS("TestErase_Dequeue",12, 2 == *(int *)PQDequeue(new_pq));
	PRINTTESTRESULTS("TestErase_Dequeue",13, 2 == *(int *)PQDequeue(new_pq));
	PRINTTESTRESULTS("TestErase_Dequeue",14, 1 == *(int *)PQDequeue(new_pq));

	
	printf("Destroy priority queue\n\n");
	PQDestroy(new_pq);

}

int MyCompareFunc(const void *new_data, const void *src_data, void *param)
{
	(void)param;
	
	if (*(int *)new_data < *(int *)src_data)
	{
		return -1;
	}
	else if (*(int *)new_data == *(int *)src_data)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int MyBetterCompareFunc(const void *new_data, const void *src_data, void *param)
{
	(void)param;
	
	if ((*(int *)new_data % 10) < (*(int *)src_data % 10))
	{
		return -1;
	}
	else if ((*(int *)new_data % 10) == (*(int *)src_data % 10))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int IsMatch(const void *data1, const void *data2)
{
	return (*(int *)data1 == *(int *)data2);
}
