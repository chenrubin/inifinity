/************************************
*		Author: ChenR				  *
*		Reviewer: Sharon			  *
*		sort						  *
*		11/12/2019					  *
*									  *
************************************/

#include <stdio.h> /* printf */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */
#include <math.h> /* abs */

#include "../sorting.h"
#include "../../../chen/MyUtils.h" /* MAX2,MIN2 */

#define HISTOGRAM_SIZE 19
#define BASE 10
#define RADIX_MIN (-9)

/* gets the index of bucket in countingSort according to element */
static size_t IntGetBucketIMP(const void *element, const void *param);

/* finds number of iterations in which to sort radix until it's finished */
static int FindNumOfIntertionsIMP(int *arr, size_t arr_size);

/* gets the index of bucket in radixSort according to element */
static size_t RadixBucketIMP(const void *element, const void *param);

/* find highest ordered numbers regardiles of its sign */
static int FindHighestOrderedNumberIMP(int *arr, size_t arr_size);

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

int RadixSort(int *arr, size_t arr_size)
{
	int i = 0;
	int num_of_sorting_iterations = FindNumOfIntertionsIMP(arr, arr_size);
	int *intermediate_arr = NULL;
	unsigned int *histogram = NULL;
	
	intermediate_arr = (int *)malloc(arr_size * (sizeof(int)));
	if (NULL == intermediate_arr)
	{
		return 1;
	}
	histogram = (unsigned int *)malloc(HISTOGRAM_SIZE * (sizeof(int)));
	if (NULL == histogram)
	{
		return 1;
	}
	
	for (i = 0; i < num_of_sorting_iterations; ++i)
	{
		memset(histogram, 0, HISTOGRAM_SIZE * sizeof(int));
		CountingSortIMP(arr, sizeof(int), arr_size, histogram, HISTOGRAM_SIZE,
					 RadixBucketIMP, &i, intermediate_arr);
		memcpy (arr, intermediate_arr, arr_size * sizeof(int)); 			 
	}
	free(intermediate_arr);
	
	return 0;
}

static int FindNumOfIntertionsIMP(int *arr, size_t arr_size)
{
	int max = 0;
	size_t num_of_iterations = 0;
	
	max = FindHighestOrderedNumberIMP(arr, arr_size);	

	while (0 != max)
	{
		num_of_iterations += 1;
		max = max / BASE;
	}
	
	return num_of_iterations;
}

static int FindHighestOrderedNumberIMP(int *arr, size_t arr_size)
{
	size_t i = 0;
	int max = 0;
	
	for (i = 0; i < arr_size; ++i)
	{
		if (max < abs(arr[i]))
		{
			max = abs(arr[i]);
		}
	}
	
	return max;
}

static size_t RadixBucketIMP(const void *element, const void *param)
{
	int i = 0;
	int res = *(int *)element;
	
	for (i = 0; i < *(int *)param; ++i)
	{
		res /= BASE;
	}
	
	return (res % BASE) - RADIX_MIN;
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
