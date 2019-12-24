#include <stdio.h> /* printf */

#include "avl.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

int MyCompareFuncIMP(const void *new_data, const void *src_data);
int MyForEachFunction(void *data, void *for_each_param);

void TestCreateDestroy();
void TestInsert();

int main()
{
	TestCreateDestroy();
	TestInsert();
	
	return 0;
}

void TestCreateDestroy()
{
	avl_t *new_avl = AVLCreate(MyCompareFuncIMP);
	AVLDestroy(new_avl);
	printf("Create AVL\n");
	printf("Destroy AVL\n");
	
	printf("\n\n");
}

void TestInsert()
{
	int arr[] = {5,3,-8,-5,6,8,-56,9,-45,0};
	avl_t *new_avl = AVLCreate(MyCompareFuncIMP);
	unsigned int i = 0;
	int param = 0;
	
	for (i = 0; i < 10; ++i)
	{
		PRINTTESTRESULTS("TestInsert_size",2*i + 1, i == AVLSize(new_avl));
		PRINTTESTRESULTS("TestInsert_insert",2*i + 2, 0 == AVLInsert(new_avl, &arr[i]));
	}
	PRINTTESTRESULTS("TestInsert",2*i + 1, i == AVLSize(new_avl));
	
	AVLForEach(new_avl, MyForEachFunction, &param);
	
	AVLDestroy(new_avl);
	printf("\n\n");
}

int MyCompareFuncIMP(const void *new_data, const void *src_data)
{
	if (*(int *)new_data > *(int *)src_data)
	{
		return 1;
	}
	else if (*(int *)new_data < *(int *)src_data)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int MyForEachFunction(void *data, void *for_each_param)
{
	(void)data;
	(void)for_each_param;
	printf("data = %d\n", *(int *)data);
	
	return 0;
}
