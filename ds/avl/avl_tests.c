#include <stdio.h> /* printf */

#include "avl.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

int MyCompareFuncIMP(const void *new_data, const void *src_data);
int MyForEachFunctionPrintData(void *data, void *for_each_param);

void TestCreateDestroy();
void TestInsert();
void TestFind();
void TestRemove();

int main()
{
/*	TestCreateDestroy();
	TestInsert();
	TestFind();
*/	TestRemove();
	
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
	
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	
	AVLDestroy(new_avl);
	printf("\n\n");
}

void TestRemove()
{
	int arr[] = {5,3,-8,-5,6,8,-56,9,-45,0};
	avl_t *new_avl = AVLCreate(MyCompareFuncIMP);
	unsigned int i = 0;
	int param = 0;
	
	for (i = 0; i < 10; ++i)
	{
		PRINTTESTRESULTS("TestInsert_insert",i , 0 == AVLInsert(new_avl, &arr[i]));
	}
	printf("\nbefore remove\n");
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	
	AVLRemove(new_avl, &arr[0]);
	/*AVLRemove(new_avl, &arr[3]);*/
	/*AVLRemove(new_avl, &arr[1]);*/
	/*AVLRemove(new_avl, &arr[2]);*/
	printf("after remove\n");
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	
	AVLDestroy(new_avl);
	printf("\n\n");
}

void TestFind()
{
	int arr[] = {5,3,-8,-5,6,8,-56,9,-45,0};
	int missing_value1 = 201;
	int missing_value2 = -201;
	int missing_value3 = 7;
	avl_t *new_avl = AVLCreate(MyCompareFuncIMP);
	unsigned int i = 0;
	
	for (i = 0; i < 10; ++i)
	{
		PRINTTESTRESULTS("TestFind_insert", i + 1, 0 == AVLInsert(new_avl, &arr[i]));
	}
	
	for (i = 0; i < 10; ++i)
	{
		PRINTTESTRESULTS("TestFind_find",i + 11, 
						  arr[i] == *(int *)AVLFind(new_avl, &arr[i]));
	}
	
	PRINTTESTRESULTS("TestFind_find",i + 21, 
						  NULL == AVLFind(new_avl, &missing_value1));
	PRINTTESTRESULTS("TestFind_find",i + 22, 
						  NULL == AVLFind(new_avl, &missing_value2));
	PRINTTESTRESULTS("TestFind_find",i + 23, 
						  NULL == AVLFind(new_avl, &missing_value3));					  					  
	
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

int MyForEachFunctionPrintData(void *data, void *for_each_param)
{
	(void)data;
	(void)for_each_param;
	printf("data = %d\n", *(int *)data);
	
	return 0;
}

int MyForEachFunctionPrintHeight(void *data, void *for_each_param)
{
	(void)data;
	(void)for_each_param;
	printf("data = %d\n", *(int *)data);
	
	return 0;
}
