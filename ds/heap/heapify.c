#include "heapify.h"

#define RIGHT_CHILD_INDEX(x) (((2) * (x)) + (2))
#define LEFT_CHILD_INDEX(x) (((2) * (x)) + (1))
#define PARENT_INDEX(x) ((x - 1) / (2))

enum num_of_children
{
	NON = 0,
	ONE = 1,
	TWO = 2
};

static void SwapIMP(void **ptr1, void **ptr2);
static size_t GetRightChildIndexIMP(size_t parent_index, size_t arr_size);
static size_t GetLeftChildIndexIMP(size_t parent_index, size_t arr_size);
static size_t GetParentIndexIMP(size_t child_index);
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
	size_t left_child_index = GetLeftChildIndexIMP(index_of_heapify, arr_size);
	size_t right_child_index = GetRightChildIndexIMP(index_of_heapify, arr_size);
	int compare_to_left_child = func(&array[index_of_heapify * elem_size],
								  	 &array[left_child_index * elem_size], 
								  	 compare_param);
	int compare_to_right_child = func(&array[index_of_heapify * elem_size],
								   	  &array[right_child_index * elem_size], 
								   	  compare_param);
	size_t higher_priority_index =  HigherPriorityIndexIMP(array,
														   left_child_index, 
														   right_child_index,
														   elem_size,
														   func,
														   compare_param);
	
	if ((1 == compare_to_left_child) &&  (1 == compare_to_right_child))
	{
		return;
	}
	
	SwapIMP((void *)(array + index_of_heapify * elem_size), 
			(void *)(array + higher_priority_index * elem_size));
	
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
	int compare_to_parent = 0;
	void *temp_index_to_heapify = (array + index_of_heapify * elem_size);
	void *temp_parent_index = (array + parent_index * elem_size);
	
	if (0 == index_of_heapify)
	{
		return;
	}

	compare_to_parent = func(&array[index_of_heapify * elem_size],
							 &array[parent_index * elem_size], 
							 compare_param);
	if (1 == compare_to_parent)
	{
		SwapIMP(&temp_index_to_heapify,&temp_parent_index);
	}
	
	HeapifyUp(arr,
			  arr_size,
			  elem_size,
			  parent_index * elem_size,
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
	if (0 == PARENT_INDEX(child_index))
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

static void SwapIMP(void **ptr1, void **ptr2)
{
	void *temp_address = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = temp_address;
}
