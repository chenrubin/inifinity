#include <stdio.h> /* printf */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string.h> /* memcmp */

#include "../sorting.h"
#include "../../../chen/MyUtils.h" /* MAX2,MIN2 */

#define ARR_LENGTH (10000)
#define RAND_RANGE (10000)
#define NUM_OF_ITERATIONS (10000)

void TestCountingSortVsQSort();
void TestCountingSortManyTimes();
void TestRadixSortVsQSort();
void TestRadixSortManyTimes();

int FindMax(int *arr, size_t size);
int FindMin(int *arr, size_t size);

int SortCompare(int *arr1, int *arr2, size_t size);
int QSortCompare(const void *num1, const void *num2);

int main()
{
	TestCountingSortVsQSort();
	TestCountingSortManyTimes();
	TestRadixSortVsQSort();	
	TestRadixSortManyTimes();

	return 0;
}

void TestCountingSortVsQSort()
{
	int i = 0;
	int j = 0;
	int orig_array[ARR_LENGTH];
	int arr_res[ARR_LENGTH];
	int arr_exp[ARR_LENGTH];
	int temp = 0;
	int status = 0;
	int min = 0;
	int max = 0;
	srand(time(NULL));
	
	for (j = 0; j < NUM_OF_ITERATIONS; ++j)
	{
		for (i = 0; i < ARR_LENGTH; ++i)
		{			
			temp = rand()%RAND_RANGE - RAND_RANGE/2;
			orig_array[i] = temp;
			arr_exp[i] = temp;
		}
		
		max = FindMax(orig_array, ARR_LENGTH);
		min = FindMin(orig_array, ARR_LENGTH);
		
		CountingSort(orig_array,ARR_LENGTH,min,max, arr_res);
		qsort(arr_exp, ARR_LENGTH, sizeof(int), QSortCompare);
		status |= memcmp(arr_res, arr_exp, ARR_LENGTH * sizeof(int));
	}
	
	PRINTTESTRESULTS("TestCountingSortVsQSort",1, 0 == status);
}

void TestCountingSortManyTimes()
{
	int i = 0;
	int j = 0;
	int orig_array[ARR_LENGTH] = {0};
	int arr_res[ARR_LENGTH] = {0};
	int temp = 0;
	int min = 0;
	int max = 0;
	clock_t start, end;
	
	srand(time(NULL));
    
	
	start = clock();
	for (j = 0; j < NUM_OF_ITERATIONS; ++j)
	{
		for (i = 0; i < ARR_LENGTH; ++i)
		{
			temp = rand()%RAND_RANGE - RAND_RANGE/2;
			arr_res[i] = temp;
		}
		
		
		CountingSort(orig_array,ARR_LENGTH,min,max, arr_res);
	}
	end = clock();
	printf("TestCountingSortManyTimes\n");
	printf("time = %f seconds\n", ((double) (end - start)) / CLOCKS_PER_SEC);
}

void TestRadixSortVsQSort()
{
	int i = 0;
	int j = 0;
	int radix_array[ARR_LENGTH];
	int arr_exp[ARR_LENGTH];
	int temp = 0;
	int status = 0;
	srand(time(NULL));
	
	for (j = 0; j < NUM_OF_ITERATIONS; ++j)
	{
		for (i = 0; i < ARR_LENGTH; ++i)
		{			
			temp = rand()%RAND_RANGE - RAND_RANGE/2;
			radix_array[i] = temp;
			arr_exp[i] = temp;
		}
		
/*		for (i = 0; i < ARR_LENGTH; ++i)
		{
			printf("%d ", radix_array[i]);
		}
		printf("original array\n");
*/		
		RadixSort(radix_array,ARR_LENGTH);
/*		for (i = 0; i < ARR_LENGTH; ++i)
		{
			printf("%d ", radix_array[i]);
		}
		printf("after radix\n");
*/		qsort(arr_exp, ARR_LENGTH, sizeof(int), QSortCompare);
/*		for (i = 0; i < ARR_LENGTH; ++i)
		{
			printf("%d ", arr_exp[i]);
		}
		printf("after qsort\n");
		printf("memcmp = %d\n", memcmp(radix_array, arr_exp, ARR_LENGTH * sizeof(int)));
*/		status |= memcmp(radix_array, arr_exp, ARR_LENGTH * sizeof(int));
	}
	
	PRINTTESTRESULTS("TestRadixSortVsQSort",1, 0 == status);
}

void TestRadixSortManyTimes()
{
	int i = 0;
	int j = 0;
	int orig_array[ARR_LENGTH] = {0};
	int temp = 0;
	clock_t start, end;
	
	srand(time(NULL));
    
	
	start = clock();
	for (j = 0; j < NUM_OF_ITERATIONS; ++j)
	{
		for (i = 0; i < ARR_LENGTH; ++i)
		{
			temp = rand()%RAND_RANGE - RAND_RANGE/2;
			orig_array[i] = temp;
		}
		
		
		RadixSort(orig_array,ARR_LENGTH);
	}
	end = clock();
	printf("TestRadixSortManyTimes\n");
	printf("time = %f seconds\n", ((double) (end - start)) / CLOCKS_PER_SEC);
}

int FindMax(int *arr, size_t size)
{
	int i = 0;
	int max = arr[0];
	
	for (i = 0; i < size; ++i)
	{
		if (max < arr[i])
		{
			max = arr[i];
		}
	}
	
	return max;
}

int FindMin(int *arr, size_t size)
{
	int i = 0;
	int min = arr[0];
	
	for (i = 0; i < size; ++i)
	{
		if (min > arr[i])
		{
			min = arr[i];
		}
	}
	
	return min;
}

int SortCompare(int *arr1, int *arr2, size_t size)
{
	return (0 == memcmp(arr1, arr2, size));
}

int QSortCompare(const void *num1, const void *num2)
{
	return ((*(int *)num1) - (*(int *)num2));
}
