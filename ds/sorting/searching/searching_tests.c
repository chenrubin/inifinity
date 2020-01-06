#include <stdio.h> /* printf */

#include "searching.h"
#include "../../../chen/MyUtils.h" /* MAX2,MIN2 */

void TestBinarySearch();
void TestJumpSearch();

int main()
{
	TestBinarySearch();
	TestJumpSearch();

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
	PRINTTESTRESULTS("TestBinarySearch", 2, 10 == *res1);
	res1 = BSearch(arr1, 10, 1);
	PRINTTESTRESULTS("TestBinarySearch", 3, 1 == *res1);
	res1 = BSearch(arr1, 10, 0);
	PRINTTESTRESULTS("TestBinarySearch", 4, NULL == res1);
	
	res2 = BSearch(arr2, 11, 8);
	PRINTTESTRESULTS("TestBinarySearch", 5, 8 == *res2);
	res2 = BSearch(arr2, 11, 11);
	PRINTTESTRESULTS("TestBinarySearch", 6, 11 == *res2);
	res2 = BSearch(arr2, 11, 1);
	PRINTTESTRESULTS("TestBinarySearch", 7, 1 == *res2);
	res2 = BSearch(arr2, 11, 0);
	PRINTTESTRESULTS("TestBinarySearch", 8, NULL == res2);
	
}

void TestJumpSearch()
{
	int arr1[] = {1,2,3,4,5,6,7,8,9,10};
	int arr2[] = {1,2,3,4,5,6,7,8,9,10,11};
	int *res1 = NULL;
	int *res2 = NULL;
	
	
	res1 = JSearch(arr1, 10, 8);
	PRINTTESTRESULTS("TestJumpSearch", 1, 8 == *res1);
	res1 = JSearch(arr1, 10, 10);
	PRINTTESTRESULTS("TestJumpSearch", 2, 10 == *res1);
	res1 = JSearch(arr1, 10, 1);
	PRINTTESTRESULTS("TestJumpSearch", 3, 1 == *res1);
	res1 = JSearch(arr1, 10, 0);
	PRINTTESTRESULTS("TestJumpSearch", 4, NULL == res1);
	
	res2 = JSearch(arr2, 11, 8);
	PRINTTESTRESULTS("TestJumpSearch", 5, 8 == *res2);
	res2 = JSearch(arr2, 11, 11);
	PRINTTESTRESULTS("TestJumpSearch", 6, 11 == *res2);
	res2 = JSearch(arr2, 11, 1);
	PRINTTESTRESULTS("TestJumpSearch", 7, 1 == *res2);
	res2 = JSearch(arr2, 11, 0);
	PRINTTESTRESULTS("TestJumpSearch", 8, NULL == res2);
	
}
