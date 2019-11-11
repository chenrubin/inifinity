#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "dllist.h"
#include "MyUtils.h" /* MAX2,MIN2 */

void TestCreateDestroy();
void TestPushFront();

int main()
{
	TestCreateDestroy();
	TestPushFront();

	
	
	return 0;
}

void TestCreateDestroy()
{	
	dl_list_t *new_list = DLListCreate();
	printf("Create Doubely linked list\n");
	DLListDestroy(new_list);
	printf("Destroy Doubely linked list\n");
	
	printf("NOW CHECK VALGRIND FOR NO MEMORY LEAKS\n\n");
}

void TestPushFront()
{
	int num[] = {13,12,11,10};
	dll_node_t *node = NULL;
	int i = 0;
	
	dl_list_t *new_list = DLListCreate();
	printf("Create Doubely linked list\n");
	
	for (i = 0; i < 4; ++i)
	{
		PRINTTESTRESULTS("TestPushFront_pushFront",i + 1,
		num[i] == *(int *)DLListGetData(DLListPushFront(new_list, &num[i])));
	}
	
	i = 0;			
	for (node = DLListBegin(new_list); node != DLListEnd(new_list); 
									   node = DLListNext(node))
	{
		PRINTTESTRESULTS("TestPushFront_getDataFromBegin",i + 1, 
				num[3 - i] == *(int *)DLListGetData(node));
		++i;		
	}
	PRINTTESTRESULTS("TestPushFront_getDataFromBegin",i, 
				num[3 - i] == *(int *)DLListGetData(node));
	
	i = 0;
	for (node = DLListEnd(new_list); node != DLListBegin(new_list); 
									   node = DLListPrev(node))
	{
		PRINTTESTRESULTS("TestPushFront_getDataFromEnd",i + 1, 
				num[i] == *(int *)DLListGetData(node));
		++i;		
	}
	PRINTTESTRESULTS("TestPushFront_getDataFromEnd",i, 
				num[i] == *(int *)DLListGetData(node));
	printf("Destroy Doubely linked list\n");
	DLListDestroy(new_list);
	
	printf("\n\n");
}
/*
void TestPushBack()
{
	int num[] = {13,12,11,10};
	dll_node_t *node = NULL;
	int i = 0;
	
	dl_list_t *new_list = DLListCreate();
	printf("Create Doubely linked list\n");
	
	for (i = 0; i < 4; ++i)
	{
		PRINTTESTRESULTS("DLListPushFront_pushFront",i + 1,
		num[i] == *(int *)DLListGetData(DLListPushFront(new_list, &num[i])));
	}
	
	i = 0;			
	for (node = DLListBegin(new_list); node != DLListEnd(new_list); 
									   node = DLListNext(node))
	{
		PRINTTESTRESULTS("DLListPushFront_getData",i + 1, 
				num[3 - i] == *(int *)DLListGetData(node));
		++i;		
	}
	PRINTTESTRESULTS("DLListPushFront_getData",i, 
				num[3 - i] == *(int *)DLListGetData(node));
	printf("Destroy Doubely linked list\n");
	DLListDestroy(new_list);
}*/



	


