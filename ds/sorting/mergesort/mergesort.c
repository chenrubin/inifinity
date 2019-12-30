/************************************
*		Author: ChenR				  *
*		Reviewer: Maoz				  *
*		mergesort					  *
*		29/12/2019					  *
*									  *
************************************/
#include <stdlib.h> /*malloc*/
#include <string.h> /*memcpy*/

#include "../sorting.h"
#include "../../../chen/MyUtils.h" /* MAX2,MIN2 */

typedef enum status
{
	SUCCESS = 0,
	FAIL = 1
}status_t ;

static void MergeIMP(int *arr1, size_t size1, int *arr2, size_t size2, 
														 int *sorted_arr);

int MergeSort(int *arr, size_t size)
{
	size_t half_size = 0;
	int *temp_arr = NULL;
	static status_t status = 0;
	
	if (1 == size)
	{
		status = FAIL;
	}
	else
	{
		temp_arr = (int *)malloc(size*sizeof(int));
		if (NULL == temp_arr)
		{
			return FAIL;
		}
		half_size = size / 2;

		status |= MergeSort(arr, half_size);
		status |= MergeSort(arr + half_size, size - half_size);
		MergeIMP(arr, half_size, arr + half_size, size - half_size, 
															  temp_arr);	
	
		memcpy(arr, temp_arr, size * sizeof(int));
		free(temp_arr);
		status = SUCCESS;
	}
	
	return status;
}

static void MergeIMP(int *arr1, size_t size1, int *arr2, size_t size2, 
														 int *sorted_arr)
{
	size_t arr1_index = 0;
	size_t arr2_index = 0;
	size_t sorted_arr_index = 0;

	while ((arr1_index < size1) && (arr2_index < size2))
	{
		if (arr1[arr1_index] < arr2[arr2_index])
		{
			sorted_arr[sorted_arr_index] = arr1[arr1_index];
			++arr1_index;
		}
		else
		{
			sorted_arr[sorted_arr_index] = arr2[arr2_index];
			++arr2_index;
		}	
		++sorted_arr_index;
	}
	
	while (arr2_index < size2)
	{
		sorted_arr[sorted_arr_index] = arr2[arr2_index];
		++sorted_arr_index;
		++arr2_index;
	}
	while (arr1_index < size1)
	{
		sorted_arr[sorted_arr_index] = arr1[arr1_index];
		++sorted_arr_index;
		++arr1_index;
	}
}
