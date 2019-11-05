#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include "slist.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define PRINTTESTRESULTS(func,num, res) \
(printf("Function %s: Test %d %-40s\n" KNRM,func, num, (res) == 1 ?\
KGRN "\t\tpassed" : KRED "\t\tfailed"))
#define ELEMENT_SIZE 4 
#define NUM_OF_ELEMENTS 20

static void PrintSList(sl_node_t *head);

void TestsCreateInsertCountDestroy();
void TestsCreateRemoveCountDestroy();
	
int main()
{
	TestsCreateInsertCountDestroy();
	TestsCreateRemoveCountDestroy();			
	
	return 0;
}

static void PrintSList(sl_node_t *head)
{	
	while (NULL != head -> next)
	{
		printf("%d, ", *(int *)(head -> data));
		head = (head -> next);
	}
	
	printf("%d\n", *(int *)(head -> data));
}

void TestsCreateInsertCountDestroy()
{
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;

	sl_node_t *last_node = SListCreateNode(&a4, NULL);
	sl_node_t *head = SListCreateNode(&a1, last_node);
	sl_node_t *insert_after_first_node = SListCreateNode(&a2, NULL);
	sl_node_t *insert_before_last_node = SListCreateNode(&a3, NULL);
	
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_SListCount",1, 
	2 == SListCount(head));
	
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_SListInsert",2,
	(head -> next)  == 
	SListInsert(insert_before_last_node, last_node));
	
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_SListCount",3, 
	3 == SListCount(head));
	
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_SListInsertAfter",4,
	a2 == *(int *)(SListInsertAfter(insert_after_first_node, head)) -> data);
	
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_SListCount",5, 
	4 == SListCount(head));
	
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_headData",6,
	a1 == *(int *)(head -> data));
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_secondData",7,
	a2 == *(int *)((head -> next) -> data));
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_thirdData",8,
	a3 == *(int *)(((head -> next) -> next) -> data));
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_lastData",9,
	a4 == *(int *)((((head -> next) -> next) -> next) -> data));
	
	PrintSList(head);
	
	SListFreeAll(head);
}

void TestsCreateRemoveCountDestroy()
{
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;
	
	sl_node_t *temp_node = NULL;
	sl_node_t *last_node = SListCreateNode(&a4, NULL);
	sl_node_t *head = SListCreateNode(&a1, last_node);
	sl_node_t *insert_after_first_node = SListCreateNode(&a2, NULL);
	sl_node_t *insert_before_last_node = SListCreateNode(&a3, NULL);
	SListInsert(insert_before_last_node, last_node);
	SListInsertAfter(insert_after_first_node, head);
	
	PrintSList(head);
	
	PRINTTESTRESULTS("TestsCreateRemoveCountDestroy_SListRemove",1, 
	a3 == *(int *)(SListRemove(insert_before_last_node) -> data));
	
	PRINTTESTRESULTS("TestsCreateRemoveCountDestroy_SListCount",2, 
	3 == SListCount(head));
	
	temp_node = SListRemoveAfter(head);
	PRINTTESTRESULTS("TestsCreateRemoveCountDestroy_SListRemoveAfter",3, 
	a2 == *(int *)(temp_node -> data));
	free(temp_node);
	
	PRINTTESTRESULTS("TestsCreateRemoveCountDestroy_SListCount",4, 
	2 == SListCount(head));
	
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_headData",5,
	a1 == *(int *)(head -> data));
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_secondData",6,
	a4 == *(int *)((head -> next) -> data));
	
	PrintSList(head);
	
	SListFreeAll(head);
}
