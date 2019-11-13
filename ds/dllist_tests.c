#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "dllist.h"
#include "MyUtils.h" /* MAX2,MIN2 */

void TestCreateDestroy();
void TestPushFront();
void TestPushBack();
void TestInsert();
void TestRemove();
void TestPopFront();
void TestPopBack();
void TestSplice();
void TestIsEmptySize();
void TestIsSameIterator();
void TestForEachAndFind();

int FindFunc(const dll_node_t *node, const void *param);
int ForEachFunc(dll_node_t *node, void *param);
static dll_iter_t ReturnIteratorFromBegin(dl_list_t *list, int num);
void FreeAll(dll_node_t *node);

struct dl_list
{
	dll_node_t *begin;
	dll_node_t *end;
};

struct node 
{
	void *data;
	struct node *prev;
	struct node *next;
};

int main()
{
	TestCreateDestroy();
	TestPushFront();
	TestPushBack();
	TestInsert();
	TestRemove();
	TestPopFront();
	TestPopBack();
	TestSplice();
	TestIsEmptySize();
	TestIsSameIterator();
	TestForEachAndFind();				
	
	return 0;
}

void TestCreateDestroy()
{	
	dl_list_t *new_list = DLListCreate();
	printf("Create Doubley linked list\n");
	DLListDestroy(new_list);
	printf("Destroy Doubley linked list\n");
	
	printf("NOW CHECK VALGRIND FOR NO MEMORY LEAKS\n\n");
}

void TestPushFront()
{
	int num[] = {13,12,11,10};
	dll_node_t *node = NULL;
	int i = 0;
	
	dl_list_t *new_list = DLListCreate();
	printf("Create Doubley linked list\n");
	
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
	
	i = 0;
	for (node = DLListEnd(new_list) -> prev; node != new_list -> begin;
									   node = DLListPrev(node))
	{
		PRINTTESTRESULTS("TestPushFront_getDataFromEnd",i + 1, 
				num[i] == *(int *)DLListGetData(node));
		++i;		
	}

	printf("Destroy Doubley linked list\n");
	DLListDestroy(new_list);
	
	printf("\n\n");
}

void TestPushBack()
{
	int num[] = {10,11,12,13};
	dll_node_t *node = NULL;
	int i = 0;
	
	dl_list_t *new_list = DLListCreate();
	printf("Create Doubley linked list\n");
	
	for (i = 0; i < 4; ++i)
	{
		PRINTTESTRESULTS("TestPushBack_pushBack",i + 1,
		num[i] == *(int *)DLListGetData(DLListPushBack(new_list, &num[i])));
	}
	
	i = 0;			
	for (node = DLListBegin(new_list); node != DLListEnd(new_list); 
									   node = DLListNext(node))
	{
		PRINTTESTRESULTS("TestPushBack_getData",i + 1, 
				num[i] == *(int *)DLListGetData(node));
		++i;		
	}

	printf("Destroy Doubley linked list\n");
	DLListDestroy(new_list);
	
	printf("\n\n");
}

void TestInsert()
{
	int num[] = {10,11,12,13,14,15};
	int inserted_num = 22;
	int num_after_insert_middle[] = {10,11,12,13,22,14,15};
	int num_after_insert_end[] = {10,11,12,13,14,15,22};
	int num_after_insert_before_first[] = {22,10,11,12,13,14,15};
	int i = 0;
	dl_list_t *new_list = NULL;
	dll_node_t *node = NULL;
	
	printf("\n!!!!!!!!Insert in the middle!!!!!!!!!\n\n");
	
	new_list = DLListCreate();
	printf("Create Doubley linked list\n");
	
	for (i = 0; i < 6; ++i)
	{
		PRINTTESTRESULTS("TestInsert_pushBack",i + 1,
		num[i] == *(int *)DLListGetData(DLListPushBack(new_list, &num[i])));
	}
	
	PRINTTESTRESULTS("TestInsert_insertMiddle",5, 
	inserted_num == 
	*(int *)DLListGetData(DLListInsert(&inserted_num, 
	ReturnIteratorFromBegin(new_list, 5), new_list)));
	
	i = 0;
	for (node = DLListBegin(new_list); node != DLListEnd(new_list); 
									   node = DLListNext(node))
	{
		PRINTTESTRESULTS("TestInsert_GetData",i + 1,
		num_after_insert_middle[i] == *(int *)DLListGetData(node));
		++i;		
	}
	
	printf("Destroy Doubley linked list\n");
	DLListDestroy(new_list);
	
	printf("\n!!!!!!!!Insert in the end!!!!!!!!!\n\n");
	
	new_list = DLListCreate();
	printf("Create Doubley linked list\n");
	
	i = 0;
	for (i = 0; i < 6; ++i)
	{
		PRINTTESTRESULTS("TestInsert_pushBack",i + 1,
		num[i] == *(int *)DLListGetData(DLListPushBack(new_list, &num[i])));
	}
	
	PRINTTESTRESULTS("TestInsert_insertEnd",5, 
	inserted_num == 
	*(int *)DLListGetData(DLListInsert(&inserted_num, 
	ReturnIteratorFromBegin(new_list, 7), new_list)));
	
	i = 0;
	for (node = DLListBegin(new_list); node != DLListEnd(new_list);
									   node = DLListNext(node))
	{
		PRINTTESTRESULTS("TestInsert_GetData",i + 1,
		num_after_insert_end[i] == *(int *)DLListGetData(node));
		++i;		
	}
	
	printf("Destroy Doubley linked list\n");
	DLListDestroy(new_list);
	
	printf("\n!!!!!!!!Insert at the beginning!!!!!!!!!\n\n");
	
	new_list = DLListCreate();
	printf("Create Doubley linked list\n");
	
	i = 0;
	for (i = 0; i < 6; ++i)
	{
		PRINTTESTRESULTS("TestInsert_pushBack",i + 1,
		num[i] == *(int *)DLListGetData(DLListPushBack(new_list, &num[i])));
	}
	
	PRINTTESTRESULTS("TestInsert_insertMiddle",5, 
	inserted_num == 
	*(int *)DLListGetData(DLListInsert(&inserted_num, 
	ReturnIteratorFromBegin(new_list, 1), new_list)));
	
	i = 0;
	for (node = DLListBegin(new_list); node != DLListEnd(new_list); 
									   node = DLListNext(node))
	{
		PRINTTESTRESULTS("TestInsert_GetData",i + 1,
		num_after_insert_before_first[i] == *(int *)DLListGetData(node));
		++i;		
	}
	
	printf("Destroy Doubley linked list\n");
	DLListDestroy(new_list);
	
	printf("\n\n");
}

void TestRemove()
{
	int num[] = {10,11,12,13,14,15};
	int num_after_remove_middle[] = {10,11,12,13,15};
	int num_after_begin[] = {11,12,13,14,15};
	int num_after_remove_end[] = {10,11,12,13,14};
	int i = 0;
	dl_list_t *new_list = NULL;
	dll_node_t *node = NULL;
	int recieved_num = 0;
	dll_node_t *ReturnIteratorFromBegin_iter = NULL;
	dll_node_t *DLListRemove_nodeReturn = NULL;
	
	printf("!!!!!!!!Remove from middle!!!!!!!!!\n\n");
	
	new_list = DLListCreate();
	printf("Create Doubley linked list\n");
	
	for (i = 0; i < 6; ++i)
	{
		PRINTTESTRESULTS("TestRemove_pushBack",i + 1,
		num[i] == *(int *)DLListGetData(DLListPushBack(new_list, &num[i])));
	}
	
	recieved_num = num[5];
	PRINTTESTRESULTS("TestRemove_removeMiddle",5, 
	recieved_num == 
	*(int *)DLListGetData(DLListRemove(ReturnIteratorFromBegin(new_list, 5))));
	
	i = 0;
	for (node = DLListBegin(new_list); node != DLListEnd(new_list); 
									   node = DLListNext(node))
	{
		PRINTTESTRESULTS("TestRemove_GetData",i + 1,
		num_after_remove_middle[i] == *(int *)DLListGetData(node));
		++i;		
	}
	
	printf("Destroy Doubley linked list\n");
	DLListDestroy(new_list);
	
	printf("\n!!!!!!!!Remove from the end!!!!!!!!!\n\n");
	
	new_list = DLListCreate();
	printf("Create Doubley linked list\n");
	
	i = 0;
	for (i = 0; i < 6; ++i)
	{
		PRINTTESTRESULTS("TestRemove_pushBack",i + 1,
		num[i] == *(int *)DLListGetData(DLListPushBack(new_list, &num[i])));
	}

	ReturnIteratorFromBegin_iter = ReturnIteratorFromBegin(new_list, 6);
	DLListRemove_nodeReturn = DLListRemove(ReturnIteratorFromBegin_iter);

	PRINTTESTRESULTS("TestRemove_removeEnd",5, DLListRemove_nodeReturn -> next == NULL);
	
	i = 0;
	for (node = DLListBegin(new_list); node != DLListEnd(new_list); 
									   node = DLListNext(node))
	{
		PRINTTESTRESULTS("TestRemove_GetData",i + 1,
		num_after_remove_end[i] == *(int *)DLListGetData(node));
		++i;
	}
	
	printf("Destroy Doubley linked list\n");
	DLListDestroy(new_list);
	
	printf("\n!!!!!!!!Remove from the beginning!!!!!!!!!\n\n");
	
	new_list = DLListCreate();
	printf("Create Doubley linked list\n");
	
	i = 0;
	for (i = 0; i < 6; ++i)
	{
		PRINTTESTRESULTS("TestRemove_pushBack",i + 1,
		num[i] == *(int *)DLListGetData(DLListPushBack(new_list, &num[i])));
	}
	
	recieved_num = *(int *)new_list -> begin -> next -> next -> data;
	PRINTTESTRESULTS("TestRemove_removeBegin",5, 
	recieved_num == 
	*(int *)DLListGetData(DLListRemove(ReturnIteratorFromBegin(new_list, 1))));
	
	i = 0;
	for (node = DLListBegin(new_list); node != DLListEnd(new_list); 
									   node = DLListNext(node))
	{
		PRINTTESTRESULTS("TestRemove_GetData",i + 1,
		num_after_begin[i] == *(int *)DLListGetData(node));
		++i;		
	}
	
	printf("Destroy Doubley linked list\n");
	DLListDestroy(new_list);
	
	printf("\n\n");
}

void TestPopFront()
{
	int num[] = {10,11,12,13,14,15};
	int num_after_pop_front[] = {11,12,13,14,15};
	int i = 0;
	dl_list_t *new_list = NULL;
	dll_node_t *node = NULL;
	
	new_list = DLListCreate();
	printf("Create Doubley linked list\n");
	
	for (i = 0; i < 6; ++i)
	{
		PRINTTESTRESULTS("TestPopFront_pushBack", i + 1,
		num[i] == *(int *)DLListGetData(DLListPushBack(new_list, &num[i])));
	}
	
	PRINTTESTRESULTS("TestPopFront_popFront", 5, 
	10 == *(int *)DLListPopFront(new_list));
	
	i = 0;
	for (node = DLListBegin(new_list); node != DLListEnd(new_list); 
									   node = DLListNext(node))
	{
		PRINTTESTRESULTS("TestPopFront_GetData", i + 1,
		num_after_pop_front[i] == *(int *)DLListGetData(node));
		++i;		
	}
	
	i = 4;
	for (node = (DLListEnd(new_list) -> prev); node != (new_list -> begin);
									   node = DLListPrev(node))
	{
		PRINTTESTRESULTS("TestPopFront_GetData", 4 - i,
		num_after_pop_front[i] == *(int *)DLListGetData(node));
		--i;		
	}
			
	printf("Destroy Doubley linked list\n");
	DLListDestroy(new_list);
	
	printf("\n\n");
}

void TestPopBack()
{
	int num[] = {10,11,12,13,14,15};
	int num_after_pop_front[] = {10,11,12,13,14};
	int i = 0;
	dl_list_t *new_list = NULL;
	dll_node_t *node = NULL;
	
	new_list = DLListCreate();
	printf("Create Doubley linked list\n");
	
	for (i = 0; i < 6; ++i)
	{
		PRINTTESTRESULTS("TestPopBack_pushBack", i + 1,
		num[i] == *(int *)DLListGetData(DLListPushBack(new_list, &num[i])));
	}
	
	PRINTTESTRESULTS("TestPopBack_popBack", 5, 
	15 == *(int *)DLListPopBack(new_list));
	
	i = 0;
	for (node = DLListBegin(new_list); node != DLListEnd(new_list); 
									   node = DLListNext(node))
	{
		PRINTTESTRESULTS("TestPopBack_GetData", i + 1,
		num_after_pop_front[i] == *(int *)DLListGetData(node));
		++i;		
	}
	
	i = 4;
	for (node = (DLListEnd(new_list) -> prev); node != (new_list -> begin);
									   node = DLListPrev(node))
	{
		PRINTTESTRESULTS("TestPopBack_GetData", 4 - i,
		num_after_pop_front[i] == *(int *)DLListGetData(node));
		--i;		
	}
			
	printf("Destroy Doubley linked list\n");
	DLListDestroy(new_list);
	
	printf("\n\n");
}

void TestSplice()
{
	int num1[] = {10,11,12,13,14,15};
	int num2[] = {20,21,22,23,24,25};
	int res[] = {10,11,21,22,23,12,13,14,15};
	dl_list_t *new_list_1 = NULL;
	dl_list_t *new_list_2 = NULL;	
	dll_node_t *node = NULL;
	dll_node_t *s_begin = NULL;
	dll_node_t *s_end = NULL;
	dll_node_t *dest = NULL;	
	int i = 0;
	
	new_list_1 = DLListCreate();
	new_list_2 = DLListCreate();
	
	printf("Create two Doubley linked lists\n");
	
	for (i = 0; i < 6; ++i)
	{
		PRINTTESTRESULTS("TestSplice_pushBack", i + 1,
		num1[i] == *(int *)DLListGetData(DLListPushBack(new_list_1, &num1[i])));
	}
	for (i = 0; i < 6; ++i)
	{
		PRINTTESTRESULTS("TestSplice_pushBack", i + 6,
		num2[i] == *(int *)DLListGetData(DLListPushBack(new_list_2, &num2[i])));
	}
	
	i = 0;
	for (node = DLListBegin(new_list_1); node != DLListEnd(new_list_1);
									   node = DLListNext(node))
	{
		PRINTTESTRESULTS("TestSplice_GetData", i + 12,
		num1[i] == *(int *)DLListGetData(node));
		++i;		
	}
	
	i = 0;
	for (node = DLListBegin(new_list_2); node != DLListEnd(new_list_2);
									   node = DLListNext(node))
	{
		PRINTTESTRESULTS("TestSplice_GetData", i + 18,
		num2[i] == *(int *)DLListGetData(node));
		++i;		
	}
	
	s_begin = ReturnIteratorFromBegin(new_list_2, 2);
	s_end = ReturnIteratorFromBegin(new_list_2, 5);
	dest = ReturnIteratorFromBegin(new_list_1, 2);
	
	PRINTTESTRESULTS("TestSplice_Splice", 24,
	dest == DLListSplice(s_begin, s_end, dest));
	
	i = 0;
	for (node = DLListBegin(new_list_1); node != DLListEnd(new_list_1);
									   node = DLListNext(node))
	{
		PRINTTESTRESULTS("TestSplice_GetData", i + 25,
		res[i] == *(int *)DLListGetData(node));
		++i;		
	}
	
	i = 0;
	for (node = (DLListEnd(new_list_1) -> prev); node != (new_list_1 -> begin);
									   node = DLListPrev(node))
	{
		PRINTTESTRESULTS("TestSplice_GetData", i + 36,
		res[9 - i - 1] == *(int *)DLListGetData(node));
		++i;		
	}
			
	printf("Destroy Doubley linked list\n");								   
	
	DLListDestroy(new_list_1);
	DLListDestroy(new_list_2);
	
	printf("\n\n");
}

void TestIsEmptySize()
{
	int num[] = {10,11,12,13,14,15};
	int i = 0;
	dl_list_t *new_list = NULL;
	
	new_list = DLListCreate();
	printf("Create Doubley linked list\n");
	
	for (i = 0; i < 6; ++i)
	{
		if (i == 0)
		{
			PRINTTESTRESULTS("TestIsEmptySize_IsEmpty", 2*i,
			1 == DLListIsEmpty(new_list));	
		}
		else
		{
			PRINTTESTRESULTS("TestIsEmptySize_IsEmpty", 2*i,
			0 == DLListIsEmpty(new_list));
		}
		
		PRINTTESTRESULTS("TestIsEmptySize_size", 2*i + 1,
			(size_t)i == DLListSize(new_list));
			
		PRINTTESTRESULTS("TestIsEmptySize_pushBack", 2*i + 1,
		num[i] == *(int *)DLListGetData(DLListPushBack(new_list, &num[i])));
	}
	
	for (i = 0; i < 6; ++i)
	{
		printf("Remove last\n");
		DLListRemove(DLListEnd(new_list) -> prev);
		
		if (i == 5)
		{
			PRINTTESTRESULTS("TestIsEmptySize_IsEmpty", 2*i + 12,
			1 == DLListIsEmpty(new_list));	
		}
		else
		{
			PRINTTESTRESULTS("TestIsEmptySize_IsEmpty", 2*i + 12,
			0 == DLListIsEmpty(new_list));
		}
		
		PRINTTESTRESULTS("TestIsEmptySize_size", 2*i + 1,
			(size_t)(5 - i) == DLListSize(new_list));
	}
		
	printf("Destroy Doubley linked list\n");
	DLListDestroy(new_list);
	
	printf("\n\n");
}

void TestIsSameIterator()
{
	int num[] = {10,11,12,13,14,15};
	int i = 0;
	dl_list_t *new_list = NULL;
	dll_iter_t node = NULL;
	
	new_list = DLListCreate();
	printf("Create Doubley linked list\n");
	
	for (i = 0; i < 6; ++i)
	{			
		PRINTTESTRESULTS("TestIsSameIterator_pushBack", i,
		num[i] == *(int *)DLListGetData(DLListPushBack(new_list, &num[i])));
	}
	
	i = 0;
	for (node = DLListBegin(new_list); node != DLListEnd(new_list);
									   node = DLListNext(node))
	{
		if (2 == i)
		{
			PRINTTESTRESULTS("TestIsSameIterator_IsSameIterator", i + 6,
			1 == DLListIsSameIterator(node, 
									new_list -> begin -> next -> next -> next));	
		}
		else
		{
			PRINTTESTRESULTS("TestIsSameIterator_IsSameIterator", i + 6,
			0 == DLListIsSameIterator(node, 
									new_list -> begin -> next -> next -> next));
		}
		
		++i;
	}
		
	printf("Destroy Doubley linked list\n");
	DLListDestroy(new_list);
	
	printf("\n\n");
}
	
void TestForEachAndFind()
{
	int num[] = {10,20,30,40,50,60};
	int num_to_find = 11;
	int num_to_add = 1;
	dl_list_t *new_list = NULL;
	int i = 0;
	
	new_list = DLListCreate();
	printf("Create Doubley linked list\n");
	
	for (i = 0; i < 6; ++i)
	{			
		PRINTTESTRESULTS("TestIsSameIterator_pushBack", i,
		num[i] == *(int *)DLListGetData(DLListPushBack(new_list, &num[i])));
	}
	
	PRINTTESTRESULTS("TestIsSameIterator_Find", 6,
				new_list -> end == DLListFind(new_list -> begin -> next -> next, 
				new_list -> end, &num_to_find, FindFunc));
				
	PRINTTESTRESULTS("TestIsSameIterator_ForEach", 7,
				0 == DLListForEach(new_list -> begin -> next, 
				new_list -> end -> prev, &num_to_add, ForEachFunc));
				
	PRINTTESTRESULTS("TestIsSameIterator_Find", 8,
				new_list -> end == DLListFind(new_list -> begin -> next -> next, 
				new_list -> end, &num_to_find, FindFunc));
				
	PRINTTESTRESULTS("TestIsSameIterator_Find", 9,
				((new_list -> begin) -> next) == DLListFind(new_list -> begin -> next, 
				new_list -> end, &num_to_find, FindFunc));								
		
	printf("Destroy Doubley linked list\n");
	DLListDestroy(new_list);
	
	printf("\n\n");
}

static dll_iter_t ReturnIteratorFromBegin(dl_list_t *list, int num)
{
	int i = 0;
	dll_iter_t iter = list -> begin;
	
	for (i = 0; i < num; ++i)
	{
		iter = DLListNext(iter);
	}
	
	return iter;
}

int FindFunc(const dll_node_t *node, const void *param)
{
	return (*(int *)node -> data == *(int *)param);
}

int ForEachFunc(dll_node_t *node, void *param)
{
	*(int *)node -> data += *(int *)param;					   
									   
	return 0;
}
