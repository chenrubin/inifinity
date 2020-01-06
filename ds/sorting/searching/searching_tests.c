#include <stdio.h> /* printf */

#include "searching.h"
#include "../../../chen/MyUtils.h" /* MAX2,MIN2 */

void TestBinarySearch();

int main()
{
	TestBinarySearch();

	return 0;
}

void TestBinarySearch()
{
	int arr1[] = {1,2,3,4,5,6,7,8,9,10};
	int arr2[] = {1,2,3,4,5,6,7,8,9,10,11};
	int *res1 = NULL;
	int *res2 = NULL;
	
	
	res1 = BSearch(arr1, 10, 8);
	PRINTTESTRESULTS("TestBinarySearch", 1, 8 == *res1);
	res1 = BSearch(arr1, 10, 10);
	PRINTTESTRESULTS("TestBinarySearch", 1, 10 == *res1);
	res1 = BSearch(arr1, 10, 1);
	PRINTTESTRESULTS("TestBinarySearch", 1, 1 == *res1);
	res1 = BSearch(arr1, 10, 0);
	PRINTTESTRESULTS("TestBinarySearch", 1, NULL == res1);
	
	res2 = BSearch(arr2, 11, 8);
	PRINTTESTRESULTS("TestBinarySearch", 1, 8 == *res2);
	res2 = BSearch(arr2, 11, 11);
	PRINTTESTRESULTS("TestBinarySearch", 1, 11 == *res2);
	res2 = BSearch(arr2, 11, 1);
	PRINTTESTRESULTS("TestBinarySearch", 1, 1 == *res2);
	res2 = BSearch(arr2, 11, 0);
	PRINTTESTRESULTS("TestBinarySearch", 1, NULL == res2);
	
}
