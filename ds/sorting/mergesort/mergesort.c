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

static void MergeIMP(int *arr1, size_t size1, int *arr2, size_t size2, 
														 int *arr3);

int MergeSort(int *arr, size_t size)
{
	size_t half_size = 0;
	int *temp_arr = NULL;
	static int status = 0;
	
	if (1 == size)
	{
		return 0;
	}
	else
	{
		temp_arr = (int *)malloc(size*sizeof(int));
		if (NULL == temp_arr)
		{
			return 1;
		}
		half_size = size / 2;

		status |= MergeSort(arr, half_size);
		status |= MergeSort(arr + half_size, size - half_size);
		MergeIMP(arr, half_size, arr + half_size, size - half_size, 
															  temp_arr);	
	
		memcpy (arr, temp_arr, size * sizeof(int));
		free(temp_arr);		
	}
	
	return 0;
}

static void MergeIMP(int *arr1, size_t size1, int *arr2, size_t size2, 
														 int *arr3)
{
	int *runner1 = arr1;
	int *runner2 = arr2;
	int i = 0;
	
	while (((size_t)(runner1 - arr1) < size1) && ((size_t)(runner2 - arr2) < size2))
	{
		if (*runner1 < *runner2)
		{
			arr3[i] = *runner1;
			++runner1;
		}
		else
		{
			arr3[i] = *runner2;
			++runner2;
		}
		
		++i;
	}
	
	if ((size_t)(runner1 - arr1) == size1)
	{
		while ((size_t)(runner2 - arr2) != size2)
		{
			arr3[i] = *runner2;
			++i;
			++runner2;
		}
	}
	else
	{
		while ((size_t)(runner1 - arr1) != size1)
		{
			arr3[i] = *runner1;
			++i;
			++runner1;
		}
	}
}
