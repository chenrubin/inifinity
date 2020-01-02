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
#define ARR_ELEMENT(x) ((char *)arr + (x * element_size))

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

void QuickSort(void *arr, 
			   size_t arr_size, 
			   size_t element_size, 
			   int (*compar)(const void *, const void*))
{
	size_t pivot_index = 0;
	size_t big_runner_index = 0;
	size_t small_runner_index = 0;
	
	if (arr_size <= 1)
	{
		return;
	}
	
	pivot_index = arr_size - 1;
	small_runner_index = pivot_index - 1;
	printf("pivot_index = %ld\n", pivot_index);
	printf("big_runner_index = %ld\n", big_runner_index);
	printf("small_runner_index = %ld\n", small_runner_index);
	while (big_runner_index != small_runner_index)
	{
		while ((0 < compar(ARR_ELEMENT(big_runner_index), 
						   ARR_ELEMENT(pivot_index))) &&
			   			   (big_runner_index != small_runner_index))
		{
			printf("inside first small while\n");
			++big_runner_index;
		}
		while ((0 >= compar(ARR_ELEMENT(small_runner_index), 
						   ARR_ELEMENT(pivot_index))) &&
			   			   (big_runner_index != small_runner_index))
		{
			printf("inside second small while\n");
			--small_runner_index;
		}
		printf("swap inside loop\n");
		SwapIMP(ARR_ELEMENT(big_runner_index), 
				ARR_ELEMENT(small_runner_index), 
				element_size);
	}
	printf("swap outside loop\n");
	SwapIMP(ARR_ELEMENT(big_runner_index), 
						ARR_ELEMENT(pivot_index), 
						element_size);
	QuickSort(arr, 
			  big_runner_index, 
			  element_size, 
			  compar);
	QuickSort(ARR_ELEMENT(big_runner_index + 1), 
			  arr_size - big_runner_index - 1, 
			  element_size, 
			  compar);		   
	
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
	printf("inside swap of %d and %d\n", *(int *)ptr1, *(int *)ptr2);
	memcpy(size, ptr1, element_size);
	memcpy(ptr1, ptr2, element_size);
	memcpy(ptr2, size, element_size);
}

