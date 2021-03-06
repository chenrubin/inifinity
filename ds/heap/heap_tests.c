#include <stdio.h> /* printf */

#include "heap.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

static int MyComparisonFunc(const void *new_data, 
							const void *src_data,
							void *compare_param);
int MyIsMatchFunction(const void *new_data, const void *src_data);							

void TestCreateDestroy();
void TestPush();
void TestPop();
void TestRemove();
void TestPushRemove();

int main()
{
	TestCreateDestroy();
	TestPush();
	TestPop();
	TestRemove();
	TestPushRemove();	
	
	return 0;
}

void TestCreateDestroy()
{
	int param = 0;
	heap_t *new_heap = HeapCreate(MyComparisonFunc, &param);
	
	HeapDestroy(new_heap);
	printf("Create heap\n");
	printf("destory heap\n");
}

void TestPush()
{
	int arr[] = {1,95,-5,100,9,-6,-5,890};
	int highest[] = {1,95,95,100,100,100,100,890};
	size_t i = 0;
	int param = 0;
	heap_t *new_heap = HeapCreate(MyComparisonFunc, &param);
	printf("Create heap\n");
	
	for (i = 0; i < sizeof(arr)/sizeof(int); ++i)
	{
		PRINTTESTRESULTS("TestPush_size",(int)(3 * i), i == HeapSize(new_heap));
		PRINTTESTRESULTS("TestPush_IsEmpty",(int)(3 * i + 1), (i == 0) == HeapIsEmpty(new_heap));
		PRINTTESTRESULTS("TestPush_push",(int)(3 * i + 2), 0 == HeapPush(new_heap, &arr[i]));
		PRINTTESTRESULTS("TestPush_Peek",(int)(3 * i + 1), highest[i] == *(int *)HeapPeek(new_heap));
#ifndef NDEBUG
		PrintArrayIMP(new_heap);
#endif
	}
	
	HeapDestroy(new_heap);
	
	printf("destory heap\n");
}

void TestPop()
{
	int arr[] = {1,95,-5,100,9,-6,-5,890};
	int highest[] = {890,100,95,9,1,-5,-5,-6};
	size_t i = 0;
	int param = 0;
	heap_t *new_heap = HeapCreate(MyComparisonFunc, &param);
	printf("Create heap\n");
	
	printf("Push to heap\n");
	for (i = 0; i < sizeof(arr)/sizeof(int); ++i)
	{
		HeapPush(new_heap, &arr[i]);
		PrintArrayIMP(new_heap);
	}
	
	for (i = 0; i < sizeof(arr)/sizeof(int); ++i)
	{
		printf("peek = %d\n", *(int *)HeapPeek(new_heap));
		PRINTTESTRESULTS("TestPop_Peek",(int)(2 * i), highest[i] == *(int *)HeapPeek(new_heap));
		printf("Pop\n");
		HeapPop(new_heap);
		PRINTTESTRESULTS("TestPop_IsEmpty",(int)(2 * i + 1), (i == (sizeof(arr)/sizeof(int) - 1)) == HeapIsEmpty(new_heap));
#ifndef NDEBUG
		PrintArrayIMP(new_heap);
#endif
	}
	
	HeapDestroy(new_heap);
	
	printf("destory heap\n");
}

void TestRemove()
{
	int arr[] = {1,95,-5,100,9,-6,-5,890};
	int remove_arr[] = {890,95,-6};
	int highest[] = {890,100,100};
	size_t i = 0;
	int param = 0;
	heap_t *new_heap = HeapCreate(MyComparisonFunc, &param);
	printf("Create heap\n");
	
	printf("Push to heap\n");
	for (i = 0; i < sizeof(arr)/sizeof(int); ++i)
	{
		HeapPush(new_heap, &arr[i]);
		PrintArrayIMP(new_heap);
	}
	
	for (i = 0; i < 3; ++i)
	{
		PRINTTESTRESULTS("TestRemove_size",1, (sizeof(arr)/sizeof(int) - i) == HeapSize(new_heap));
		printf("peek = %d\n", *(int *)HeapPeek(new_heap));
		PRINTTESTRESULTS("TestRemove_Peek",1, highest[i] == *(int *)HeapPeek(new_heap));
		printf("Remove\n");
		PRINTTESTRESULTS("TestRemove_size",1, remove_arr[i] == *(int *)HeapRemove(new_heap, MyIsMatchFunction, &remove_arr[i]));
#ifndef NDEBUG
		PrintArrayIMP(new_heap);
#endif
	}
	
	PRINTTESTRESULTS("TestRemove_size",1, (sizeof(arr)/sizeof(int) - i) == HeapSize(new_heap));
	printf("peek = %d\n", *(int *)HeapPeek(new_heap));
	PRINTTESTRESULTS("TestRemove_Peek",1, highest[i - 1] == *(int *)HeapPeek(new_heap));
	printf("Remove\n");
	PRINTTESTRESULTS("TestRemove_size",1, NULL == HeapRemove(new_heap, MyIsMatchFunction, &remove_arr[i]));
#ifndef NDEBUG
		PrintArrayIMP(new_heap);
#endif
	
	HeapDestroy(new_heap);
	
	printf("destory heap\n");
}

void TestPushRemove()
{
	int arr[] = {50,40,30,18,36,25,28,5,6,32,33};
	size_t i = 0;
	int param = 0;
	int elem = 35;
	heap_t *new_heap = HeapCreate(MyComparisonFunc, &param);
	printf("Create heap\n");
	
	printf("Push to heap\n");
	for (i = 0; i < sizeof(arr)/sizeof(int); ++i)
	{
		HeapPush(new_heap, &arr[i]);
		PrintArrayIMP(new_heap);
	}

	PRINTTESTRESULTS("TestPushRemove_size",1, (sizeof(arr)/sizeof(int)) == HeapSize(new_heap));
	printf("Remove\n");
	PRINTTESTRESULTS("TestPushRemove_remove",2, arr[5] == *(int *)HeapRemove(new_heap, MyIsMatchFunction, &arr[5]));
	printf("peek = %d\n", *(int *)HeapPeek(new_heap));
	PRINTTESTRESULTS("TestPushRemove_Peek",3, 50 == *(int *)HeapPeek(new_heap));
#ifndef NDEBUG
		PrintArrayIMP(new_heap);
#endif
	/*50, 40, 33, 18, 36, 30, 28, 5, 6, 32,*/
	PRINTTESTRESULTS("TestPushRemove_size",4, (sizeof(arr)/sizeof(int) - 1) == HeapSize(new_heap));
	
	
	printf("Remove\n");
	PRINTTESTRESULTS("TestPushRemove_remove",5, arr[1] == *(int *)HeapRemove(new_heap, MyIsMatchFunction, &arr[1]));
	printf("peek = %d\n", *(int *)HeapPeek(new_heap));
	PRINTTESTRESULTS("TestPushRemove_Peek",6, 50 == *(int *)HeapPeek(new_heap));
#ifndef NDEBUG
		PrintArrayIMP(new_heap);
#endif
	/*50, 36, 33, 18, 32, 30, 28, 5, 6,*/
	PRINTTESTRESULTS("TestPushRemove_size",7, (sizeof(arr)/sizeof(int) - 2) == HeapSize(new_heap));
	
	
	printf("Remove\n");
	PRINTTESTRESULTS("TestPushRemove_remove",8, arr[0] == *(int *)HeapRemove(new_heap, MyIsMatchFunction, &arr[0]));
	printf("peek = %d\n", *(int *)HeapPeek(new_heap));
	PRINTTESTRESULTS("TestPushRemove_Peek",9, 36 == *(int *)HeapPeek(new_heap));
#ifndef NDEBUG
		PrintArrayIMP(new_heap);
#endif
	/*36, 32, 33, 18, 6, 30, 28, 5,*/
	PRINTTESTRESULTS("TestPushRemove_size",10, (sizeof(arr)/sizeof(int) - 3) == HeapSize(new_heap));
	
	printf("Push\n");
	PRINTTESTRESULTS("TestPush_push",11, (0 == HeapPush(new_heap, &elem)));
	printf("peek = %d\n", *(int *)HeapPeek(new_heap));
	PRINTTESTRESULTS("TestPushRemove_Peek",12, 36 == *(int *)HeapPeek(new_heap));
#ifndef NDEBUG
		PrintArrayIMP(new_heap);
#endif
	/*36, 35, 33, 32, 6, 30, 28, 5, 18*/
	PRINTTESTRESULTS("TestPushRemove_size",13, (sizeof(arr)/sizeof(int) - 2) == HeapSize(new_heap));
	
	HeapDestroy(new_heap);
	
	printf("destory heap\n");
}

static int MyComparisonFunc(const void *new_data, 
							const void *src_data,
							void *compare_param)	
{
	UNUSED(compare_param);
	
	if (*(int *)new_data >= *(int *)src_data)
	{
		return 1;
	}
	
	return 0;
}

int MyIsMatchFunction(const void *new_data, const void *src_data)
{
	return (*(int *)new_data == *(int *)src_data);
}
