/****************************************************************************
                                                                           
			                      sorting	                        
                               OL79 - 11/12/19                              
                                                                           
                             Writer:    Gal Salemon                       
                             Reviewer:  Chen                             
*****************************************************************************/
#include <stdlib.h> /* size_t */
#include <assert.h> /* assert */
#include <string.h> /* memset, memcpy */

#include "../sorting.h"
#include "../myutils.h"

#define BASE (10)
#define HIS_SIZE (19)
#define RADIX_MIN (-9)

static int FindMaxIMP(int *arr, size_t arr_size);
static int FindMinIMP(int *arr, size_t arr_size);
static size_t GetBucket(void *element, void *param);
static size_t GetRadixBucketIMP(void *element, void *param);
static int GetNumOfDigitsIMP(int max_value, int min_value);
static void *PromotePointerIMP(const void *arr, int index, size_t element_size);
static void *GetValueFromArrayIMP(const void *arr_to_sort, int index, 
							   size_t element_size);
static void InsertValueToResultArrayIMP(void *result_arr, int index_for_arr, 
										size_t element_size, void *value);
static int GetIndexFromHistogramIMP(const void *arr_to_sort, int index, 
									size_t element_size, void *param,
									size_t (*GetBucket)(void *element, 
									void *param));
static int GetIndexForArrayInsertIMP(unsigned int *histogram, int index);
static void DecreaseHistogramIndexIMP(unsigned int *histogram, int index);
static void SumHistogramIMP(unsigned int *histogram, int his_len);
static void SetHistogramIMP(const void *arr_to_sort, int arr_len, 
							size_t element_size, unsigned int *histogram, 
							size_t (*GetBucket)(void *element, void *param), 
							void *param);
static void SetResultArrayIMP(const void *arr_to_sort, void *result_arr, 
							  size_t arr_len, size_t element_size, 
							  unsigned int *histogram, void *param, 
 							  size_t (*GetBucket)(void *element, void *param));
static void InsertSortedElementsToResultArrayIMP(int *arr, int *result_arr, 
											  size_t arr_size, 
											  size_t element_size, 
											  unsigned int *histogram, 
											  int digit);

 							  
int CountingSort(const int *arr, size_t arr_size, int min_val, int max_val,
				 int *result_arr)
{
	int param = min_val;
	size_t element_size = sizeof(int);
	size_t his_size = max_val - min_val + 1;	
	unsigned int *histogram = 
				 		(unsigned int*)calloc(his_size, sizeof(unsigned int));
	if (NULL == histogram)
	{
		return 1;
	}
	
	assert(arr);
	assert(result_arr);

	CountingSortIMP(arr, element_size, arr_size, histogram, his_size,
					GetBucket, &param, result_arr);
	free(histogram);
						
	return 0;
}

int RadixSort(int *arr, size_t arr_size)
{
	int digit = 0;	
	unsigned int *histogram = NULL;
	size_t element_size = sizeof(int);
	int max_value = FindMaxIMP(arr, arr_size);
	int min_value = FindMinIMP(arr, arr_size);
	int num_of_digits = GetNumOfDigitsIMP(max_value, min_value);
	int *result_arr = (int*)malloc(arr_size * element_size);
	if (NULL == result_arr)
	{
		return 1;
	}
	
	histogram = (unsigned int*)calloc(HIS_SIZE, sizeof(unsigned int));
	if (NULL == histogram)
	{
		free(result_arr);
		
		return 1;
	}
	
	assert(arr);	
	 	
	for (digit = 1; digit <= num_of_digits; ++digit)
	{	
		InsertSortedElementsToResultArrayIMP
		(arr, result_arr, arr_size, element_size, histogram, digit);
	}
	
	free(histogram);
	free(result_arr);			
					
	return 0;
}

static void InsertSortedElementsToResultArrayIMP(int *arr, int *result_arr, 
											  size_t arr_size, 
											  size_t element_size, 
											  unsigned int *histogram, 
											  int digit)
{
	CountingSortIMP(arr, element_size, arr_size, histogram, HIS_SIZE, 
					GetRadixBucketIMP, &digit, result_arr);
	memset(histogram, 0, HIS_SIZE * element_size);
	memcpy(arr, result_arr, arr_size * element_size);

}

static size_t GetRadixBucketIMP(void *element, void *param)
{
	int i = 0;
	int digit = 0;
	int value = *((int*)element);
	int how_many_loops = *((int*)param);
	
	for (i = 0; i < how_many_loops; ++i)
	{
		digit = value % BASE;
		value = value / BASE;
	}

	return (digit - (RADIX_MIN));
}

void CountingSortIMP(const void *arr_to_sort, size_t element_size, 
					 size_t arr_len, unsigned int *histogram, size_t his_len,
					 size_t (*GetBucket)(void *element, void *param),
					 void *param, void *result_arr)					 
{
	assert(GetBucket);
	assert(arr_to_sort);
	assert(result_arr);
	assert(histogram);	
	
	SetHistogramIMP(arr_to_sort, arr_len, element_size, 
					histogram, GetBucket, param);
	
	SumHistogramIMP(histogram, his_len);

	SetResultArrayIMP(arr_to_sort, result_arr, arr_len, element_size, 
					  histogram, param, GetBucket);
}

static void SetResultArrayIMP(const void *arr_to_sort, void *result_arr, 
							  size_t arr_len, size_t element_size, 
							  unsigned int *histogram, void *param, 
							  size_t (*GetBucket)(void *element, void *param))
{
	int i = 0;
	void *value = NULL;
	int index_for_arr = 0;
	int index_from_histogram = 0;

	for (i = arr_len - 1; i >= 0; --i)
	{					  
		index_from_histogram = GetIndexFromHistogramIMP(arr_to_sort, i, 
													 element_size, param, 
													 GetBucket);
		index_for_arr = GetIndexForArrayInsertIMP(histogram, index_from_histogram);		
		DecreaseHistogramIndexIMP(histogram, index_from_histogram);
		value = GetValueFromArrayIMP(arr_to_sort, i, element_size);		
		InsertValueToResultArrayIMP(result_arr, index_for_arr, 
									element_size, value);
	}
}

static void SetHistogramIMP(const void *arr_to_sort, int arr_len, 
							size_t element_size, unsigned int *histogram, 
							size_t (*GetBucket)(void *element, void *param), 
							void *param)
{
	int i = 0;
	int index_for_arr_insert = 0;
	
	for (i = 0; i < arr_len; ++i)
	{
		index_for_arr_insert = GetBucket(PromotePointerIMP
							  (arr_to_sort, i, element_size), param);
		histogram[index_for_arr_insert] += 1; 
	}
}

static void SumHistogramIMP(unsigned int *histogram, int his_len)
{
	int i = 0;
	int sum = 0;
	
	for (i = 0; i < his_len; ++i)
	{		
		sum += histogram[i];
		histogram[i] = sum;
	}
}

static void DecreaseHistogramIndexIMP(unsigned int *histogram, int index)
{
	--histogram[index];
}

static int GetIndexForArrayInsertIMP(unsigned int *histogram, int index)
{
	return (histogram[index] - 1);
}

static int GetIndexFromHistogramIMP(const void *arr_to_sort, int index, 
								 size_t element_size, void *param,
								 size_t (*GetBucket)(void *element, void *param))
{
	return GetBucket(PromotePointerIMP(arr_to_sort, index, element_size), param);
}

static void *GetValueFromArrayIMP(const void *arr_to_sort, int index, 
							   size_t element_size)
{
	void *value = PromotePointerIMP(arr_to_sort, index, element_size);
	
	return value;
}

static void InsertValueToResultArrayIMP(void *result_arr, int index_for_arr, 
										size_t element_size, void *value)
{
	memcpy(PromotePointerIMP(result_arr, index_for_arr, element_size), 
		   value, element_size);	

}

static void *PromotePointerIMP(const void *arr, int index, size_t element_size)
{
	return (char*)arr + (index * element_size);
}

static size_t GetBucket(void *element, void *param)
{
	return *((int*)element) - *((int*)param);
}

static int GetNumOfDigitsIMP(int max_value, int min_value)
{
	int min_loops = 0;
	int max_loops = 0;
	
	while (max_value != 0)
	{
		max_value /= BASE;
		++max_loops; 
	}
	
	while (min_value != 0)
	{
		min_value /= BASE;
		++min_loops; 
	}

	return MAX2(min_loops, max_loops);
}

static int FindMaxIMP(int *arr, size_t arr_size)
{	
	int len = arr_size;
	int max = 0;
	int i = 0;
	
	for (i = 0; i < len; ++i)
	{
		if (arr[i] > max)
		{
			max = arr[i];
		}
	}
	 
	return max;
}

static int FindMinIMP(int *arr, size_t arr_size)
{
	int len = arr_size;
	int min = 0;
	int i = 0;
	
	for (i = 0; i < len; ++i)
	{
		if (arr[i] < min)
		{
			min = arr[i];
		}
	}
	 
	return min;
}

