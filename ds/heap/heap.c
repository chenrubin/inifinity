/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		heap						  *
*		7/11/2019					  *
*									  *
************************************/

#include <stdlib.h>

#include "heap.h"
#include "heapify.h"
#include "../vector/vector.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

#define POINTER_SIZE sizeof(size_t)
#define CAPACITY 20
#define FIRST_ELEMENT_INDEX 0
#define LAST_ELEMENT_INDEX (HeapSize(heap) - 1)

static void SwapIMP(void *ptr1, void *ptr2);

struct heap
{
	d_vector_t *vector;
	comparison_t comparison_func;
	void *param;
	
};

heap_t *HeapCreate(comparison_t comparison_func, void *comparison_param)
{
	heap_t *new_heap = (heap_t *)malloc(sizeof(heap_t));
	if (NULL == new_heap)
	{
		return NULL;
	}
	
	new_heap -> vector = VectorCreate(CAPACITY, POINTER_SIZE);
	if (NULL == new_heap -> vector)
	{
		free(new_heap);
		new_heap = NULL;
	}
	
	new_heap -> comparison_func = comparison_func;
	new_heap -> param = comparison_param;
	
	return new_heap;
}

void HeapDestroy(heap_t *heap)
{
	VectorDestroy(heap -> vector);
	free(heap);
}

int HeapPush(heap_t *heap, void *data)
{
	VectorPushBack(heap -> vector, &data);
	HeapifyUp(VectorGetItemAddress(heap -> vector, 0), 
			  HeapSize(heap), 
			  sizeof(void *), 
			  LAST_ELEMENT_INDEX, 
			  heap -> comparison_func, 
			  heap -> param);
	return 0;		    
}

void HeapPop(heap_t *heap)
{
	void *first_address = VectorGetItemAddress(heap -> vector, 
											   FIRST_ELEMENT_INDEX);
	void *last_address = VectorGetItemAddress(heap -> vector,
											  HeapSize(heap) - 1);										   
	SwapIMP(first_address, last_address);
	HeapifyDown(VectorGetItemAddress(heap -> vector, 0), 
				HeapSize(heap), 
				sizeof(void *), 
			    FIRST_ELEMENT_INDEX, 
			    heap -> comparison_func, 
			    heap -> param);
}

void *HeapPeek(const heap_t *heap)
{
	return (VectorGetItemAddress(heap -> vector, HeapSize(heap) - 1));
}

size_t HeapSize(const heap_t *heap)
{
	return (VectorSize(heap -> vector));
}

int HeapIsEmpty(const heap_t *heap)
{
	return (0 == HeapSize(heap));
}

static void SwapIMP(void *ptr1, void *ptr2)
{
	void *temp_address = ptr1;
	ptr1 = ptr2;
	ptr2 = temp_address;
}
