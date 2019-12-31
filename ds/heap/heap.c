/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		heap						  *
*		7/11/2019					  *
*									  *
************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "heap.h"
#include "heapify.h"
#include "../vector/vector.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

#define POINTER_SIZE sizeof(size_t)
#define CAPACITY 20
#define FIRST_ELEMENT_INDEX 0
#define LAST_ELEMENT_INDEX (HeapSize(heap) - 1)
#define RIGHT_CHILD_INDEX(x) (((2) * (x)) + (2))
#define LEFT_CHILD_INDEX(x) (((2) * (x)) + (1))
#define PARENT_INDEX(x) ((x - 1) / (2))

enum status
{
	SUCCESS = 0,
	FAIL = 1
};

static int GenericComparisonFuncIMP(const void *new_data, 
							const void *src_data,
							void *compare_param);
static void SwapIMP(void **ptr1, void **ptr2);
void PrintArrayIMP(heap_t *heap);
static int GetIndexToRemoveIMP(heap_t *heap, void *data, is_match_t func);
static int IsHeapifyUpIMP(heap_t *heap, size_t index_to_remove);
static size_t GetParentIndexIMP(size_t child_index);

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
			  GenericComparisonFuncIMP, 
			  heap);
	return 0;		    
}

void HeapPop(heap_t *heap)
{
	void *first_address = VectorGetItemAddress(heap -> vector, 
											   FIRST_ELEMENT_INDEX);
	void *last_address = VectorGetItemAddress(heap -> vector,
											  HeapSize(heap) - 1);										   
	SwapIMP(first_address, last_address);
	VectorPopBack(heap -> vector);
	HeapifyDown(VectorGetItemAddress(heap -> vector, 0), 
				HeapSize(heap), 
				sizeof(void *), 
			    FIRST_ELEMENT_INDEX, 
			    GenericComparisonFuncIMP, 
			    heap);
}

void *HeapPeek(const heap_t *heap)
{
	void **ptr = NULL;
	
	assert(heap);
	
	ptr = (VectorGetItemAddress(heap -> vector, 0));
	
	return *ptr;
}

size_t HeapSize(const heap_t *heap)
{
	return (VectorSize(heap -> vector));
}

int HeapIsEmpty(const heap_t *heap)
{
	return (0 == HeapSize(heap));
}

int HeapRemove(heap_t *heap, is_match_t is_match_func, void *param)
{
	int index_to_remove = GetIndexToRemoveIMP(heap, param, is_match_func);
	
	if (-1 == index_to_remove)
	{
		return FAIL;
	}
	
	if ((size_t)index_to_remove == LAST_ELEMENT_INDEX)
	{
		VectorPopBack(heap -> vector);
	}
	else
	{	
		SwapIMP(VectorGetItemAddress(heap -> vector, index_to_remove), 
				VectorGetItemAddress(heap -> vector, LAST_ELEMENT_INDEX));
		VectorPopBack(heap -> vector);
		
		if (IsHeapifyUpIMP(heap, index_to_remove))
		{
			HeapifyUp(VectorGetItemAddress(heap -> vector, 0), 
				  	  HeapSize(heap), 
				  	  sizeof(void *), 
				  	  index_to_remove, 
				  	  GenericComparisonFuncIMP, 
				  	  heap);
		}
		
		HeapifyDown(VectorGetItemAddress(heap -> vector, 0), 
					HeapSize(heap), 
					POINTER_SIZE, 
					index_to_remove, 
					GenericComparisonFuncIMP, 
					heap);
	}
	
	return SUCCESS;		
}

static int IsHeapifyUpIMP(heap_t *heap, size_t index_to_remove)
{
	size_t parent_index = GetParentIndexIMP(index_to_remove);
	if (parent_index != index_to_remove)
	{
		if (1 != GenericComparisonFuncIMP(VectorGetItemAddress(heap -> vector, 
															   index_to_remove), 
									   	  VectorGetItemAddress(heap -> vector, 
									   						   parent_index),
									   						   heap))
		{
			return 1;
		}
	}
	
	return 0;
}

static int GetIndexToRemoveIMP(heap_t *heap, void *data, is_match_t func)
{
	size_t i = 0;
	
	for (i = 0; i < HeapSize(heap); ++i)
	{
		if (func(data, *(char **)VectorGetItemAddress(heap -> vector, i)))
		{
			return i;
		}
	}
	
	return -1;
}

static void SwapIMP(void **ptr1, void **ptr2)
{
	void *temp_address = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = temp_address;
}

void PrintArrayIMP(heap_t *heap)
{
	void *start = VectorGetItemAddress(heap -> vector, 0);
	size_t i = 0;

	for (i = 0; i < HeapSize(heap); ++i)
	{
		printf("%d, ", **((int **)start + i));
	}
	printf("\n");
}

static int GenericComparisonFuncIMP(const void *new_data, 
									const void *src_data,
									void *compare_param)	
{
	heap_t *heap = compare_param;
	
	return heap -> comparison_func(*(void **)new_data, 
								 *(void **)src_data, heap -> param);
}

static size_t GetParentIndexIMP(size_t child_index)
{
	if (0 == child_index)
	{
		return 0;
	}
	
	return PARENT_INDEX(child_index);
}
