/********************************
*	Author: ChenR				*
*	Reviewer: EyalF				*
*	ds_SList					*
*								*
*								*
********************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include "slist.h"

#define EXTEND_SHRINK_FACTOR 2
#define SHRINK_LIMIT 4

static void SwapData(void **data1, void **data2);
void PrintSList(node_t *head);

node_t *SListCreateNode(void *data, node_t *next)
{
	node_t *new_node = (node_t *)malloc(sizeof(node_t));
	
	new_node -> data = data;
	new_node -> next = next;
	
	return new_node;
}

void SListFreeNode(node_t *node)
{
	node_t *next_node = node -> next;
	
	assert(NULL != node);
	
	while (NULL != next_node)
	{
		free(node);
		node = next_node;
		next_node = (next_node -> next);
	}
	
	free(node);
}

node_t *SListInsert(node_t *new_node, node_t *pos)
{	
	assert(NULL != new_node);
	assert(NULL != pos);
	
	new_node -> next = pos -> next;
	pos -> next = new_node;
	SwapData(&(new_node -> data), &(pos -> data));
	
	return pos;
}

node_t *SListInsertAfter(node_t *new_node, node_t *pos)
{
	assert(NULL != new_node);
	assert(NULL != pos);
	
	new_node -> next = pos -> next;
	pos -> next = new_node;
	
	return new_node;
}

size_t SListCount(const node_t *head)
{
	size_t counter = 0;
	
	assert(NULL != head);
	
	while (NULL != head -> next)
	{
		counter += 1;
		head = (head -> next);
	}
	
	return (counter + 1);
}

node_t *SListRemove(node_t *node)
{
	node_t *temp_node = (node -> next) -> next;
	
	node -> data = (node -> next) -> data;
	free(node -> next);
	node -> next = temp_node;
	
	return node;
}

node_t *SListRemoveAfter(node_t *node)
{
	node_t *temp_node = (node -> next) -> next;
	
	free(node -> next);
	node -> next = temp_node;
	
	return node;
}

node_t *SListFlip(node_t *head)
{
	node_t *runner0 = NULL;
	node_t *runner1 = NULL;
	node_t *runner2 = NULL;
	
	if (NULL == (head -> next))
	{
		return head;
	}
	else if (NULL == ((head -> next) -> next))
	{
		runner1 = head -> next;
		head -> next = NULL;
		runner1 -> next = head;
		
		return runner1;
	}
	else
	{
		runner0 = head;
		runner1 = head -> next;
		runner2 = (head -> next) -> next;
		
		head -> next = NULL;
		
		while (NULL != runner2)
		{
			runner1 -> next = runner0;
			runner0 = runner1;
			runner1 = runner2;
			runner2 = runner2 -> next;			
		}
		
		runner1 -> next = runner0;
		
		return runner1;
	}
}

node_t *SListFind(const node_t *node, void *param, find_ptr ptr)
{
	while (NULL != node)
	{
		if(find_ptr(node, param))
		{
			return node;
		}
		
		node = node -> next;
	}
	
	return NULL;
}

static void SwapData(void **data1, void **data2)
{
	void *temp_data = *data1;
	*data1 = *data2;
	*data2 = temp_data;
}


