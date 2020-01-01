/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		heapsort					  *
*		7/11/2019					  *
*									  *
************************************/
#include <string.h> /* memcpy */
#include <alloca.h> /* malloc */

#include "../sorting.h"
#include "../../heap/heapify.h"
#include "../../../chen/MyUtils.h" /* MAX2,MIN2 */

#define FIRST_INDEX 0

 /* wrapper in order to trick into currct sorting order */
static int IsBeforeWrapperIMP(const void *new_data, 
			  	 			  const void *src_data, 
			   				  void *param);
			   				  
/* generic swap */			   				  
static void SwapIMP(void *ptr1, void *ptr2, size_t element_size);


typedef struct is_before_wrapper
{
	is_before_t func;
	void *is_before_param;
}wrapper_t;

void HeapSort(void *arr, 
			  size_t arr_size, 
			  size_t element_size, 
			  is_before_t func, 
			  void *param)
{
	int index_to_heapify = 0;
	char *first_element = arr;
	char *last_element = NULL;
	
	wrapper_t *my_wrapper = (wrapper_t *)alloca(sizeof(wrapper_t));
	my_wrapper -> func = func;
	my_wrapper -> is_before_param = param;
	
	for (index_to_heapify = (arr_size / 2) - 1; 
		 index_to_heapify >= 0; 
		 --index_to_heapify)
	{
		HeapifyDown(arr, 
					arr_size, 
					element_size, 
					index_to_heapify, 
					IsBeforeWrapperIMP, 
					my_wrapper);
	}
		
	while (arr_size > 0)
	{
		last_element = (char *)arr + ((arr_size - 1 ) * element_size);
		SwapIMP(first_element, last_element, element_size);
		--arr_size;
		HeapifyDown(arr, 
					arr_size, 
					element_size, 
					FIRST_INDEX, 
					IsBeforeWrapperIMP, 
					my_wrapper);
	}
}

static int IsBeforeWrapperIMP(const void *new_data, 
			  	 			  const void *src_data, 
			   				  void *param)
{
	int res = ((wrapper_t *)param) -> func(new_data, 
										 src_data, 
										 ((wrapper_t *)param) -> 
										   is_before_param);
	
	return (!res);
}			   

static void SwapIMP(void *ptr1, void *ptr2, size_t element_size)
{
	void *size = (void *)alloca(element_size);
	
	memcpy(size, ptr1, element_size);
	memcpy(ptr1, ptr2, element_size);
	memcpy(ptr2, size, element_size);
}

