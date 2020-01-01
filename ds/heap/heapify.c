/************************************
*		Author: ChenR				  *
*		Reviewer: Dvir				  *
*		heapify						  *
*		1/1/2020					  *
*									  *
************************************/

#include <string.h> /* memcpy */

#include "heapify.h"

#define RIGHT_CHILD_INDEX(x) (((2) * (x)) + (2))
#define LEFT_CHILD_INDEX(x) (((2) * (x)) + (1))
#define PARENT_INDEX(x) ((x - 1) / (2))

enum should_swap_with_child
{
	SWAP_WITH_CHILD = 0,
	NO_SWAP_WITH_CHILD = 1
};

enum should_swap_with_parent
{
	NO_SWAP_WITH_PARENT = 0,
	SWAP_WITH_PARENT = 1
};

/* checks if index has a parent or not */
static int IsParentIMP(size_t size, size_t index);

/* generix swap */
static void SwapIMP(void *ptr1, void *ptr2, size_t element_size);

/* get right child index */
static size_t GetRightChildIndexIMP(size_t parent_index, size_t arr_size);

/* get left child index */
static size_t GetLeftChildIndexIMP(size_t parent_index, size_t arr_size);

/* get parent index */
static size_t GetParentIndexIMP(size_t child_index);

/* returns the higher priority child with which we need to swap or not */
static size_t HigherPriorityIndexIMP(void *arr, 
									 size_t index1,
									 size_t index2,
				 					 size_t elem_size, 
			     					 comparison_t func, 
			     					 void *compare_param);
			     					 
void HeapifyDown(void *arr, 
				 size_t arr_size, 
				 size_t elem_size, 
			     size_t index_of_heapify, 
			     comparison_t func, 
			     void *compare_param)
{
	char *array = (char *)arr;
	int compare_to_left_child = NO_SWAP_WITH_CHILD;
	int compare_to_right_child = NO_SWAP_WITH_CHILD;
	size_t left_child_index = GetLeftChildIndexIMP(index_of_heapify, 
												   arr_size);
	size_t right_child_index = GetRightChildIndexIMP(index_of_heapify, 
													 arr_size);
	size_t higher_priority_index = 0;
	void *current_data = NULL;
	void *child_data = NULL;
	

	compare_to_left_child = func(array + index_of_heapify * elem_size,
							array + left_child_index * elem_size, 
							compare_param);

	compare_to_right_child = func(array + index_of_heapify * elem_size,
							 array + right_child_index * elem_size, 
							 compare_param);
	
	higher_priority_index =  HigherPriorityIndexIMP(array,
													left_child_index, 
													right_child_index,
													elem_size,
													func,
													compare_param);
	if (left_child_index == index_of_heapify)
	{
		higher_priority_index = right_child_index;
		compare_to_left_child = NO_SWAP_WITH_CHILD;
	}
	if (right_child_index == index_of_heapify)
	{
		higher_priority_index = left_child_index;
		compare_to_right_child = NO_SWAP_WITH_CHILD;
	}
	
	current_data = array + index_of_heapify * elem_size;
	child_data = array + higher_priority_index * elem_size;
	
	if ((!IsParentIMP(arr_size, index_of_heapify) || 
		((NO_SWAP_WITH_CHILD == compare_to_left_child) && 
		 (NO_SWAP_WITH_CHILD == compare_to_right_child))))
	{
		return;
	}

	SwapIMP(current_data, child_data, elem_size);
	
	HeapifyDown(arr, 
				arr_size, 
				elem_size, 
				higher_priority_index,
				func, 
			    compare_param);
}

void HeapifyUp(void *arr, 
				size_t arr_size, 
				size_t elem_size, 
			    size_t index_of_heapify, 
			    comparison_t func, 
			    void *compare_param)
{
	char *array = (char *)arr;
	size_t parent_index = GetParentIndexIMP(index_of_heapify);
	int compare_to_parent = NO_SWAP_WITH_PARENT;
	void *current_data = NULL;
	void *parent_data = NULL;
	
	compare_to_parent = func(array + index_of_heapify * elem_size,
							 array + parent_index * elem_size, 
							 compare_param);
							 
	if ((0 == index_of_heapify) || (SWAP_WITH_PARENT != compare_to_parent))
	{
		return;
	}
	
	current_data = array + index_of_heapify * elem_size;
	parent_data = array + parent_index * elem_size;
	
	SwapIMP(current_data,parent_data, elem_size);
	HeapifyUp(arr,
			  arr_size,
			  elem_size,
			  parent_index,
			  func,
			  compare_param);
}			    

static size_t GetLeftChildIndexIMP(size_t parent_index, size_t arr_size)
{
	if (LEFT_CHILD_INDEX(parent_index) >= arr_size)
	{
		return parent_index;
	}
	
	return LEFT_CHILD_INDEX(parent_index);
}

static size_t GetRightChildIndexIMP(size_t parent_index, size_t arr_size)
{
	if (RIGHT_CHILD_INDEX(parent_index) >= arr_size)
	{
		return parent_index;
	}
	
	return RIGHT_CHILD_INDEX(parent_index);
}

static size_t GetParentIndexIMP(size_t child_index)
{
	if (0 == child_index)
	{
		return 0;
	}
	
	return PARENT_INDEX(child_index);
}

static size_t HigherPriorityIndexIMP(void *arr, 
									 size_t index1,
									 size_t index2,
				 					 size_t elem_size, 
			     					 comparison_t func, 
			     					 void *compare_param)
{
	char *array = (char *)arr;
	if (1 == func(&array[index1 * elem_size],
				  &array[index2 * elem_size], 
				  compare_param))
	{
		return index1;
	}
	else
	{
		return index2;
	}
}

static int IsParentIMP(size_t size, size_t index)
{
	return ((RIGHT_CHILD_INDEX(index) < size) ||	
			(LEFT_CHILD_INDEX(index) < size));
}		

static void SwapIMP(void *ptr1, void *ptr2, size_t element_size)
{
	void *size = (void *)alloca(element_size);
	
	memcpy(size, ptr1, element_size);
	memcpy(ptr1, ptr2, element_size);
	memcpy(ptr2, size, element_size);
}
