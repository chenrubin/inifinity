/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		searching					  *
*		7/11/2019					  *
*									  *
************************************/

#include "searching.h"

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
	
	return (BSearch(arr1 + size / 2, size/2, val));
}

