#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include "slist.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define PRINTTESTRESULTS(func,num, res) \
(printf("Function %s: Test %d %-40s\n" KNRM,func, num, (res) == 1 ?\
KGRN "\t\tpassed" : KRED "\t\tfailed"))
#define ELEMENT_SIZE 4 
#define NUM_OF_ELEMENTS 20

static void PrintSList(node_t *head);
	
int main()
{
	int x = 20;
	int y = 10;
	int z = 15;
	int k = 12;
	node_t *head_after_flip = NULL;
	node_t *second_node = SListCreateNode(&x, NULL);
	node_t *first_node = SListCreateNode(&y, second_node);
/*	node_t *insert_before_node = SListCreateNode(&z, NULL);
	node_t *insert_after_first_node = SListCreateNode(&k, NULL);
	
	SListInsert(insert_before_node, second_node); 
	SListInsertAfter(insert_after_first_node, first_node);
*/	
	PrintSList(first_node);
/*	printf("first node = %d\n", *(int*)first_node->data);
	printf("second node = %d\n", *(int*)(first_node->next)->data);
	printf("third node = %d\n", *(int*)((first_node->next)->next)->data);
	printf("fourth node = %d\n", *(int*)(((first_node->next)->next)->next)->data);
*/	printf("Num of nodes = %ld\n", SListCount(first_node));
	
	
	head_after_flip = SListFlip(first_node);
	PrintSList(head_after_flip);
/*	SListRemove(insert_after_first_node);
	
	PrintSList(first_node);
	printf("first node = %d\n", *(int*)first_node->data);
	printf("second node = %d\n", *(int*)(first_node->next)->data);
	printf("third node = %d\n", *(int*)((first_node->next)->next)->data);
	printf("Num of nodes = %ld\n", SListCount(first_node));
	
	SListRemoveAfter(first_node);
	
	PrintSList(first_node);
	printf("first node = %d\n", *(int*)first_node->data);
	printf("second node = %d\n", *(int*)(first_node->next)->data);
	printf("Num of nodes = %ld\n", SListCount(first_node));
*/				
	SListFreeNode(head_after_flip);
	
	return 0;
}

static void PrintSList(node_t *head)
{
	while (NULL != head -> next)
	{
		printf("%d, ", *(int *)(head -> data));
		head = (head -> next);
	}
	
	printf("%d\n", *(int *)(head -> data));
}

/* push one element to vector and get address *//*
void TestVectorPushGetAddr()
{
	int elements[] = {-1025, 123, 0, 33}; 
	size_t len = (sizeof(elements) / sizeof(elements[0]));
	d_vector_t *new_vector = NULL;
	int i = 0;
	int *int_p = NULL;
	const int element_size = 4;
	
	for (i = 0; i < (int)len; ++i)
	{
		int_p = &elements[i];
		new_vector = VectorCreate(NUM_OF_ELEMENTS, element_size);
		PRINTTESTRESULTS("TestStackPushGetAddr_push", i + 1, 
					  	  0 == VectorPushBack(new_vector, int_p));
		PRINTTESTRESULTS("TestStackPushGetAddr_getAddr", i + 1, 
				elements[i] == *((int *)VectorGetItemAddress(new_vector, 0)));
		VectorDestroy(new_vector);
		printf("\n");
	} 
}*/
