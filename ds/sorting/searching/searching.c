/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		searching					  *
*		7/11/2019					  *
*									  *
************************************/

#include <math.h> /* sqrt */

#include "searching.h"
#include "../../../chen/MyUtils.h" /* MAX2,MIN2 */

int *SearchEachElementIMP(const int *arr, size_t size, int val);

int *BSearch(const int *arr, size_t size, int val)
{
	int *arr1 = (int *)arr;
	int temp = arr1[size/2];
	
	if (val == temp)
	{
		return (arr1 + size / 2);
	}
	if (1 == size)
	{
		return NULL;
	}
	if (val < temp)
	{
		return (BSearch(arr1, size/2, val));
	}
	
	return (BSearch(arr1 + size / 2, size - size/2, val));
}

int *JSearch(const int *arr, size_t size, int val)
{
	size_t size_sqrt = (size_t)sqrt(size);
	size_t i = size_sqrt;
	int *res = NULL;
	
	for (i = size_sqrt; i < size; i = i + size_sqrt)
	{
		if (arr[i] >= val)
		{
			res = SearchEachElementIMP(arr + i - size_sqrt, size_sqrt, val);
			if (NULL != res)
			{
				return res;
			}
		}	
	}
	i = i - size_sqrt;
	res = SearchEachElementIMP(arr + i, size - i, val);
	
	return res;
}

int *SearchEachElementIMP(const int *arr, size_t size, int val)
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
