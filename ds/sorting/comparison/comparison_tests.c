#include <stdio.h> /* printf */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string.h> /* memcmp */

#include "../sorting.h"
#include "../../../chen/MyUtils.h" /* MAX2,MIN2 */

#define ARR_LENGTH (500)
#define RAND_RANGE (100)
#define NUM_OF_ITERATIONS (200)

void TestBubbleSortManyTimes();
void TestBubbleSortVsQSort();
void TestInsertionSortVsQSort();
void TestInsertionSortManyTimes();
void TestSelectionSortVsQSort();
void TestSelectionSortManyTimes();

void TestCountingSortVsQSort();
int FindMax(int *arr, size_t size);
int FindMin(int *arr, size_t size);

int SortCompare(int *arr1, int *arr2, size_t size);
int QSortCompare(const void *num1, const void *num2);

int main()
{
	TestBubbleSortVsQSort();
	TestBubbleSortManyTimes();
	TestInsertionSortVsQSort();
	TestInsertionSortManyTimes();
	TestSelectionSortVsQSort();
	TestSelectionSortManyTimes();

	return 0;
}

void TestBubbleSortManyTimes()
{
	int i = 0;
	int j = 0;
	int arr_res[ARR_LENGTH] = {0};
	int temp = 0;
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
		
		
		BubbleSort(arr_res, ARR_LENGTH);
	}
	end = clock();
	printf("TestBubbleSortManyTimes\n");
	printf("time = %f seconds\n", ((double) (end - start)) / CLOCKS_PER_SEC);
}

void TestBubbleSortVsQSort()
{
	int i = 0;
	int j = 0;
	int arr_res[ARR_LENGTH] = {0};
	int arr_exp[ARR_LENGTH] = {0};
	int temp = 0;
	int status = 0;
	srand(time(NULL));
	
	for (j = 0; j < NUM_OF_ITERATIONS; ++j)
	{
		for (i = 0; i < ARR_LENGTH; ++i)
		{
			temp = rand()%RAND_RANGE - RAND_RANGE/2;
			arr_res[i] = temp;
			arr_exp[i] = temp;
		}
		
		BubbleSort(arr_res, ARR_LENGTH);
		
		qsort(arr_exp, ARR_LENGTH, sizeof(int), QSortCompare);
		
		status |= memcmp(arr_res, arr_exp, ARR_LENGTH * sizeof(int));
	}
	
	PRINTTESTRESULTS("TestBubbleSortVsQSort",1, 0 == status);
}

void TestInsertionSortManyTimes()
{
	int i = 0;
	int j = 0;
	int arr_res[ARR_LENGTH] = {0};
	int temp = 0;
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
		
		InsertionSort(arr_res, ARR_LENGTH);;
	}
	end = clock();
	printf("TestInsertionSortManyTimes\n");
	printf("time = %f seconds\n", ((double) (end - start)) / CLOCKS_PER_SEC);
}

void TestInsertionSortVsQSort()
{
	int i = 0;
	int j = 0;
	int arr_res[ARR_LENGTH] = {0};
	int arr_exp[ARR_LENGTH] = {0};
	int temp = 0;
	int status = 0;
	srand(time(NULL));
	
	for (j = 0; j < NUM_OF_ITERATIONS; ++j)
	{
		for (i = 0; i < ARR_LENGTH; ++i)
		{
			temp = rand()%RAND_RANGE - RAND_RANGE / 2;
			arr_res[i] = temp;
			arr_exp[i] = temp;
		}
		
		InsertionSort(arr_res, ARR_LENGTH);
		
/*		for (i = 0; i< ARR_LENGTH; ++i)
		{
			printf("%d ", arr_res[i]);
		}
		printf("after insertion \n");
*/		
		qsort(arr_exp, ARR_LENGTH, sizeof(int), QSortCompare);
/*		
		for (i = 0; i< ARR_LENGTH; ++i)
		{
			printf("%d ", arr_exp[i]);
		}
		printf("qsort\n");
*/		status |= memcmp(arr_res, arr_exp, ARR_LENGTH * sizeof(int));
	}
	
	PRINTTESTRESULTS("TestInsertionSortVsQSort",1, 0 == status);
}

void TestSelectionSortManyTimes()
{
	int i = 0;
	int j = 0;
	int arr_res[ARR_LENGTH] = {0};
	int temp = 0;
	clock_t start, end;
	
	srand(time(NULL));
	
	start = clock();
	for (j = 0; j < NUM_OF_ITERATIONS; ++j)
	{
		for (i = 0; i < ARR_LENGTH; ++i)
		{
			temp = rand()%RAND_RANGE - RAND_RANGE / 2;
			arr_res[i] = temp;
		}
		
		SelectionSort(arr_res, ARR_LENGTH);;
	}
	end = clock();
	printf("TestSelectionSortManyTimes\n");
	printf("time = %f seconds\n", ((double) (end - start)) / CLOCKS_PER_SEC);
}

void TestSelectionSortVsQSort()
{
	int i = 0;
	int j = 0;
	int arr_res[ARR_LENGTH] = {0};
	int arr_exp[ARR_LENGTH] = {0};
	int temp = 0;
	int status = 0;
	srand(time(NULL));
	
	for (j = 0; j < NUM_OF_ITERATIONS; ++j)
	{
		for (i = 0; i < ARR_LENGTH; ++i)
		{
			temp = rand()%RAND_RANGE - RAND_RANGE / 2;
			arr_res[i] = temp;
			arr_exp[i] = temp;
		}
/*		
		printf("\nbefore \nqsort\n");
		for (i = 0; i< ARR_LENGTH; ++i)
		{
			printf("%d ", arr_exp[i]);
		}
		
		printf("\nselection sort\n");
		for (i = 0; i< ARR_LENGTH; ++i)
		{
			printf("%d ", arr_res[i]);
		}
*/		
		SelectionSort(arr_res, ARR_LENGTH);
		qsort(arr_exp, ARR_LENGTH, sizeof(int), QSortCompare);
		
		status |= memcmp(arr_res, arr_exp, ARR_LENGTH * sizeof(int));
	}
/*	
	printf("\nAfter \nqsort\n");
	for (i = 0; i< ARR_LENGTH; ++i)
	{
		printf("%d ", arr_exp[i]);
	}
	
	printf("\nselection sort\n");
	for (i = 0; i< ARR_LENGTH; ++i)
	{
		printf("%d ", arr_res[i]);
	}
	
*/	
	PRINTTESTRESULTS("TestSelectionSortVsQSort",1, 0 == status);
}

int SortCompare(int *arr1, int *arr2, size_t size)
{
	return (0 == memcmp(arr1, arr2, size));
}

int QSortCompare(const void *num1, const void *num2)
{
	return ((*(int *)num1) - (*(int *)num2));
}
