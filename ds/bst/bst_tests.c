#include <stdio.h> /* printf */
#include <stdlib.h> /* printf */

#include "bst.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

int MyComparisonFunc(const void *new_data, 
					 const void *src_data, 
					 const void *comp_param);
int QSortCompare(const void *num1, const void *num2);
int MyForEachFunc(void *data, void *for_each_param);

void TestCreateDestroy();
void TestInsertDestroy();
void TestNextPrev();
void TestSizeIsEmpty();
void TestFind();
void TestForEach();

int main()
{
	TestCreateDestroy();
	TestInsertDestroy();
	TestNextPrev();
	TestSizeIsEmpty();
	TestFind();
	TestForEach();
	
	
	
	return 0;
}

void TestCreateDestroy()
{
	int param = 0;
	
	bst_t *new_bst = BSTCreate(MyComparisonFunc, &param);
	printf("Create bst\n");
	
	BSTDestroy(new_bst);
	printf("Destroy bst\n\n");
}

void TestInsertDestroy()
{
	int param = 0;
	int values[] = {100,150,300,-20,-50,452,465,0,101,102,103};
	
	size_t i = 0;
	
	bst_t *new_bst = BSTCreate(MyComparisonFunc, &param);
	
	for (i = 0; i < sizeof(values)/sizeof(int); ++i)
	{
		PRINTTESTRESULTS("TestInsertDestroy_Insert",i, 
		values[i] == *(int *)BSTGetData(BSTInsert(new_bst, &values[i])));
	}
	
	BSTDestroy(new_bst);
	printf("Destroy bst\n\n");
}

void TestNextPrev()
{
	int param = 0;
	int values[] = {100,150,300,-20,-50,452,465,0,101,102,103};
/*	bst_iter_t iter_array = (bst_iter_t)malloc(sizeof(values)/sizeof(int));*/
	
	bst_t *new_bst = BSTCreate(MyComparisonFunc, &param);
	
	bst_iter_t iter4 = BSTInsert(new_bst, &values[4]);
	bst_iter_t iter3 = BSTInsert(new_bst, &values[3]);
	bst_iter_t iter7 = BSTInsert(new_bst, &values[7]);
	bst_iter_t iter0 = BSTInsert(new_bst, &values[0]);
	bst_iter_t iter8 = BSTInsert(new_bst, &values[8]);
	bst_iter_t iter9 = BSTInsert(new_bst, &values[9]);
	bst_iter_t iter10 = BSTInsert(new_bst, &values[10]);
	bst_iter_t iter1 = BSTInsert(new_bst, &values[1]);
	bst_iter_t iter2 = BSTInsert(new_bst, &values[2]);
	bst_iter_t iter5 = BSTInsert(new_bst, &values[5]);
	bst_iter_t iter6 = BSTInsert(new_bst, &values[6]);
	
	PRINTTESTRESULTS("TestNextPrev_next",1, 1 == BSTIsSameIterator(BSTNext(iter4), iter3));
	PRINTTESTRESULTS("TestNextPrev_next",2, 1 == BSTIsSameIterator(BSTNext(iter3), iter7));
	PRINTTESTRESULTS("TestNextPrev_next",3, 1 == BSTIsSameIterator(BSTNext(iter7), iter0));
	PRINTTESTRESULTS("TestNextPrev_next",4, 1 == BSTIsSameIterator(BSTNext(iter0), iter8));
	PRINTTESTRESULTS("TestNextPrev_next",5, 1 == BSTIsSameIterator(BSTNext(iter8), iter9));
	PRINTTESTRESULTS("TestNextPrev_next",6, 1 == BSTIsSameIterator(BSTNext(iter9), iter10));
	PRINTTESTRESULTS("TestNextPrev_next",7, 1 == BSTIsSameIterator(BSTNext(iter10), iter1));
	PRINTTESTRESULTS("TestNextPrev_next",8, 1 == BSTIsSameIterator(BSTNext(iter1), iter2));
	PRINTTESTRESULTS("TestNextPrev_next",9, 1 == BSTIsSameIterator(BSTNext(iter2), iter5));
	PRINTTESTRESULTS("TestNextPrev_next",10, 1 == BSTIsSameIterator(BSTNext(iter5), iter6));
	
	PRINTTESTRESULTS("TestNextPrev_prev",1, 1 == BSTIsSameIterator(BSTPrev(iter3), iter4));
	PRINTTESTRESULTS("TestNextPrev_prev",2, 1 == BSTIsSameIterator(BSTPrev(iter7), iter3));
	PRINTTESTRESULTS("TestNextPrev_prev",3, 1 == BSTIsSameIterator(BSTPrev(iter0), iter7));
	PRINTTESTRESULTS("TestNextPrev_prev",4, 1 == BSTIsSameIterator(BSTPrev(iter8), iter0));
	PRINTTESTRESULTS("TestNextPrev_prev",5, 1 == BSTIsSameIterator(BSTPrev(iter9), iter8));
	PRINTTESTRESULTS("TestNextPrev_prev",6, 1 == BSTIsSameIterator(BSTPrev(iter10), iter9));
	PRINTTESTRESULTS("TestNextPrev_prev",7, 1 == BSTIsSameIterator(BSTPrev(iter1), iter10));
	PRINTTESTRESULTS("TestNextPrev_prev",8, 1 == BSTIsSameIterator(BSTPrev(iter2), iter1));
	PRINTTESTRESULTS("TestNextPrev_prev",9, 1 == BSTIsSameIterator(BSTPrev(iter5), iter2));
	PRINTTESTRESULTS("TestNextPrev_prev",10, 1 == BSTIsSameIterator(BSTPrev(iter6), iter5));

/*	printf("Insert values into bst\n");*/
	
/*	qsort(iter_array, sizeof(values)/sizeof(int), 
					  sizeof(bst_iter_t), QSortCompare);
*/	
	
	
	BSTDestroy(new_bst);
/*	free(iter_array);*/

	printf("\n\n");
}

void TestSizeIsEmpty()
{
	int param = 0;
	int values[] = {100,150,300,-20,-50,452,465,0,101,102,103};
	bst_t *new_bst = BSTCreate(MyComparisonFunc, &param);
	size_t i = 0;
	
	for (i = 0; i < sizeof(values) / sizeof(int); ++i)
	{
		if (i == 0)
		{
			PRINTTESTRESULTS("TestSizeISEmpty_IsEmpty",2 * i, 1 == BSTIsEmpty(new_bst));
		}
		else
		{
			PRINTTESTRESULTS("TestSizeISEmpty_IsEmpty",2 * i, 0 == BSTIsEmpty(new_bst));
		}
		
		PRINTTESTRESULTS("TestSize_SizeAndThenInsert",2*(i + 1), i == BSTSize(new_bst));
		BSTInsert(new_bst, &values[i]);
	}
	
	PRINTTESTRESULTS("TestSize_size",i + 1, i == BSTSize(new_bst));
	
	BSTDestroy(new_bst);
	
	printf("\n\n");
}

void TestFind()
{
	int param = 0;
	int values[] = {100,150,300,-20,-50,452,465,0,101,102,103};
	bst_t *new_bst = BSTCreate(MyComparisonFunc, &param);
	size_t i = 0;
	int member_not_to_find = 500;
	
	printf("Insert members into bst\n");
	for (i = 0; i < sizeof(values) / sizeof(int); ++i)
	{
		BSTInsert(new_bst, &values[i]);
	}
	
	for (i = 0; i < sizeof(values) / sizeof(int); ++i)
	{
		PRINTTESTRESULTS("TestFind_Find", i + 1, 
		values[i] == *(int *)BSTGetData(BSTFind(new_bst, &values[i])));
	}
	
	PRINTTESTRESULTS("TestFind_Find", i + 1, 
	BSTIsSameIterator(BSTEnd(new_bst),  BSTFind(new_bst, &member_not_to_find)));
	
	BSTDestroy(new_bst);
	
	printf("\n\n");
}

void TestForEach()
{
	int param = 0;
	int values[] = {100,150,300,-20,-50,452,465,0,101,102,103};
	int values_qsort[] = {100,150,300,-20,-50,452,465,0,101,102,103};
	int values_sorted[] = {-40,-10,10,110,111,112,113,160,310,462,475};
	bst_t *new_bst = BSTCreate(MyComparisonFunc, &param);
	size_t i = 0;
	int for_each_param = 10;
	bst_iter_t runner = NULL;
	
	printf("Insert members into bst\n");
	for (i = 0; i < sizeof(values) / sizeof(int); ++i)
	{
		BSTInsert(new_bst, &values[i]);
	}

	qsort(values_qsort, sizeof(values_qsort)/sizeof(int),
		   	   sizeof(int), QSortCompare);
	
	PRINTTESTRESULTS("TestForEach_ForEach", 1, 
	0 == BSTForEach(BSTBegin(new_bst), BSTEnd(new_bst), 
					MyForEachFunc, &for_each_param));
	
	i = 0;					
	for (runner = BSTBegin(new_bst); 
		 !BSTIsSameIterator(runner, BSTEnd(new_bst));
		 runner = BSTNext(runner))
	{
		PRINTTESTRESULTS("TestForEach_GetData", i + 1, 
		values_sorted[i] == *(int *)BSTGetData(runner));
		++i;
	}
	
	BSTDestroy(new_bst);
	
	printf("\n\n");
}

int MyComparisonFunc(const void *new_data, 
					 const void *src_data, 
					 const void *comp_param)
{
	(void)comp_param;
	
	if (*(int *)new_data > *(int *)src_data)
	{
		return 1;
	}
	else if (*(int *)src_data > *(int *)new_data)
	{
		return -1;
	}
	
	return 0;
}

int MyForEachFunc(void *data, void *for_each_param)
{	
	*(int *)data += *(int *)for_each_param;

	return 0;
}
							   
int QSortCompare(const void *num1, const void *num2)
{
	return (*(int *)num1 - *(int *)num2);
}
