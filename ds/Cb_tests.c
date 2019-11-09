#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "CBuff.h"

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

struct CBuff
{
    size_t capacity;
    char *write_ptr;
    char *read_ptr;
    char begin[1];
};

int main()
{
	size_t num_of_writes = 0;
	size_t num_of_reads = 0;	
	CBuff_t *new_CB = CBuffCreate(20);
	char *buf_to_read_from = "Hello moshe, my name is chen";
	char buf_to_write_on[20];
	num_of_writes = CBuffWrite(new_CB, buf_to_read_from, 27);
	num_of_reads = CBuffRead(new_CB, buf_to_write_on, 27);
	CBuffDestroy(new_CB);
	
	return 0;
}

/*
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
}*/
