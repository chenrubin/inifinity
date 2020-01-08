/************************************
*		Author: ChenR				  *
*		Reviewer: EyalF				  *
*		searching					  *
*		7/12/2020					  *
*									  *
************************************/

#include <math.h> /* sqrt */

#include "searching.h"
#include "../../../chen/MyUtils.h" /* MAX2,MIN2 */

static int *SearchEachElementIMP(const int *arr, size_t size, int val);

int *BSearch(const int *arr, size_t size, int val)
{
	int *arr1 = (int *)arr;
	int middle = arr1[size/2];
	
	if (val == middle)
	{
		return (arr1 + size / 2);
	}
	if (1 == size)
	{
		return NULL;
	}
	if (val < middle)
	{
		return (BSearch(arr1, size/2, val));
	}
	
	return (BSearch(arr1 + size / 2, size - size/2, val));
}

int *JSearch(const int *arr, size_t size, int val)
{
	size_t block_size = (size_t)sqrt(size);
	size_t i = block_size;
	int *res = NULL;
	int *start = NULL;
	
	for (i = block_size; i < size; i = i + block_size)
	{
		if (arr[i] >= val)
		{
			start = (int *)arr + i - block_size;
			res = SearchEachElementIMP(start, block_size, val);
			if (NULL != res)
			{
				return res;
			}
		}	
	}
	i = i - block_size;
	if (arr[i] <= val)
	{
		res = SearchEachElementIMP(arr + i, size - i, val);
	}
	
	return res;
}

static int *SearchEachElementIMP(const int *arr, size_t size, int val)
{
	size_t i = 0;
	int *arr1 = (int *)arr;
	
	for (i =0; i < size; ++i)
	{
		if (val == arr1[i])
		{
			return (arr1 + i);
		}
	}
	
	return NULL;
}
