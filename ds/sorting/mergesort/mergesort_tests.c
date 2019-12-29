#include <stdio.h> /* printf */
#include <time.h> /* time */
#include <stdlib.h> /* rand */
#include <unistd.h> /* sleep */

#include "../sorting.h"
#include "../../../chen/MyUtils.h" /* MAX2,MIN2 */

#define SIZE (20)

int MyCompare(const void* p1, const void* p2);

int main()
{
	size_t i = 0;
	int arr1[SIZE] = {0};
	int arr2[SIZE + 1] = {0};
	int arr1qsort[SIZE] = {0};
	int arr2qsort[SIZE + 1] = {0};
	int temp = 0;
	int status = 1;
	
	srand(time(NULL));
	
	for (i = 0; i < SIZE; ++i)
	{
		sleep(1);
		printf("i = %ld\n", i);
		temp = rand() % 100 - 50;
		arr1[i] = temp;
		arr1qsort[i] = temp;
	}
	for (i = 0; i < SIZE + 1; ++i)
	{
		sleep(1);
		printf("i = %ld\n", i);
		temp = rand() % 100 - 50;
		arr2[i] = temp;
		arr2qsort[i] = temp;
	}
		
	qsort(arr1qsort, SIZE, sizeof(int), MyCompare);
	qsort(arr2qsort, SIZE + 1, sizeof(int), MyCompare);
	
/*	printf("before sort\n");
	for (i = 0; i < SIZE; ++i)
	{
		printf("%d ", arr1[i]);
	}
	printf("\narr1qsort\n");
	for (i = 0; i < SIZE; ++i)
	{
		printf("%d ", arr1qsort[i]);
	}
	printf("\n");
	printf("arr2\n");
	for (i = 0; i < SIZE + 1; ++i)
	{
		printf("%d ", arr2[i]);
	}
	printf("\narr2qsort\n");
	for (i = 0; i < SIZE + 1; ++i)
	{
		printf("%d ", arr2qsort[i]);
	}
	printf("\n");
*/	MergeSort(arr1, SIZE);
	MergeSort(arr2, SIZE + 1);

	for (i = 0; i < SIZE; ++i)
	{
		status &= (arr1[i] == arr1qsort[i]);
	}
	PRINTTESTRESULTS("First sort of even numbered arr",1, 1 == status);
/*	printf("arr1\n");
	for (i = 0; i < SIZE; ++i)
	{
		printf("%d ", arr1[i]);
	}
	printf("\narr1qsort\n");
	for (i = 0; i < SIZE; ++i)
	{
		printf("%d ", arr1qsort[i]);
	}
	printf("\n");
	
*/	for (i = 0; i < SIZE + 1; ++i)
	{
		status &= (arr2[i] == arr2qsort[i]);
	}
	PRINTTESTRESULTS("First sort of odd numbered arr",1, 1 == status);
/*	printf("arr2\n");
	for (i = 0; i < SIZE + 1; ++i)
	{
		printf("%d ", arr2[i]);
	}
	printf("\narr2qsort\n");
	for (i = 0; i < SIZE + 1; ++i)
	{
		printf("%d ", arr2qsort[i]);
	}
	printf("\n");
*/	
	return 0;
}
 
int MyCompare(const void* p1, const void* p2)
{
	return (*(int *)p1 > *(int *)p2);
}

	
