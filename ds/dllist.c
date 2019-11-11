/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		dllist							  *
*		7/11/2019					  *
*									  *
************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "dllist.h"
#include "MyUtils.h" /* MAX2,MIN2 */

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

static dll_node_t *CreateNode(void *data, dll_node_t *next);
static void FreeAll(dll_node_t *node);
static dll_node_t *InsertAfter(dll_node_t *new_node, dll_node_t *pos);
static dll_node_t *InsertBefore(dll_node_t *new_node, dll_node_t *pos);


dl_list_t *DLListCreate(void)
{
	int data = 0xDEADBEEF;
	
	dl_list_t *new_dll = (dl_list_t *)malloc(sizeof(dl_list_t));
	if (NULL == new_dll)
	{
		return NULL;
	}
	
	new_dll -> begin = CreateNode(&data, NULL);
	if (NULL == (new_dll -> begin))
	{
		new_dll -> begin = NULL;
		DLListDestroy(new_dll);
		
		return NULL;
	}
	
	new_dll -> end = CreateNode(&data, NULL);
	if (NULL == (new_dll -> end))
	{
		new_dll -> end = NULL;
		DLListDestroy(new_dll);
		
		return NULL;	
	}
	
	(new_dll -> begin -> next) = new_dll -> end;
	(new_dll -> begin -> prev) = NULL;
	(new_dll -> end -> next) = NULL;
	(new_dll -> end -> prev) = new_dll -> begin;
	
	return new_dll;
}

void DLListDestroy(dl_list_t *list)
{
	FreeAll(list -> begin);
	list -> end = NULL;
	free(list);
}

dll_iter_t DLListPushFront(dl_list_t *list, void *data)
{
	dll_node_t *new_node = CreateNode(data, NULL);
	if (NULL == new_node)
	{
		return NULL;
	}
	
	return (InsertAfter(new_node, list -> begin));
}

dll_iter_t DLListPushBack(dl_list_t *list, void *data)
{
	dll_node_t *new_node = CreateNode(data, NULL);
	if (NULL == new_node)
	{
		return NULL;
	}
	
	return (InsertBefore(new_node, list -> end));
}

void *DLListGetData(dll_iter_t iterator)
{
	return iterator -> data;
}

dll_iter_t DLListBegin(const dl_list_t *list)
{
	return (list -> begin -> next);
}

dll_iter_t DLListEnd(const dl_list_t *list)
{
	return (list -> end -> prev);
}

dll_iter_t DLListNext(dll_iter_t iterator)
{
	return (iterator -> next);
}

dll_iter_t DLListPrev(dll_iter_t iterator)
{
	return (iterator -> prev);
}

/*****************************************************************/

static dll_node_t *CreateNode(void *data, dll_node_t *next)
{
	dll_node_t *new_node = (dll_node_t *)malloc(sizeof(dll_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	assert(NULL != data);
	
	new_node -> data = data;
	new_node -> next = next;
	
	return new_node;
}

static void FreeAll(dll_node_t *node)
{
	dll_node_t *temp_node = NULL;
	
	assert(NULL != node);
	
	while (NULL != node)
	{
		temp_node = node;
		node = node -> next;
		free(temp_node);
	}
}

static dll_node_t *InsertAfter(dll_node_t *new_node, dll_node_t *pos)
{
	assert(NULL != new_node);
	assert(NULL != pos);
	
	new_node -> next = pos -> next;
	pos -> next -> prev = new_node;
	pos -> next = new_node;
	new_node -> prev = pos -> next;
	
	return new_node;
}

static dll_node_t *InsertBefore(dll_node_t *new_node, dll_node_t *pos)
{
	assert(NULL != new_node);
	assert(NULL != pos);
	
	new_node -> prev = pos -> prev;
	pos -> prev -> next = new_node;
	pos -> prev = new_node;
	new_node -> next = pos -> prev;
	
	return new_node;
}
