#include <stdio.h> /* printf */

#include "sortedlist.h"
#include "dllist.h"
#include "MyUtils.h" /* MAX2,MIN2 */

void TestCreateDestroy();
int IsMatch(const void *data1, const void *data2, void *param);

int main()
{
	TestCreateDestroy();

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
/*	for (node = DLListBegin(new_srtlist -> dll);
		 node != DLListEnd(new_srtlist -> dll);
		 node = DLListNext(node))
	{
		printf("%d, ", *(int *)node -> data);
	}	 
	SrtListInsert(&n2, new_srtlist);
	for (node = DLListBegin(new_srtlist -> dll);
		 node != DLListEnd(new_srtlist -> dll);
		 node = DLListNext(node))
	{
		printf("%d, ", *(int *)node -> data);
	}	 
	SrtListInsert(&n3, new_srtlist);
	for (node = DLListBegin(new_srtlist -> dll);
		 node != DLListEnd(new_srtlist -> dll);
		 node = DLListNext(node))
	{
		printf("%d, ", *(int *)node -> data);
	}	 
	SrtListInsert(&n4, new_srtlist);
	for (node = DLListBegin(new_srtlist -> dll);
		 node != DLListEnd(new_srtlist -> dll);
		 node = DLListNext(node))
	{
		printf("%d, ", *(int *)node -> data);
	}	 
	SrtListInsert(&n5, new_srtlist);
	for (node = DLListBegin(new_srtlist -> dll);
		 node != DLListEnd(new_srtlist -> dll);
		 node = DLListNext(node))
	{
		printf("%d, ", *(int *)node -> data);
	}	 	
*/	
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
