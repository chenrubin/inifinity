#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include "queue.h" /* stack functions */
#include "slist.h"

struct queue
{
    sl_node_t *rear;
    sl_node_t *front;
};

queue_t *QCreate()
{
	
	queue_t *new_queue = NULL;
	sl_node_t *dummy_node = NULL;
	
	new_queue = (queue_t *)malloc(sizeof(queue_t));
	if (NULL == new_queue)
	{
		return NULL;
	}
	
	dummy_node = SListCreateNode(0, NULL);
	
	new_queue -> rear = dummy_node;
	new_queue -> front = dummy_node;	
	
	return new_queue;
}

void QDestroy(queue_t *queue)
{
	assert(NULL != queue);
	
	SListFreeAll(rear);
	SListFreeAll(front);
	free(queue);	
}

int QEnqueue(queue_t *queue, void *data)
{
	sl_node_t *new_node = SListCreateNode(data, NULL);
	
	assert(NULL != queue);
	
	SListInsertAfter(new_node, queue -> rear);
	(queue -> rear) = queue -> (rear -> next);
	
	return 0;
}

void *QPeek(const queue_t *queue)
{
	assert(NULL != queue);
	
	return (queue -> (front -> (next -> data));
}

int QDequeue(queue_t *queue)
{
	assert(NULL != queue);
	
	if ((queue -> (front -> next)) == (queue -> rear))
	{
		
	}
	
	SListRemoveAfter(queue -> front);

	return 0;
}

int QIsEmpty(const queue_t *queue)
{
	assert(NULL != queue);
	
	return ((queue -> front) == (queue -> rear));
}

size_t QSize(const queue_t *queue)
{
	assert(NULL != queue);
	
	return (SListCount(queue -> front) - 1);
}


