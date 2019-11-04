/********************************
*	Author: ChenR				*
*	Reviewer: EyalF				*
*	ds_SList					*
*						*
*								*
********************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include "slist.h"

#define EXTEND_SHRINK_FACTOR 2
#define SHRINK_LIMIT 4

static void SwapData(void **data1, void **data2);

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
	
	while (NULL != next_node)
	{
		free(node);
		next_node = next_node -> next;
	}
	
	free(node);
}

node_t *SListInsert(node_t *new_node, node_t *pos)
{	
	new_node -> next = pos -> next;
	pos -> next = new_node;
	SwapData(&(new_node -> data), &(pos -> data));
	
	return pos;
}

node_t *SListInsertAfter(node_t *new_node, node_t *pos)
{
	new_node -> next = pos -> next;
	pos -> next = new_node;
	
	return new_node;
}

static void SwapData(void **data1, void **data2)
{
	void *temp_data = *data1;
	*data1 = *data2;
	*data2 = temp_data;
}
