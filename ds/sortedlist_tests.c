#include <stdio.h> /* printf */

#include "sortedlist.h"
#include "dllist.h"
#include "MyUtils.h" /* MAX2,MIN2 */

void TestCreateDestroy();
void TestInsert();
int IsMatch(const void *data1, const void *data2, void *param);

struct srt_list
{
	dl_list_t *dll;
	is_before_t is_before_ptr;
	void *is_before_param;
};

struct srt_iterator
{
	dll_iter_t iterator;
};

int main()
{
	TestCreateDestroy();
	TestInsert();

	return 0;
}

void TestCreateDestroy()
{
	srt_list_t *new_srtlist = SrtListCreate(NULL, IsMatch);
	printf("Create srt_list\n");
	printf("Destroy srt_list\n");
	SrtListDestroy(new_srtlist);
}

void TestInsert()
{
	int n1 = 1;
	int n2 = 2;
	int n3 = 17;
	int n4 = 6;
	int n5 = 22;
	dll_node_t *node = NULL;
	
	srt_list_t *new_srtlist = SrtListCreate(NULL, IsMatch);
	printf("Create srt_list\n");
	
	
	SrtListInsert(&n1, new_srtlist);
	for (node = DLListBegin(new_srtlist -> dll);
		 node != DLListEnd(new_srtlist -> dll);
		 node = DLListNext(node))
	{
		printf("%d, ", *(int *)DLListGetData(node));
	}	 
	SrtListInsert(&n2, new_srtlist);
	for (node = DLListBegin(new_srtlist -> dll);
		 node != DLListEnd(new_srtlist -> dll);
		 node = DLListNext(node))
	{
		printf("%d, ", *(int *)DLListGetData(node));
	}	 
	SrtListInsert(&n3, new_srtlist);
	for (node = DLListBegin(new_srtlist -> dll);
		 node != DLListEnd(new_srtlist -> dll);
		 node = DLListNext(node))
	{
		printf("%d, ", *(int *)DLListGetData(node));
	}	 
	SrtListInsert(&n4, new_srtlist);
	for (node = DLListBegin(new_srtlist -> dll);
		 node != DLListEnd(new_srtlist -> dll);
		 node = DLListNext(node))
	{
		printf("%d, ", *(int *)DLListGetData(node));
	}	 
	SrtListInsert(&n5, new_srtlist);
	for (node = DLListBegin(new_srtlist -> dll);
		 node != DLListEnd(new_srtlist -> dll);
		 node = DLListNext(node))
	{
		printf("%d, ", *(int *)DLListGetData(node));
	}	 	
	
	printf("Destroy srt_list\n");
	SrtListDestroy(new_srtlist);
}

/********************************************************/
/* list is sorted small to big */
int IsMatch(const void *data1, const void *data2, void *param)
{
	(void)param;
	
	return (*(int *)data1 <= *(int *)data2);
}
