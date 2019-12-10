/************************************
*		Author: ChenR				  *
*		Reviewer: Sharon			  *
*		sort						  *
*		10/12/2019					  *
*									  *
************************************/

#include <stdio.h> /* printf */
#include <assert.h> /* assert */

#include "../sorting.h"
#include "../../../chen/MyUtils.h" /* MAX2,MIN2 */

/* swap twp integers */
static void SwapIntIMP(int *num1, int *num2);

/* return 1 if num1 is bigger than num2 */
static int IsBiggerIMP(int num1, int num2);

/* moves all numbers beteen index1 and index2 (inclusive) one place 
   to the right and copies what's in index2 to index1 place */ 
static void ShiftNumbersToRightIMP(int *arr, int index1, int index2);

/* find the index of the min between index1 and index2 includive */
static int FindMinIndexIMP(int *arr, int index1, int index2);

/* checks if index == -1 meaning it passed the beginnig of the array */
static int IsInBeginningIMP(int index);

void BubbleSort(int *arr, size_t size)
{
	size_t i = 0;
	size_t j = 0;
	int is_swap = 0;
	
	assert(arr);
	assert(size);
	
	for (i = size - 1; i > 0; --i)
	{
		is_swap = 0;
		
		for (j = 0; j < i; ++j)
		{
			if (arr[j] > arr[j + 1])
			{
				SwapIntIMP(&arr[j], &arr[j + 1]);
				is_swap = 1;
			}
		}
		
		if (!is_swap)
		{
			break;
		}
	}
}

void InsertionSort(int *arr, size_t size)
{
	size_t i = 0;
	int j = 0;
	
	assert(arr);
	assert(size);
	
	for (i = 1; i < size; ++i)
	{
		j = i - 1;
		while (j >= 0)
		{
			if (IsBiggerIMP(arr[i], arr[j]))
			{
				ShiftNumbersToRightIMP(arr, j + 1, i);
				break;
			}
			
			--j;
		}
		
		if (IsInBeginningIMP(j))
		{
			ShiftNumbersToRightIMP(arr, j + 1, i);
		}
	}
}

void SelectionSort(int *arr, size_t size)
{
	size_t i = 0;
	
	assert(arr);
	assert(size);
	
	for (i = 0; i < size - 1; ++i)
	{
		SwapIntIMP(&arr[FindMinIndexIMP(arr, i, size - 1)], &arr[i]);
	}
}

static int FindMinIndexIMP(int *arr, int index1, int index2)
{
	int i = index1;
	int min = arr[index1];
	int res = index1;
	
	for (i = index1; i <= index2; ++i)
	{
		if (min > arr[i])
		{
			min = arr[i];
			res = i;
		}
	}

	return res;
}

static void ShiftNumbersToRightIMP(int *arr, int index1, int index2)
{
	int i = 0;
	int temp_num = arr[index2];
	
	for (i = index2; i > index1; --i)
	{
		arr[i] = arr[i - 1];
	}
	
	arr[index1] = temp_num;
}

static int IsBiggerIMP(int num1, int num2)
{
	return (num1 >= num2);
}

static int IsInBeginningIMP(int index)
{
	return (0 > index);
}

static void SwapIntIMP(int *num1, int *num2)
{
	int temp = *num1;
	
	*num1 = *num2;
	*num2 = temp;
}
