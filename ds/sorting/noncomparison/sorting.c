/************************************
*		Author: ChenR				  *
*		Reviewer: Sharon			  *
*		sort						  *
*		10/12/2019					  *
*									  *
************************************/

#include <stdio.h> /* printf */
#include <assert.h> /* assert */
#include <stdlib.h> /* assert */
#include <string.h> /* memcpy */

#include "../sorting.h"
#include "../../../chen/MyUtils.h" /* MAX2,MIN2 */


static size_t IntGetBucketIMP(const void *element, const void *param);
void CountingSortIMP(const void *arr_to_sort,
					 size_t element_size,
					 size_t arr_size,
					 unsigned int *histogram,
					 size_t his_size,
					 size_t (*GetBucket)(const void *element,const void *param),
					 void *param,
					 void *result);

static size_t IntGetBucketIMP(const void *element, const void *param)
{
	return *(int *)element - *(int *)param;
}

int CountingSort(const int *arr,
				 size_t arr_size,
				 int min_val,
				 int max_val,
				 int *result)
{
	int his_size = max_val - min_val + 1;
	unsigned int *histogram = 
				 (unsigned int *)calloc(his_size,sizeof(int));
	if (NULL == histogram)
	{
		return 1;
	}
	
	CountingSortIMP(arr, sizeof(int), arr_size, histogram, 
					his_size, IntGetBucketIMP, &min_val, result);
	free(histogram);				
					
	return 0;				
}				 
				 
void CountingSortIMP(const void *arr_to_sort,
					 size_t element_size,
					 size_t arr_size,
					 unsigned int *histogram,
					 size_t his_size,
					 size_t (*GetBucket)(const void *element,const void *param),
					 void *param,
					 void *result)
{
	int i = 0;
	int index;
	char *sort_runner = (char *)arr_to_sort;
	char *result_runner = (char *)result;
	
	for (i = 0; i < (int)arr_size; ++i)
	{
		++histogram[GetBucket(sort_runner + i * element_size, param)];
	}
	
	for (i = 1; i < (int)his_size; ++i)
	{
		histogram[i] += histogram[i - 1];
	}
	
	/* put in result */
	for (i = (int)arr_size - 1; i >= 0 ; --i)
	{
		index = GetBucket(sort_runner + i * element_size, param);
		histogram[index] -= 1;
		memcpy(result_runner + (histogram[index]) * element_size, 
			   sort_runner + i * element_size, element_size);
	}
}					 
