#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include "../slist/slist.h"
#include "queue.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define PRINTTESTRESULTS(func,num, res) \
(printf("%-55s: Test %d %s\n" KNRM,func, num, (res) == 1 ?\
KGRN "\t\tpassed" : KRED "\t\tfailed"))

void TestsCreateDestroyQueue();
void TestsEnqueuePeek();
void TestsQDequeue();
void TestsIsEmpty();
void TestsSize();
void TestsAppend();
void TestsCrazyTest();

int main()
{
	TestsCreateDestroyQueue();
	TestsEnqueuePeek();
	TestsQDequeue();
	TestsIsEmpty();
	TestsSize();
	TestsAppend();
	TestsCrazyTest();
	
	return 0;
}

void TestsCreateDestroyQueue()
{
	queue_t *new_queue = QCreate();
	printf("Creating queue\n"); 
	QDestroy(new_queue);
	printf("Destroying queue\nValgrind should be tested manually\n\n");	
}

void TestsEnqueuePeek()
{
	queue_t *new_queue = QCreate();
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;
	
	printf("Create queue\n");
	
	PRINTTESTRESULTS("TestsEnqueuePeek_Enqueue",1, 
	0 == QEnqueue(new_queue, &a1));
	PRINTTESTRESULTS("TestsEnqueuePeek_Peek",2, 
	a1 == *(int *)QPeek(new_queue));
	PRINTTESTRESULTS("TestsEnqueuePeek_Enqueue",3, 
	0 == QEnqueue(new_queue, &a2));
	PRINTTESTRESULTS("TestsEnqueuePeek_Peek",4, 
	a1 == *(int *)QPeek(new_queue));
	PRINTTESTRESULTS("TestsEnqueuePeek_Enqueue",5, 
	0 == QEnqueue(new_queue, &a3));
	PRINTTESTRESULTS("TestsEnqueuePeek_Peek",6, 
	a1 == *(int *)QPeek(new_queue));
	PRINTTESTRESULTS("TestsEnqueuePeek_Enqueue",7, 
	0 == QEnqueue(new_queue, &a4));
	PRINTTESTRESULTS("TestsEnqueuePeek_Peek",8, 
	a1 == *(int *)QPeek(new_queue));
	
	printf("Destroy queue\n"); 
	QDestroy(new_queue);
		
	printf("\n\n");
}

void TestsQDequeue()
{
	queue_t *new_queue = QCreate();
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;
	
	printf("Create queue\n");
	
	PRINTTESTRESULTS("TestsQDequeue_Enqueue",1, 
	0 == QEnqueue(new_queue, &a1));
	PRINTTESTRESULTS("TestsQDequeue_Enqueue",2, 
	0 == QEnqueue(new_queue, &a2));
	PRINTTESTRESULTS("TestsQDequeue_Enqueue",3, 
	0 == QEnqueue(new_queue, &a3));
	PRINTTESTRESULTS("TestsQDequeue_Enqueue",4, 
	0 == QEnqueue(new_queue, &a4));
	PRINTTESTRESULTS("TestsQDequeue_Peek",5, 
	a1 == *(int *)QPeek(new_queue));
	printf("Dequeue\n");
	QDequeue(new_queue);
	PRINTTESTRESULTS("TestsQDequeue_Peek",6,
	a2 == *(int *)QPeek(new_queue));
	printf("Dequeue\n");
	QDequeue(new_queue);
	PRINTTESTRESULTS("TestsQDequeue_Peek",7, 
	a3 == *(int *)QPeek(new_queue));
	printf("Dequeue\n");
	QDequeue(new_queue);
	PRINTTESTRESULTS("TestsQDequeue_Peek",8, 
	a4 == *(int *)QPeek(new_queue));
	printf("Dequeue\n");
	QDequeue(new_queue);
	PRINTTESTRESULTS("TestsQDequeue_Enqueue",9, 
	0 == QEnqueue(new_queue, &a1));
	
	printf("Destroy queue\n"); 
	QDestroy(new_queue);
		
	printf("\n\n");
}

void TestsIsEmpty()
{
	queue_t *new_queue = QCreate();
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;
	
	printf("Create queue\n");
	
	PRINTTESTRESULTS("TestsIsEmpty_IsEmpty",1, 
	1 == QIsEmpty(new_queue));
	PRINTTESTRESULTS("TestsIsEmpty_Enqueue",2,
	0 == QEnqueue(new_queue, &a1));
	PRINTTESTRESULTS("TestsIsEmpty_IsEmpty",3, 
	0 == QIsEmpty(new_queue));
	PRINTTESTRESULTS("TestsIsEmpty_Enqueue",4, 
	0 == QEnqueue(new_queue, &a2));
	PRINTTESTRESULTS("TestsIsEmpty_IsEmpty",5, 
	0 == QIsEmpty(new_queue));
	PRINTTESTRESULTS("TestsIsEmpty_Enqueue",6, 
	0 == QEnqueue(new_queue, &a3));
	PRINTTESTRESULTS("TestsIsEmpty_IsEmpty",7, 
	0 == QIsEmpty(new_queue));
	PRINTTESTRESULTS("TestsIsEmpty_Enqueue",8, 
	0 == QEnqueue(new_queue, &a4));
	PRINTTESTRESULTS("TestsIsEmpty_IsEmpty",9,
	0 == QIsEmpty(new_queue));
	printf("Dequeue\n");
	QDequeue(new_queue);
	PRINTTESTRESULTS("TestsIsEmpty_IsEmpty",10, 
	0 == QIsEmpty(new_queue));
	printf("Dequeue\n");
	QDequeue(new_queue);
	PRINTTESTRESULTS("TestsIsEmpty_IsEmpty",11, 
	0 == QIsEmpty(new_queue));
	printf("Dequeue\n");
	QDequeue(new_queue);
	PRINTTESTRESULTS("TestsIsEmpty_IsEmpty",12, 
	0 == QIsEmpty(new_queue));
	printf("Dequeue\n");
	QDequeue(new_queue);
	PRINTTESTRESULTS("TestsIsEmpty_IsEmpty",13, 
	1 == QIsEmpty(new_queue));
	
	printf("Destroy queue\n"); 
	QDestroy(new_queue);
		
	printf("\n\n");
}

void TestsSize()
{
	queue_t *new_queue = QCreate();
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;
	
	printf("Create queue\n");
	
	PRINTTESTRESULTS("TestsSize_Size",1, 
	0 == QSize(new_queue));
	PRINTTESTRESULTS("TestsSize_Enqueue",2,
	0 == QEnqueue(new_queue, &a1));
	PRINTTESTRESULTS("TestsSize_Size",3, 
	1 == QSize(new_queue));
	PRINTTESTRESULTS("TestsSize_Enqueue",4, 
	0 == QEnqueue(new_queue, &a2));
	PRINTTESTRESULTS("TestsSize_Size",5, 
	2 == QSize(new_queue));
	PRINTTESTRESULTS("TestsSize_Enqueue",6, 
	0 == QEnqueue(new_queue, &a3));
	PRINTTESTRESULTS("TestsSize_Size",7, 
	3 == QSize(new_queue));
	PRINTTESTRESULTS("TestsSize_Enqueue",8, 
	0 == QEnqueue(new_queue, &a4));
	PRINTTESTRESULTS("TestsSize_Size",9, 
	4 == QSize(new_queue));
	printf("Dequeue\n");
	QDequeue(new_queue);
	PRINTTESTRESULTS("TestsSize_Size",10, 
	3 == QSize(new_queue));
	printf("Dequeue\n");
	QDequeue(new_queue);
	PRINTTESTRESULTS("TestsSize_Size",11, 
	2 == QSize(new_queue));
	printf("Dequeue\n");
	QDequeue(new_queue);
	PRINTTESTRESULTS("TestsSize_Size",12, 
	1 == QSize(new_queue));
	printf("Dequeue\n");
	QDequeue(new_queue);
	PRINTTESTRESULTS("TestsSize_Size",13, 
	0 == QSize(new_queue));
	
	printf("Destroy queue\n"); 
	QDestroy(new_queue);
		
	printf("\n\n");
}

void TestsAppend()
{
	queue_t *new_queue_dst = QCreate();
	queue_t *new_queue_src = QCreate();
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;
	int a5 = 5;
	int a6 = 6;
	int a7 = 7;
	int a8 = 8;
	printf("Create queue dest\n");
	printf("Create queue src\n");
	
	PRINTTESTRESULTS("TestsAppend_EnqueueDest",1,
	0 == QEnqueue(new_queue_dst, &a1));
	PRINTTESTRESULTS("TestsAppend_EnqueueDest",2,
	0 == QEnqueue(new_queue_dst, &a2));
	PRINTTESTRESULTS("TestsAppend_EnqueueDest",3,
	0 == QEnqueue(new_queue_dst, &a3));
	PRINTTESTRESULTS("TestsAppend_EnqueueDest",4,
	0 == QEnqueue(new_queue_dst, &a4));
	PRINTTESTRESULTS("TestsAppend_Enqueuesrc",5,
	0 == QEnqueue(new_queue_src, &a5));
	PRINTTESTRESULTS("TestsAppend_Enqueuesrc",6,
	0 == QEnqueue(new_queue_src, &a6));
	PRINTTESTRESULTS("TestsAppend_Enqueuesrc",7,
	0 == QEnqueue(new_queue_src, &a7));
	PRINTTESTRESULTS("TestsAppend_Enqueuesrc",8,
	0 == QEnqueue(new_queue_src, &a8));
	PRINTTESTRESULTS("TestsAppend_Size",9,
	4 == QSize(new_queue_dst));
	PRINTTESTRESULTS("TestsAppend_Size",10, 
	4 == QSize(new_queue_src));
	printf("Append dst to src\n");
	QAppend(new_queue_dst, new_queue_src);
	PRINTTESTRESULTS("TestsAppend_Size",11,
	8 == QSize(new_queue_dst));
	
	printf("Destroy queue\n");
	QDestroy(new_queue_dst);
	
	printf("\n\n");
}

void TestsCrazyTest()
{
	queue_t *new_queue_dst = QCreate();
	queue_t *new_queue_src = QCreate();
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;
	int a5 = 5;
	int a6 = 6;
	int a7 = 7;
	int a8 = 8;
	int a9 = 9;
	int a10 = 10;
	printf("Create queue dest\n");
	printf("Create queue src\n");
	
	PRINTTESTRESULTS("TestsCrazyTest_EnqueueDest",1,
	0 == QEnqueue(new_queue_dst, &a1));
	PRINTTESTRESULTS("TestsCrazyTest_EnqueueDest",2,
	0 == QEnqueue(new_queue_dst, &a2));
	PRINTTESTRESULTS("TestsCrazyTest_EnqueueDest",3,
	0 == QEnqueue(new_queue_dst, &a3));
	PRINTTESTRESULTS("TestsCrazyTest_EnqueueDest",4,
	0 == QEnqueue(new_queue_dst, &a4));
	PRINTTESTRESULTS("TestsCrazyTest_Enqueuesrc",5,
	0 == QEnqueue(new_queue_src, &a5));
	PRINTTESTRESULTS("TestsCrazyTest_Enqueuesrc",6,
	0 == QEnqueue(new_queue_src, &a6));
	PRINTTESTRESULTS("TestsCrazyTest_Enqueuesrc",7,
	0 == QEnqueue(new_queue_src, &a7));
	PRINTTESTRESULTS("TestsCrazyTest_Enqueuesrc",8,
	0 == QEnqueue(new_queue_src, &a8));
	PRINTTESTRESULTS("TestsCrazyTest_Size",9,
	4 == QSize(new_queue_dst));
	PRINTTESTRESULTS("TestsCrazyTest_Size",10, 
	4 == QSize(new_queue_src));
	printf("Append dst to src\n");
	QAppend(new_queue_dst, new_queue_src);
	PRINTTESTRESULTS("TestsCrazyTest_Size",11, 
	8 == QSize(new_queue_dst));
	PRINTTESTRESULTS("TestsCrazyTest_Enqueuedst",12,
	0 == QEnqueue(new_queue_dst, &a9));
	PRINTTESTRESULTS("TestsCrazyTest_Size",13,
	9 == QSize(new_queue_dst));
	PRINTTESTRESULTS("TestsCrazyTest_Enqueuedst",14,
	0 == QEnqueue(new_queue_dst, &a10));
	PRINTTESTRESULTS("TestsCrazyTest_Size",15,
	10 == QSize(new_queue_dst));
	PRINTTESTRESULTS("TestsQDequeue_Peek",16,
	a1 == *(int *)QPeek(new_queue_dst));
	
	printf("Dequeue\n");
	QDequeue(new_queue_dst);
	PRINTTESTRESULTS("TestsCrazyTest_Peek",17,
	a2 == *(int *)QPeek(new_queue_dst));
	PRINTTESTRESULTS("TestsCrazyTest_Size",18,
	9 == QSize(new_queue_dst));
	printf("Dequeue\n");
	QDequeue(new_queue_dst);
	PRINTTESTRESULTS("TestsCrazyTest_Peek",19,
	a3 == *(int *)QPeek(new_queue_dst));
	PRINTTESTRESULTS("TestsCrazyTest_Size",20,
	8 == QSize(new_queue_dst));
	
	printf("Dequeue\n");
	QDequeue(new_queue_dst);
	PRINTTESTRESULTS("TestsCrazyTest_Peek",21,
	a4 == *(int *)QPeek(new_queue_dst));
	PRINTTESTRESULTS("TestsCrazyTest_Size",22,
	7 == QSize(new_queue_dst));
	
	printf("Dequeue\n");
	QDequeue(new_queue_dst);
	PRINTTESTRESULTS("TestsCrazyTest_Peek",23,
	a5 == *(int *)QPeek(new_queue_dst));
	PRINTTESTRESULTS("TestsCrazyTest_Size",24,
	6 == QSize(new_queue_dst));
	
	printf("Dequeue\n");
	QDequeue(new_queue_dst);
	PRINTTESTRESULTS("TestsCrazyTest_Peek",25,
	a6 == *(int *)QPeek(new_queue_dst));
	PRINTTESTRESULTS("TestsCrazyTest_Size",26,
	5 == QSize(new_queue_dst));
	
	printf("Dequeue\n");
	QDequeue(new_queue_dst);
	PRINTTESTRESULTS("TestsCrazyTest_Peek",27,
	a7 == *(int *)QPeek(new_queue_dst));
	PRINTTESTRESULTS("TestsCrazyTest_Size",28,
	4 == QSize(new_queue_dst));
	
	printf("Dequeue\n");
	QDequeue(new_queue_dst);
	PRINTTESTRESULTS("TestsCrazyTest_Peek",29,
	a8 == *(int *)QPeek(new_queue_dst));
	PRINTTESTRESULTS("TestsCrazyTest_Size",30,
	3 == QSize(new_queue_dst));
	
	printf("Dequeue\n");
	QDequeue(new_queue_dst);
	PRINTTESTRESULTS("TestsCrazyTest_Peek",31,
	a9 == *(int *)QPeek(new_queue_dst));
	PRINTTESTRESULTS("TestsCrazyTest_Size",32,
	2 == QSize(new_queue_dst));
	
	printf("Dequeue\n");
	QDequeue(new_queue_dst);
	PRINTTESTRESULTS("TestsCrazyTest_Peek",33,
	a10 == *(int *)QPeek(new_queue_dst));
	PRINTTESTRESULTS("TestsCrazyTest_Size",34,
	1 == QSize(new_queue_dst));
	
	printf("Destroy queue\n"); 
	QDestroy(new_queue_dst);
}
