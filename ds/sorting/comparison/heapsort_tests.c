#include <stdio.h> 		/* printf */
#include <stdlib.h>     /* srand, rand */
#include <time.h>     	/* time */
#include <string.h>     /* memcmp */

#include "../sorting.h"
#include "../../../chen/MyUtils.h" /* MAX2,MIN2 */

#define ARR_LENGTH 50
#define NUM_OF_ITERATIONS 30
#define RAND_RANGE 5000

void TestHeapSort();

/* qsort compare function */
int QSortCompare(const void *num1, const void *num2);

/* is before function */	
int MyIsBefore(const void *new_data, 
			   const void *src_data, 
			   void *param);

int main()
{
	TestHeapSort();

	return 0;
}

void TestHeapSort()
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
		for (i = 0; i < ARR_LENGTH; ++i)
		{
			temp = rand()%RAND_RANGE - RAND_RANGE / 2;
			arr_res[i] = temp;
			arr_exp[i] = temp;
		}
		
		printf("\nbefore \nqsort\n");
		for (i = 0; i< ARR_LENGTH; ++i)
		{
			printf("%d ", arr_exp[i]);
		}
		
		printf("\nheap sort\n");
		for (i = 0; i< ARR_LENGTH; ++i)
		{
			printf("%d ", arr_res[i]);
		}
		
		HeapSort(arr_res, 
				ARR_LENGTH, 
				sizeof(int), 
				MyIsBefore, 
				&param);
		qsort(arr_exp, ARR_LENGTH, sizeof(int), QSortCompare);
		
		status |= memcmp(arr_res, arr_exp, ARR_LENGTH * sizeof(int));
	}
	
	printf("\nAfter \nqsort\n");
	for (i = 0; i< ARR_LENGTH; ++i)
	{
		printf("%d ", arr_exp[i]);
	}
	
	printf("\nheap sort\n");
	for (i = 0; i< ARR_LENGTH; ++i)
	{
		printf("%d ", arr_res[i]);
	}
	printf("\n");
	
	PRINTTESTRESULTS("TestSelectionSortVsQSort",1, 0 == status);
}

int QSortCompare(const void *num1, const void *num2)
{
	return ((*(int *)num1) - (*(int *)num2));
}

int MyIsBefore(const void *new_data, 
			   const void *src_data, 
			   void *param)
{
	UNUSED(param);
	
	return ((*(int *)new_data) < (*(int *)src_data));
}						   
