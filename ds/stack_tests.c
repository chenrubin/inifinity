#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include "stack.h" /* stack functions */

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define PRINTTESTRESULTS(func,num, res) \
(printf("Function %s: Test %d %10s\n" KNRM,func, num, (res) == 1 ?\
KGRN "\t\tpassed" : KRED "\t\tfailed"))
#define ELEMENT_SIZE 4 
#define NUM_OF_ELEMENTS 20

void TestStackPushPeek();
void TestStackPopIsEmpty();
void TestStackfull();
void TestStackDifferentDataTypes();

struct stack_t
{
    size_t elements_size;
    char *head;
    void *tail;
    void *current;
};
	
int main()
{
	TestStackPushPeek();
	TestStackPopIsEmpty();
	TestStackfull();
	TestStackDifferentDataTypes();
	
	return 0;
}

/* push one element to stack and peek on it*/
void TestStackPushPeek()
{
	int elements[] = {-1025, 123, 0, 33}; 
	size_t len = (sizeof(elements) / sizeof(elements[0]));
	stack_t *new_stack = NULL;
	int i = 0;
	int *int_p = NULL;
	const int element_size = 4;
	
	for (i = 0; i < (int)len; ++i)
	{
		int_p = &elements[i];
		new_stack = StackCreate(NUM_OF_ELEMENTS, element_size);
		PRINTTESTRESULTS("TestStackPushPeek_push", i + 1, 
					  	  0 == StackPush(new_stack, int_p));
		PRINTTESTRESULTS("TestStackPushPeek_peek", i + 1, 
					  	  elements[i] == *((int *)StackPeek(new_stack)));
		StackDestroy(new_stack);
		printf("\n");
	} 
}

/* verify IsEmpty, push, !IsEmpty, pop, IsEmpty*/
void TestStackPopIsEmpty()
{
	int elements[] = {-1025, 123, 0, 33}; 
	size_t len = (sizeof(elements) / sizeof(elements[0]));
	stack_t *new_stack = NULL;
	int i = 0;
	int *int_p = NULL;
	const int element_size = 4;
	
	for (i = 0; i < (int)len; ++i)
	{
		int_p = &elements[i];
		new_stack = StackCreate(NUM_OF_ELEMENTS, element_size);
		
		PRINTTESTRESULTS("TestStackPopIsEmpty_IsEmpty", i + 1,
					  	  0 == StackIsEmpty(new_stack));
		PRINTTESTRESULTS("TestStackPopIsEmpty_push", i + 1, 
					  	  0 == StackPush(new_stack, int_p));
		PRINTTESTRESULTS("TestStackPopIsEmpty_IsEmpty", i + 1, 
					  	  1 == StackIsEmpty(new_stack));
		StackPop(new_stack);
		
		PRINTTESTRESULTS("TestStackPopIsEmpty_IsEmpty", i + 1, 
					  	  0 == StackIsEmpty(new_stack));
		StackDestroy(new_stack); 
		printf("\n");			  	  
	} 
}

/* Fill stack to full capasity. Then empty it all the way.*/
void TestStackfull()
{
	float elements[] = {-1025.23, 123.456, 0.1, 33, 256, -2.5, 10, 253.4897,
	5.5489, 258796.54, 598, 561.2645, 5.8, 65.48, 653.5, 16.48, -89,45, 101.2,
	123.465}; 
	size_t len = (sizeof(elements) / sizeof(elements[0]));
	stack_t *new_stack = NULL;
	int i = 0;
	float *float_p = NULL;
	const int element_size = 4;
	
	new_stack = StackCreate(NUM_OF_ELEMENTS, element_size);

	/* fill all the way */
	for (i = 0; i < (int)len; ++i)
	{
		float_p = &elements[i];
		
		if (0 == i)		
		{
			PRINTTESTRESULTS("TestFillStack_IsEmpty", i + 1, 
					  	  0 == StackIsEmpty(new_stack));
		}
		else
		{
			PRINTTESTRESULTS("TestFillStack_IsEmpty", i + 1, 
					  	  1 == StackIsEmpty(new_stack));
		}
		
		PRINTTESTRESULTS("TestFillStack_push", i + 1, 
					  	  0 == StackPush(new_stack, float_p));
		
		PRINTTESTRESULTS("TestFillStack_size", i + 1, 
					  	  (size_t)(i + 1) == StackSize(new_stack));			  	  
					  	  			  	  
		printf("\n");			  	  
	}
	
	for (i = 0; i < (int)len; ++i)
	{
		PRINTTESTRESULTS("TestEmptyStack_size", i + 1, 
					  	  (size_t)((int)len - i) == StackSize(new_stack));
		
		StackPop(new_stack);
		
		if (i == ((int)len - 1))		
		{
			PRINTTESTRESULTS("TestEmptyStack_IsEmpty", i + 1, 
					  	  0 == StackIsEmpty(new_stack));
		}
		else
		{
			PRINTTESTRESULTS("TestEmptyStack_IsEmpty", i + 1, 
					  	  1 == StackIsEmpty(new_stack));
		}
	
		printf("\n");			  	  
	}
	
	StackDestroy(new_stack);
}

/* verify stack with different data types */
void TestStackDifferentDataTypes()
{
	size_t element_size1 = 8;
	size_t size_t_element = 56;
	long long_element = 4891489416489;
	char ch = 5;
	stack_t *stack_t_pointer = StackCreate(NUM_OF_ELEMENTS, 1);
	size_t *size_t_pointer = &size_t_element;
	long *long_pointer = &long_element;
	char *char_pointer = &ch;
	stack_t *new_stack = NULL;
	
	/* size_t */
	new_stack = StackCreate(NUM_OF_ELEMENTS, element_size1);
	
	PRINTTESTRESULTS("TestStackDifferentDataTypes_sizeT_size", 1, 
					  	  0 == StackSize(new_stack));
	PRINTTESTRESULTS("TestStackDifferentDataTypes_sizeT_push", 2, 
				  	  0 == StackPush(new_stack, size_t_pointer));
	PRINTTESTRESULTS("TestStackDifferentDataTypes_sizeT_size", 3, 
					  	  1 == StackSize(new_stack));			  	  
	PRINTTESTRESULTS("TestStackDifferentDataTypes_sizeT_peek", 4, 
				  	  *size_t_pointer == *((size_t *)StackPeek(new_stack)));
	PRINTTESTRESULTS("TestStackDifferentDataTypes_sizeT_size", 5, 
					  	  1 == StackSize(new_stack));
	StackPop(new_stack);
	PRINTTESTRESULTS("TestStackDifferentDataTypes_sizeT_size", 6, 
					  	  0 == StackSize(new_stack));
					  	  				  	  
	StackPop(new_stack);
	StackDestroy(new_stack);
	printf("\n");
	
	/* long */
	new_stack = StackCreate(NUM_OF_ELEMENTS, element_size1);
	
	PRINTTESTRESULTS("TestStackDifferentDataTypes_long_size", 1,
					  	  0 == StackSize(new_stack));
	PRINTTESTRESULTS("TestStackDifferentDataTypes_long_push", 2,
				  	  0 == StackPush(new_stack, long_pointer));
	PRINTTESTRESULTS("TestStackDifferentDataTypes_long_size", 3,
					  	  1 == StackSize(new_stack));			  	  
	PRINTTESTRESULTS("TestStackDifferentDataTypes_long_peek", 4,
		*long_pointer == *((long *)StackPeek(new_stack)));
	PRINTTESTRESULTS("TestStackDifferentDataTypes_long_size", 5,
					  	  1 == StackSize(new_stack));
	StackPop(new_stack);
	PRINTTESTRESULTS("TestStackDifferentDataTypes_long_size", 6,
					  	  0 == StackSize(new_stack));
					  	  				  	  
	StackPop(new_stack);
	StackDestroy(new_stack);
	printf("\n");
	
	/* char */
	new_stack = StackCreate(NUM_OF_ELEMENTS, element_size1);
	
	PRINTTESTRESULTS("TestStackDifferentDataTypes_char_size", 1,
					  	  0 == StackSize(new_stack));
	PRINTTESTRESULTS("TestStackDifferentDataTypes_char_push", 2,
				  	  0 == StackPush(new_stack, char_pointer));
	PRINTTESTRESULTS("TestStackDifferentDataTypes_char_size", 3,
					  	  1 == StackSize(new_stack));			  	  
	PRINTTESTRESULTS("TestStackDifferentDataTypes_char_peek", 4,
		*char_pointer == *((char *)StackPeek(new_stack)));
	PRINTTESTRESULTS("TestStackDifferentDataTypes_char_size", 5,
					  	  1 == StackSize(new_stack));
	StackPop(new_stack);
	PRINTTESTRESULTS("TestStackDifferentDataTypes_char_size", 6,
					  	  0 == StackSize(new_stack));
					  	  				  	  
	StackPop(new_stack);
	StackDestroy(new_stack);
	printf("\n");
	
	/* stack_t */
	new_stack = StackCreate(NUM_OF_ELEMENTS, element_size1);
	
	PRINTTESTRESULTS("TestStackDifferentDataTypes_stackT_size", 1,
					  	  0 == StackSize(new_stack));
	PRINTTESTRESULTS("TestStackDifferentDataTypes_stackT_push", 2,
				  	  0 == StackPush(new_stack, stack_t_pointer));
	PRINTTESTRESULTS("TestStackDifferentDataTypes_stackT_size", 3,
					  	  1 == StackSize(new_stack));
					  	  
	stack_t_pointer = (stack_t *)StackPeek(new_stack);
					  	  
	PRINTTESTRESULTS("TestStackDifferentDataTypes_stackT_peek_current", 4,
		((char *)(stack_t_pointer -> current)) == 
		((char *)(((stack_t *)StackPeek(new_stack)) -> current)));
	PRINTTESTRESULTS("TestStackDifferentDataTypes_stackT_peek_head", 5,
		((char *)(stack_t_pointer -> head)) == 
		((char *)(((stack_t *)StackPeek(new_stack)) -> head)));
	PRINTTESTRESULTS("TestStackDifferentDataTypes_stackT_peek_tail", 6,
		((char *)(stack_t_pointer -> tail)) == 
		((char *)(((stack_t *)StackPeek(new_stack)) -> tail)));
	PRINTTESTRESULTS("TestStackDifferentDataTypes_stackT_peek_elementSize", 7,
		((size_t *)(stack_t_pointer -> elements_size)) == 
		((size_t *)(((stack_t *)StackPeek(new_stack)) -> elements_size)));			
		
	PRINTTESTRESULTS("TestStackDifferentDataTypes_stackT_size", 8,
					  	  1 == StackSize(new_stack));
	StackPop(new_stack);
	PRINTTESTRESULTS("TestStackDifferentDataTypes_char_size", 9,
					  	  0 == StackSize(new_stack));
					  	  				  	  
	StackPop(new_stack);
	StackDestroy(new_stack);
	printf("\n");
}

