#include <stdio.h> /* printf */
#include <string.h> /* strcmp */

#include "sortedlist.h"
#include "dllist.h"
#include "MyUtils.h" /* MAX2,MIN2 */

void TestCreateDestroy();
void TestInsert();
void TestPopFrontSize();
void TestPopBackSize();
void TestRemove();
void TestMerge();
void TestMergeOpposite();
void TestForEach();
void TestFindIf();
void TestFind();
void TestFindStruct();
int IsMatch(const void *data1, const void *data2, void *param);
int IsBeforeStringCmp(const void *data1, const void *data2, void *param);
int PrintSortedList(void *data, void *param);
int AddToNode(void *data, void *param);
int FindNumber(const void *data, const void *param);
int FindIfStudent(const void *data, const void *param);
int IsMatchOpposite(const void *data1, const void *data2, void *param);

typedef struct student
{
	char *name;
	int age;
}student_t;

int main()
{
	TestCreateDestroy();
	TestInsert();
	TestPopFrontSize();
	TestPopBackSize();	
	TestRemove();
	TestMerge();
	TestMergeOpposite();
	TestForEach();
	TestFindIf();
	TestFind();
	TestFindStruct();	
	
	return 0;
}

void TestCreateDestroy()
{
	srt_list_t *new_srtlist = SrtListCreate(NULL, IsMatch);
	printf("Create srt_list\n");
	printf("Destroy srt_list\n");
	SrtListDestroy(new_srtlist);
	printf("Check valgrind for non leeks\n\n");
}

void TestInsert()
{
	int list_members[] = {1,2,17,6,22,6};
	int list_sorted[] = {1,2,6,6,17,22};
	srt_iter_t srt_iter = {0};
	int i = 0;
	
	srt_list_t *new_srtlist = SrtListCreate(NULL, IsMatch);
	printf("Create srt_list\n");
	
	for (i = 0; i < 6; ++i)
	{
		srt_iter = SrtListInsert(&list_members[i], new_srtlist);
		PRINTTESTRESULTS("TestInsert_Insert",i ,
			list_members[i] == *(int *)SrtListGetData(srt_iter));
	}
	
	i = 0;
		for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		PRINTTESTRESULTS("TestInsert_GetData",6 + i, 
		list_sorted[i] == *(int *)SrtListGetData(srt_iter));
		++i;
	}
	
	i = 0;
	for (srt_iter = SrtListPrev(SrtListEnd(new_srtlist));
		 !SrtListIsSameIterator(srt_iter, 
		 						SrtListPrev(SrtListBegin(new_srtlist)));
		 srt_iter = SrtListPrev(srt_iter))
	{
		PRINTTESTRESULTS("TestInsert_GetData",12 + i, 
		list_sorted[5 - i] == *(int *)SrtListGetData(srt_iter));
		++i;
	} 	 
	
	printf("\n");
		   
	printf("Destroy srt_list\n\n");
	SrtListDestroy(new_srtlist);
}

void TestPopFrontSize()
{
	int list_members[] = {1,2,17,6};
	int list_sorted1[] = {1,2,6,17};
	int list_sorted2[] = {2,6,17};
	int list_sorted3[] = {6,17};
	int list_sorted4[] = {17};
	srt_iter_t srt_iter = {0};
	int i = 0;
	
	srt_list_t *new_srtlist = SrtListCreate(NULL, IsMatch);
	printf("Create srt_list\n");
	
	for (i = 0; i < 4; ++i)
	{
		srt_iter = SrtListInsert(&list_members[i], new_srtlist);
		PRINTTESTRESULTS("TestPopFrontSize_Insert",i ,
			list_members[i] == *(int *)SrtListGetData(srt_iter));
	}
	PRINTTESTRESULTS("TestPopFrontSize_Empty",i , 
					 0 == SrtListIsEmpty(new_srtlist));
	PRINTTESTRESULTS("TestPopFrontSize_Size",i , 4 == SrtListSize(new_srtlist));
	
	PRINTTESTRESULTS("TestPopFrontSize_PopFront",i ,
			list_sorted1[0] == *(int *)SrtListPopFront(new_srtlist));
	PRINTTESTRESULTS("TestPopFrontSize_Empty",i , 
					 0 == SrtListIsEmpty(new_srtlist));		
	PRINTTESTRESULTS("TestPopFrontSize_Size",i , 3 == SrtListSize(new_srtlist));		
	i = 0;
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		PRINTTESTRESULTS("TestPopFrontSize_GetData",4 + i, 
		list_sorted2[i] == *(int *)SrtListGetData(srt_iter));
		++i;
	}
	
	PRINTTESTRESULTS("TestPopFrontSize_PopFront",8 + i,
			list_sorted1[1] == *(int *)SrtListPopFront(new_srtlist));
	PRINTTESTRESULTS("TestPopFrontSize_Empty",i , 
					 0 == SrtListIsEmpty(new_srtlist));		
	PRINTTESTRESULTS("TestPopFrontSize_Size",i , 2 == SrtListSize(new_srtlist));		
	i = 0;
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		PRINTTESTRESULTS("TestPopFrontSize_GetData",4 + i, 
		list_sorted3[i] == *(int *)SrtListGetData(srt_iter));
		++i;
	}
	
	PRINTTESTRESULTS("TestPopFrontSize_PopFront",8 + i,
			list_sorted1[2] == *(int *)SrtListPopFront(new_srtlist));
	PRINTTESTRESULTS("TestPopFrontSize_Empty",i , 
					 0 == SrtListIsEmpty(new_srtlist));		
	PRINTTESTRESULTS("TestPopFrontSize_Size",i , 1 == SrtListSize(new_srtlist));		
	i = 0;
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		PRINTTESTRESULTS("TestPopFrontSize_GetData",4 + i, 
		list_sorted4[i] == *(int *)SrtListGetData(srt_iter));
		++i;
	}
	
	PRINTTESTRESULTS("TestPopFrontSize_PopFront",8 + i,
			list_sorted1[3] == *(int *)SrtListPopFront(new_srtlist));
	PRINTTESTRESULTS("TestPopFrontSize_Empty",i , 
					 1 == SrtListIsEmpty(new_srtlist));		
	PRINTTESTRESULTS("TestPopFrontSize_Size",i , 0 == SrtListSize(new_srtlist));		
	
	printf("Destroy srt_list\n\n");
	SrtListDestroy(new_srtlist);
}

void TestPopBackSize()
{
	int list_members[] = {1,2,17,6};
	int list_sorted1[] = {1,2,6,17};
	int list_sorted2[] = {1,2,6};
	int list_sorted3[] = {1,2};
	int list_sorted4[] = {1};
	srt_iter_t srt_iter = {0};
	int i = 0;
	
	srt_list_t *new_srtlist = SrtListCreate(NULL, IsMatch);
	printf("Create srt_list\n");
	
	for (i = 0; i < 4; ++i)
	{
		srt_iter = SrtListInsert(&list_members[i], new_srtlist);
		PRINTTESTRESULTS("TestPopBackSize_Insert",i ,
			list_members[i] == *(int *)SrtListGetData(srt_iter));
	}
	
	PRINTTESTRESULTS("TestPopBackSize_Size",i , 4 == SrtListSize(new_srtlist));
	
	PRINTTESTRESULTS("TestPopBackSize_PopBack",i ,
			list_sorted1[3] == *(int *)SrtListPopBack(new_srtlist));
	PRINTTESTRESULTS("TestPopBackSize_Size",i , 3 == SrtListSize(new_srtlist));		
	i = 0;
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		PRINTTESTRESULTS("TestPopBackSize_GetData",4 + i, 
		list_sorted2[i] == *(int *)SrtListGetData(srt_iter));
		++i;
	}
	
	PRINTTESTRESULTS("TestPopBackSize_PopBack",8 + i,
			list_sorted2[2] == *(int *)SrtListPopBack(new_srtlist));
	PRINTTESTRESULTS("TestPopBackSize_Size",i , 2 == SrtListSize(new_srtlist));		
	i = 0;
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		PRINTTESTRESULTS("TestPopBackSize_GetData",4 + i, 
		list_sorted3[i] == *(int *)SrtListGetData(srt_iter));
		++i;
	}
	
	PRINTTESTRESULTS("TestPopBackSize_PopBack",8 + i,
			list_sorted3[1] == *(int *)SrtListPopBack(new_srtlist));
	PRINTTESTRESULTS("TestPopBackSize_Size",i , 1 == SrtListSize(new_srtlist));		
	i = 0;
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		PRINTTESTRESULTS("TestPopBackSize_GetData",4 + i, 
		list_sorted4[i] == *(int *)SrtListGetData(srt_iter));
		++i;
	}
	
	PRINTTESTRESULTS("TestPopBackSize_PopBack",8 + i,
			list_sorted4[0] == *(int *)SrtListPopBack(new_srtlist));
	PRINTTESTRESULTS("TestPopBackSize_Size",i , 0 == SrtListSize(new_srtlist));		
	
	printf("Destroy srt_list\n\n");
	SrtListDestroy(new_srtlist);
}

void TestRemove()
{
	int list_members[] = {1,2,17,6,2,9};
	int list_sorted2[] = {2,2,6,9,17};
	int list_sorted3[] = {2,6,9,17};
	int list_sorted4[] = {2,6,9};
	srt_iter_t srt_iter = {0};
	srt_iter_t removed_srt_iter = {0};
	
	int i = 0;
	
	srt_list_t *new_srtlist = SrtListCreate(NULL, IsMatch);
	printf("Create srt_list\n");
	
	for (i = 0; i < 6; ++i)
	{
		srt_iter = SrtListInsert(&list_members[i], new_srtlist);
		PRINTTESTRESULTS("TestRemove_Insert",i ,
			list_members[i] == *(int *)SrtListGetData(srt_iter));
	}
	
	PRINTTESTRESULTS("TestRemove_Size",i , 6 == SrtListSize(new_srtlist));
	
	removed_srt_iter = SrtListBegin(new_srtlist);
	srt_iter = SrtListNext(removed_srt_iter);
	PRINTTESTRESULTS("TestRemove_Remove",i ,
			SrtListIsSameIterator(srt_iter, SrtListRemove(removed_srt_iter)));
			
	PRINTTESTRESULTS("TestRemove_Size",i , 5 == SrtListSize(new_srtlist));	
	
	i = 0;
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		PRINTTESTRESULTS("TestRemove_GetData",4 + i, 
		list_sorted2[i] == *(int *)SrtListGetData(srt_iter));
		++i;
	}
	
	/**/
	removed_srt_iter = SrtListNext(SrtListBegin(new_srtlist));
	srt_iter = SrtListNext(removed_srt_iter);
	PRINTTESTRESULTS("TestRemove_Remove",i ,
			SrtListIsSameIterator(srt_iter, SrtListRemove(removed_srt_iter)));
			
	PRINTTESTRESULTS("TestRemove_Size",i , 4 == SrtListSize(new_srtlist));
	
	i = 0;
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		PRINTTESTRESULTS("TestRemove_GetData",4 + i, 
		list_sorted3[i] == *(int *)SrtListGetData(srt_iter));
		++i;
	}
	
	/**/
	removed_srt_iter = SrtListPrev(SrtListEnd(new_srtlist));
	srt_iter = SrtListEnd(new_srtlist);
	PRINTTESTRESULTS("TestRemove_Remove",i ,
			SrtListIsSameIterator(srt_iter, SrtListRemove(removed_srt_iter)));
			
	PRINTTESTRESULTS("TestRemove_Size",i , 3 == SrtListSize(new_srtlist));
	
	i = 0;
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		PRINTTESTRESULTS("TestRemove_GetData",4 + i, 
		list_sorted4[i] == *(int *)SrtListGetData(srt_iter));
		++i;
	}
	
	printf("Destroy srt_list\n\n");
	SrtListDestroy(new_srtlist);
}

void TestMerge()
{
	int list_members1[] = {1,2,17,6,2,9,4,8,12,14};
	int list_members2[] = {23,-89,-65,-8,12,512,-1202,1,5};
	int list_merged[] = {-1202,-89,-65,-8,1,1,2,2,4,5,6,8,9,12,12,14,17,23,512};
	srt_iter_t srt_iter = {0};
	int i = 0;
	
	srt_list_t *new_srtlist1 = SrtListCreate(NULL, IsMatch);
	srt_list_t *new_srtlist2 = SrtListCreate(NULL, IsMatch);
	printf("Create two srt_list\n");
	
	for (i = 0; i < 10; ++i)
	{
		srt_iter = SrtListInsert(&list_members1[i], new_srtlist1);
		PRINTTESTRESULTS("TestMerge_Insert",i ,
			list_members1[i] == *(int *)SrtListGetData(srt_iter));
	}
	PRINTTESTRESULTS("TestMerge_Size",i , 10 == SrtListSize(new_srtlist1));
	
	for (i = 0; i < 9; ++i)
	{
		srt_iter = SrtListInsert(&list_members2[i], new_srtlist2);
		PRINTTESTRESULTS("TestMerge_Insert",i ,
			list_members2[i] == *(int *)SrtListGetData(srt_iter));
	}
	PRINTTESTRESULTS("TestMerge_Size",i , 9 == SrtListSize(new_srtlist2));

	printf("Merge lists\n");
	SrtListMerge(new_srtlist1, new_srtlist2);
	
	i = 0;
	for (srt_iter = SrtListBegin(new_srtlist2);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist2));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		PRINTTESTRESULTS("TestMerge_GetData",4 + i, 
		list_merged[i] == *(int *)SrtListGetData(srt_iter));
		++i;
	}
	
	for (srt_iter = SrtListBegin(new_srtlist2);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist2));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		printf("%d, ", *(int *)SrtListGetData(srt_iter));
	}
	
	SrtListDestroy(new_srtlist1);
	
	printf("Destroy srt_list\n\n");
	SrtListDestroy(new_srtlist2);
}

void TestMergeOpposite()
{
	int list_members1[] = {1,2,17,6,2,9,4,8,12,14};
	int list_members2[] = {23,-89,-65,-8,12,512,-1202,1,5};
	int list_merged[] = {512,23,17,14,12,12,9,8,6,5,4,2,2,1,1,-8,-65,-89,-1202};
	srt_iter_t srt_iter = {0};
	int i = 0;
	
	srt_list_t *new_srtlist1 = SrtListCreate(NULL, IsMatch);
	srt_list_t *new_srtlist2 = SrtListCreate(NULL, IsMatchOpposite);
	printf("Create two srt_list\n");
	
	for (i = 0; i < 10; ++i)
	{
		srt_iter = SrtListInsert(&list_members1[i], new_srtlist1);
		PRINTTESTRESULTS("TestMergeOpposite_Insert",i ,
			list_members1[i] == *(int *)SrtListGetData(srt_iter));
	}
	PRINTTESTRESULTS("TestMergeOpposite_Size",i , 10 == SrtListSize(new_srtlist1));
	
	for (i = 0; i < 9; ++i)
	{
		srt_iter = SrtListInsert(&list_members2[i], new_srtlist2);
		PRINTTESTRESULTS("TestMergeOpposite_Insert",i ,
			list_members2[i] == *(int *)SrtListGetData(srt_iter));
	}
	PRINTTESTRESULTS("TestMergeOpposite_Size",i , 9 == SrtListSize(new_srtlist2));
	
	printf("Merge lists\n");
	SrtListMerge(new_srtlist1, new_srtlist2);
	
	i = 0;
	for (srt_iter = SrtListBegin(new_srtlist2);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist2));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		PRINTTESTRESULTS("TestMergeOpposite_GetData",4 + i, 
		list_merged[i] == *(int *)SrtListGetData(srt_iter));
		++i;
	}
	
	for (srt_iter = SrtListBegin(new_srtlist2);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist2));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		printf("%d, ", *(int *)SrtListGetData(srt_iter));
	}
	
	SrtListDestroy(new_srtlist1);
	printf("Destroy srt_list\n\n");
	SrtListDestroy(new_srtlist2);
}

void TestForEach()
{
	int list_members[] = {1,2,17,6,2,9};
	int list_sorted[] = {1,2,2,6,9,17};
	srt_iter_t srt_iter = {0};	
	int i = 0;
	int param = 1;
	
	srt_list_t *new_srtlist = SrtListCreate(NULL, IsMatch);
	printf("Create srt_list\n");
	
	for (i = 0; i < 6; ++i)
	{
		srt_iter = SrtListInsert(&list_members[i], new_srtlist);
		PRINTTESTRESULTS("TestForEach_Insert",i ,
			list_members[i] == *(int *)SrtListGetData(srt_iter));
	}
	
	i = 0;
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		PRINTTESTRESULTS("TestForEach_GetData",4 + i, 
		list_sorted[i] == *(int *)SrtListGetData(srt_iter));
		++i;
	}
	
	PRINTTESTRESULTS("TestForEach_ForEach",i, 
	0 == SrtListForEach(SrtListBegin(new_srtlist), 
	SrtListEnd(new_srtlist), &param, AddToNode));
	
	i = 0;
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		PRINTTESTRESULTS("TestForEach_GetData",4 + i, 
		(list_sorted[i] + param) == *(int *)SrtListGetData(srt_iter));
		++i;
	}
	
	printf("Destroy srt_list\n\n");
	SrtListDestroy(new_srtlist);
}

void TestFindIf()
{
	int list_members[] = {1,2,17,6,2,9};	
	srt_iter_t srt_iter = {0};	
	int i = 0;
	int param_to_find = 17;
	int param_not_to_find = 3;	
	int FindNumber(const void *data, const void *param);
	srt_list_t *new_srtlist = SrtListCreate(NULL, IsMatch);
	printf("Create srt_list\n");
	
	for (i = 0; i < 6; ++i)
	{
		srt_iter = SrtListInsert(&list_members[i], new_srtlist);
		PRINTTESTRESULTS("TestFindIf_Insert",i ,
			list_members[i] == *(int *)SrtListGetData(srt_iter));
	}
	
	PRINTTESTRESULTS("TestFindIf_FindIf",i ,
	SrtListIsSameIterator(SrtListPrev(SrtListEnd(new_srtlist)), 
	SrtListFindIf(SrtListBegin(new_srtlist), SrtListEnd(new_srtlist), 
	&param_to_find, FindNumber)));
						 
	PRINTTESTRESULTS("TestFindIf_FindIf",i + 1,
	SrtListIsSameIterator(SrtListEnd(new_srtlist),
	SrtListFindIf(SrtListBegin(new_srtlist), SrtListEnd(new_srtlist), 
	&param_not_to_find, FindNumber)));					 
	
	printf("Destroy srt_list\n\n");
	SrtListDestroy(new_srtlist);
}

void TestFind()
{
	int list_members[] = {1,2,17,6,2,9};	
	srt_iter_t srt_iter = {0};	
	int i = 0;
	const int param_to_find = 17;
	const int param_not_to_find = 3;	
	int FindNumber(const void *data, const void *param);
	srt_list_t *new_srtlist = SrtListCreate(NULL, IsMatch);
	printf("Create srt_list\n");
	
	for (i = 0; i < 6; ++i)
	{
		srt_iter = SrtListInsert(&list_members[i], new_srtlist);
		PRINTTESTRESULTS("TestFind_Insert",i ,
			list_members[i] == *(int *)SrtListGetData(srt_iter));
	}
	
	PRINTTESTRESULTS("TestFind_Find",i ,
	SrtListIsSameIterator(SrtListPrev(SrtListEnd(new_srtlist)),
	SrtListFind(SrtListBegin(new_srtlist), SrtListEnd(new_srtlist), 
	&param_to_find, new_srtlist)));
						 
	PRINTTESTRESULTS("TestFind_Find",i + 1,
	SrtListIsSameIterator(SrtListEnd(new_srtlist),
	SrtListFind(SrtListBegin(new_srtlist), SrtListEnd(new_srtlist), 
	&param_not_to_find, new_srtlist)));					 
	
	printf("Destroy srt_list\n\n");
	SrtListDestroy(new_srtlist);
}

void TestFindStruct()
{
	student_t student1 = {"maoz", 29};
	student_t student2 = {"eyal", 41};
	student_t student3 = {"sharon", 25};
	student_t student4 = {"erez", 37};
	student_t student5 = {"tamir", 26};
	srt_iter_t srt_iter = {0};
	
	int i = 0;
	const char *param_to_find = "tamir";
	const char *param_not_to_find = "moshe";	
	srt_list_t *new_srtlist = SrtListCreate(NULL, IsBeforeStringCmp);
	printf("Create srt_list\n");
	
	SrtListInsert(&student1, new_srtlist);
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		printf("%s, ", ((student_t *)SrtListGetData(srt_iter))->name);
	}
	printf("\n");
	SrtListInsert(&student2, new_srtlist);
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		printf("%s, ", ((student_t *)SrtListGetData(srt_iter))->name);
	}
	printf("\n");
	SrtListInsert(&student3, new_srtlist);
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		printf("%s, ", ((student_t *)SrtListGetData(srt_iter))->name);
	}
	printf("\n");
	SrtListInsert(&student4, new_srtlist);
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		printf("%s, ", ((student_t *)SrtListGetData(srt_iter))->name);
	}
	printf("\n");
	SrtListInsert(&student5, new_srtlist);
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		printf("%s, ", ((student_t *)SrtListGetData(srt_iter))->name);
	}
	printf("\n");
	PRINTTESTRESULTS("TestFindStruct_Find",i ,
	SrtListIsSameIterator(SrtListPrev(SrtListEnd(new_srtlist)),
	SrtListFind(SrtListBegin(new_srtlist), SrtListEnd(new_srtlist), 
	&param_to_find, new_srtlist)));
						 
	PRINTTESTRESULTS("TestFindStruct_Find",i + 1,
	SrtListIsSameIterator(SrtListEnd(new_srtlist),
	SrtListFind(SrtListBegin(new_srtlist), SrtListEnd(new_srtlist), 
	&param_not_to_find, new_srtlist)));					 
	
	printf("Destroy srt_list\n\n");
	SrtListDestroy(new_srtlist);
}

void TestFindIfStruct()
{
	student_t student1 = {"maoz", 29};
	student_t student2 = {"eyal", 41};
	student_t student3 = {"sharon", 25};
	student_t student4 = {"erez", 37};
	student_t student5 = {"tamir", 26};
	srt_iter_t srt_iter = {0};
	
	int i = 0;
	const int param_to_find = 41;
	const int param_not_to_find = 40;	
	srt_list_t *new_srtlist = SrtListCreate(NULL, IsBeforeStringCmp);
	printf("Create srt_list\n");
	
	SrtListInsert(&student1, new_srtlist);
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		printf("%s, ", ((student_t *)SrtListGetData(srt_iter))->name);
	}
	printf("\n");
	SrtListInsert(&student2, new_srtlist);
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		printf("%s, ", ((student_t *)SrtListGetData(srt_iter))->name);
	}
	printf("\n");
	SrtListInsert(&student3, new_srtlist);
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		printf("%s, ", ((student_t *)SrtListGetData(srt_iter))->name);
	}
	printf("\n");
	SrtListInsert(&student4, new_srtlist);
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		printf("%s, ", ((student_t *)SrtListGetData(srt_iter))->name);
	}
	printf("\n");
	SrtListInsert(&student5, new_srtlist);
	for (srt_iter = SrtListBegin(new_srtlist);
		 	 !SrtListIsSameIterator(srt_iter, SrtListEnd(new_srtlist));
		 	 srt_iter = SrtListNext(srt_iter))
	{
		printf("%s, ", ((student_t *)SrtListGetData(srt_iter))->name);
	}
	printf("\n");
	PRINTTESTRESULTS("TestFindIfStruct_FindIf",i ,
	SrtListIsSameIterator(SrtListPrev(SrtListEnd(new_srtlist)), 
	SrtListFindIf(SrtListBegin(new_srtlist), SrtListEnd(new_srtlist), 
	&param_to_find, FindIfStudent)));
						 
	PRINTTESTRESULTS("TestFindIfStruct_FindIf",i + 1,
	SrtListIsSameIterator(SrtListEnd(new_srtlist),
	SrtListFindIf(SrtListBegin(new_srtlist), SrtListEnd(new_srtlist), 
	&param_not_to_find, FindIfStudent)));				 
	
	printf("Destroy srt_list\n\n");
	SrtListDestroy(new_srtlist);
}

/********************************************************/
/* list is sorted small to big - isbefore function */
int IsMatch(const void *data1, const void *data2, void *param)
{
	(void)param;
	
	return (*(int *)data1 < *(int *)data2);
}

int IsMatchOpposite(const void *data1, const void *data2, void *param)
{
	(void)param;
	
	return (*(int *)data1 > *(int *)data2);
}

int IsBeforeStringCmp(const void *data1, const void *data2, void *param)
{
	(void)param;
	
	if (strcmp(((student_t *)data1)->name,((student_t *)data2)->name) < 0)
	{
		return 1;
	}
	else
	{
		return 0;	
	}
}

/* print data n a node */
int PrintSortedList(void *data, void *param)
{
	printf("%d, ", *(int *)data);
	(void)param;
	
	return 0;
}
/* foreach function */
int AddToNode(void *data, void *param)
{
	*(int *)data += *(int *)param;
		
	return 0;
}
/* findif function */
int FindNumber(const void *data, const void *param)
{
	return (*(int *)data == *(int *)param);
}

/* find function anothetr one */
int FindIfStudent(const void *data, const void *param)
{
	return (((student_t *)data) -> age == *(int *)param);
}
