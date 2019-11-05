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
#define MAX(a,b) (a) >= (b) ? a : b

static void SwapData(void **data1, void **data2);
static sl_node_t *MoveNodeForward(sl_node_t *node, size_t steps);
void PrintSList(sl_node_t *head);

sl_node_t *SListCreateNode(void *data, sl_node_t *next)
{
	sl_node_t *new_node = (sl_node_t *)malloc(sizeof(sl_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	new_node -> data = data;
	new_node -> next = next;
	
	return new_node;
}

void SListFreeAll(sl_node_t *node)
{
	sl_node_t *next_node = node -> next;
	
	assert(NULL != node);
	
	while (NULL != next_node)
	{
		free(node);
		node = next_node;
		next_node = (next_node -> next);
	}
	
	free(node);
}

sl_node_t *SListInsert(sl_node_t *new_node, sl_node_t *pos)
{	
	assert(NULL != new_node);
	assert(NULL != pos);
	
	new_node -> next = pos -> next;
	pos -> next = new_node;
	SwapData(&(new_node -> data), &(pos -> data));
	
	return pos;
}

sl_node_t *SListInsertAfter(sl_node_t *new_node, sl_node_t *pos)
{
	assert(NULL != new_node);
	assert(NULL != pos);
	
	new_node -> next = pos -> next;
	pos -> next = new_node;
	
	return new_node;
}

size_t SListCount(const sl_node_t *head)
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

sl_node_t *SListRemove(sl_node_t *node)
{
	SwapData(&(node -> data), &((node -> next) -> data));
	SListRemoveAfter(node);
	
	return node;
}

sl_node_t *SListRemoveAfter(sl_node_t *node)
{
	sl_node_t *temp_node = NULL;
	
	if (NULL == (node -> next))
	{
		node = node -> next;
	}
	else
	{
		temp_node = (node -> next) -> next;
		node -> next = temp_node;
	}
	
	return node;
}

sl_node_t *SListFlip(sl_node_t *head)
{
	sl_node_t *runner0 = NULL;
	sl_node_t *runner1 = NULL;
	sl_node_t *runner2 = NULL;
	
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

const sl_node_t *SListFind(const sl_node_t *node, void *param, find_ptr ptr)
{
	while (NULL != node)
	{
		if(ptr(node, param))
		{
			return node;
		}
		
		node = node -> next;
	}
	
	return NULL;
}

sl_node_t *SListFindIntersection(sl_node_t *node1, sl_node_t *node2)
{
	size_t count_node1 = SListCount(node1);
	size_t count_node2 = SListCount(node2);
	size_t max_count = MAX(count_node1, count_node2);
	
	if (max_count == count_node1)
	{
		MoveNodeForward(node1, count_node1 - count_node2);
	}
	else
	{
		MoveNodeForward(node2, count_node2 - count_node1);
	}
	
	if (node2 == node1)
	{
		return NULL;
	}
	else
	{
		while (node2 != node1)
		{
			node1 = node1 -> next;
			node2 = node2 -> next;			
		}
		
		return node1;
	}
}

int SListHasLoop (sl_node_t *head)
{
	sl_node_t *runner2 = head -> next;
	sl_node_t *runner1 = head;
	
	assert(head);
	
	if (NULL != runner2)
	{
		runner2 = (runner2 -> next) -> next;
		runner1 = runner1 -> next;
		
		if ((runner2 == runner1) || (runner2 == runner1 -> next))
		{
			return 0;
		}
	}
	
	return 1;
}

int SListForEach(sl_node_t *node, void *param, for_each_ptr ptr)
{
	while (NULL != node)
	{
		if(ptr(node, param))
		{
			return 1;
		}
		
		node = node -> next;
	}
	
	return 0;
}

static void SwapData(void **data1, void **data2)
{
	void *temp_data = *data1;
	*data1 = *data2;
	*data2 = temp_data;
}

static sl_node_t *MoveNodeForward(sl_node_t *node, size_t steps)
{
	size_t i = 0;
	
	for (i = 0; i < steps; ++i)
	{
		node = node -> next;
	}
	
	return node;
}


