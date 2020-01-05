#include <stdio.h> 		/* printf */
#include <stdlib.h>     /* srand, rand */
#include <time.h>     	/* time */
#include <string.h>     /* memcmp */
#include <unistd.h>		/* sleep */

#include "../sorting.h"
#include "../../../chen/MyUtils.h" /* MAX2,MIN2 */

#define ARR_LENGTH 100
#define NUM_OF_ITERATIONS 30
#define RAND_RANGE 2000

void TestQuickSort();

/* qsort compare function */
int QSortCompare(const void *num1, const void *num2);

/* is before function */	
int MyCompareFunc(const void *new_data, 
			   const void *src_data, 
			   void *param);

int main()
{
	TestQuickSort();

	return 0;
}
/*
display *(int *)arr@4
display small_runner_index
display big_runner_index 
display pivot_index
display *(int *)((char *)arr + (big_runner_index * element_size))
display *(int *)((char *)arr + (small_runner_index * element_size))
display *(int *)((char *)arr + (pivot_index * element_size))
*/
void TestQuickSort()
{
	int i = 0;
	int j = 0;
	int arr_res[ARR_LENGTH] = {0};
	int arr_exp[ARR_LENGTH] = {0};
	int temp = 0;
	int status = 0;
	int param = 0;
	srand(time(NULL));
	
	for (j = 0; j < NUM_OF_ITERATIONS; ++j)
	{
		sleep(1);
		for (i = 0; i < ARR_LENGTH; ++i)
		{
			temp = rand()%RAND_RANGE - RAND_RANGE / 2;
			arr_res[i] = temp;
			arr_exp[i] = temp;
		}
		
/*		printf("\nbefore qsort\n");
		for (i = 0; i< ARR_LENGTH; ++i)
		{
			printf("%d ", arr_exp[i]);
		}
		
		printf("\nbefore My quick sort\n");
		for (i = 0; i< ARR_LENGTH; ++i)
		{
			printf("%d ", arr_res[i]);
		}
		printf("\n");
		QuickSort(arr_res, ARR_LENGTH, sizeof(int), QSortCompare);
		qsort(arr_exp, ARR_LENGTH, sizeof(int), QSortCompare);
*/		
/*		PRINTTESTRESULTS("TestQuickSortVsQSort",1, 0 == memcmp(arr_res, arr_exp, ARR_LENGTH));*/
		status |= memcmp(arr_res, arr_exp, ARR_LENGTH * sizeof(int));
/*		
		printf("\nAfter \nqsort\n");
		for (i = 0; i< ARR_LENGTH; ++i)
		{
			printf("%d ", arr_exp[i]);
		}
		
		printf("\nquick sort\n");
		for (i = 0; i< ARR_LENGTH; ++i)
		{
			printf("%d ", arr_res[i]);
		}
		printf("\n");
*/	}
	
	PRINTTESTRESULTS("TestQuickSortVsQSort",1, 0 == status);
}

int QSortCompare(const void *num1, const void *num2)
{
	return ((*(int *)num1) - (*(int *)num2));
}

int MyCompareFunc(const void *new_data, 
			   const void *src_data, 
			   void *param)
{
	UNUSED(param);
	
	return ((*(int *)new_data) - (*(int *)src_data));
}						   
