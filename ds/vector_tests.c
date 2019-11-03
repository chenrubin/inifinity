#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include "vector.h" /* stack functions */

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define PRINTTESTRESULTS(func,num, res) \
(printf("Function %s: Test %d %-30s\n" KNRM,func, num, (res) == 1 ?\
KGRN "\t\tpassed" : KRED "\t\tfailed"))
#define ELEMENT_SIZE 4 
#define NUM_OF_ELEMENTS 20

void TestVectorPushGetAddr();
void TestVectorPushPopSize();
void TestVectorcheckCapacity();

struct d_vector_t
{
	size_t capacity;
	size_t element_size;
	size_t size;
	void *head;
};
	
int main()
{
	TestVectorPushGetAddr();
	TestVectorPushPopSize();
	TestVectorcheckCapacity();	
	
	
	
	
	
	return 0;
}

/* push one element to vector and get address */
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
}

/* push and then pop one element to/from, check size */
void TestVectorPushPopSize()
{
	float elements[] = {-1025.56, 123.22, 0.1, 33}; 
	size_t len = (sizeof(elements) / sizeof(elements[0]));
	d_vector_t *new_vector = NULL;
	int i = 0;
	float *float_p = NULL;
	const int element_size = 4;
	
	for (i = 0; i < (int)len; ++i)
	{
		float_p = &elements[i];
		new_vector = VectorCreate(NUM_OF_ELEMENTS, element_size);
		PRINTTESTRESULTS("TestStackPushPopSize_size", i + 1, 
						 0 == (VectorSize(new_vector)));
		PRINTTESTRESULTS("TestStackPushPopSize_push", i + 1, 
					  	  0 == VectorPushBack(new_vector, float_p));
		PRINTTESTRESULTS("TestStackPushPopSize_size", i + 1, 
						 1 == (VectorSize(new_vector)));
		VectorPopBack(new_vector);
		PRINTTESTRESULTS("TestStackPushPopSize_size", i + 1, 
						 0 == (VectorSize(new_vector)));
		VectorDestroy(new_vector);
		printf("\n");
	} 
}

void TestVectorcheckCapacity()
{
	unsigned long elements[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,
	0xFFFFFFFFFFFFFFFF, 16,17,18,19,20, 21}; 
	d_vector_t *new_vector = NULL;
	const int element_size = 8;
	int i = 0;
	unsigned long *long_p = NULL;

	new_vector = VectorCreate(20, element_size);
	
	for (i = 0; i < 20; ++i)
	{
		long_p = &elements[i];
		PRINTTESTRESULTS("TestVectorcheckCapacity_push", 1, 
						0 == VectorPushBack(new_vector, long_p));
		PRINTTESTRESULTS("TestVectorcheckCapacity_size", 1, 
					 (size_t)i + 1 == (VectorSize(new_vector)));
	}
	
	long_p = &elements[i];
	PRINTTESTRESULTS("TestVectorcheckCapacity_capacity", 1, 
						 20 == (VectorSize(new_vector)));	 
	PRINTTESTRESULTS("TestVectorcheckCapacity_push", 1, 
						0 == VectorPushBack(new_vector, long_p));
	PRINTTESTRESULTS("TestVectorcheckCapacity_capacity", 1, 
						 40 == (VectorCapacity(new_vector)));
	
	for (i = 0; i < 11; ++i)
	{
		PRINTTESTRESULTS("TestVectorcheckCapacity_size", 1, 
					 (size_t)(21 - i) == (VectorSize(new_vector)));
		VectorPopBack(new_vector);
	}					 
	
	PRINTTESTRESULTS("TestVectorcheckCapacity_capacity", 1, 
						 40 == (VectorCapacity(new_vector)));
						 
	VectorPopBack(new_vector);
	PRINTTESTRESULTS("TestVectorcheckCapacity_capacity", 1, 
						 20 == (VectorCapacity(new_vector)));
						 	
	VectorDestroy(new_vector);
	printf("\n");
}


