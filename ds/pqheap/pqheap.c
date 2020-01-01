/************************************
*		Author: ChenR				  *
*		Reviewer: Gal				  *
*		PQ							  *
*		18/11/2019					  *
*									  *
************************************/
#include <stdlib.h> 	/* malloc */
#include <assert.h> 	/* assert */

#include "pqheap.h"			/* priority queue */
#include "../heap/heap.h"
#include "../heap/heapify.h"

struct p_queue
{
	heap_t *heap;
};

p_queue_t *PQCreate(void *priority_param, int(*compare_func)
				   (const void *new_data, const void *src_data, void *param))
{
	p_queue_t *new_pq = (p_queue_t *)malloc(sizeof(p_queue_t));
	if (NULL == new_pq)
	{
		return NULL;
	}
	
	assert(compare_func);
	
	new_pq -> heap = HeapCreate(compare_func, priority_param);
	
	if (NULL == new_pq -> heap)
	{
		free(new_pq);
		new_pq = NULL;
		
		return NULL;
	}
	
	return new_pq;
}

void PQDestroy(p_queue_t *p_queue)
{
	assert(p_queue);
	
	HeapDestroy(p_queue -> heap);
	free(p_queue);
	p_queue = NULL;
}

int PQEnqueue(p_queue_t *p_queue, void *data)
{
	assert(p_queue);

	return (HeapPush(p_queue -> heap, data));
}

void *PQDequeue(p_queue_t *p_queue)
{
	void *data = NULL;
	
	assert(p_queue);
	
	data = HeapPeek(p_queue -> heap);
	HeapPop(p_queue -> heap);
	
	return data;
}

void *PQPeek(const p_queue_t *p_queue)
{
	assert(p_queue);
	
	return (HeapPeek(p_queue -> heap));
}

size_t PQSize(const p_queue_t *p_queue)
{
	return (HeapSize(p_queue -> heap));
}

int PQIsEmpty(const p_queue_t *p_queue)
{
	assert(p_queue);
	
	return (HeapIsEmpty(p_queue -> heap));
}

void PQClear(p_queue_t *p_queue)
{
	assert(p_queue);
	
	while (!PQIsEmpty(p_queue))
	{
		PQDequeue(p_queue);
	}
}

void *PQErase(void *data, p_queue_t *p_queue, 
 			  int(*is_match)(const void *new_data, const void *param))
{	
	return (HeapRemove(p_queue -> heap, is_match, data));
}
