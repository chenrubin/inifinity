#include <stdio.h> /* printf */

#include "heap.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

static int MyComparisonFunc(const void *new_data, 
							const void *src_data,
							void *compare_param);

void TestCreateRemove();
void TestPush();

int main()
{
/*	TestCreateRemove();
*/	TestPush();

	return 0;
}

void TestCreateRemove()
{
	int param = 0;
	heap_t *new_heap = HeapCreate(MyComparisonFunc, &param);
	
	HeapDestroy(new_heap);
	printf("Create heap\n");
	printf("destory heap\n");
}

void TestPush()
{
	int arr[] = {1,95,-5,9,-6,-5,89};
	size_t i = 0;
	int param = 0;
	heap_t *new_heap = HeapCreate(MyComparisonFunc, &param);
	
	for (i = 0; i < sizeof(arr)/sizeof(int); ++i)
	{
		HeapPush(new_heap, &arr[i]);
	}
	
	HeapDestroy(new_heap);
	printf("Create heap\n");
	printf("destory heap\n");
}

static int MyComparisonFunc(const void *new_data, 
							const void *src_data,
							void *compare_param)
{
	UNUSED(compare_param);
	
	if (*(int *)new_data > *(int *)src_data)
	{
		return 1;
	}
	
	return 0;
}
