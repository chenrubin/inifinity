#include <stdio.h>
#include <stdlib.h> 
#include "stack.h"

#define PRINTTESTRESULTS(func,num, res) \
(printf("Function %s: Test %d %s\n",func, num, (res) == 1 ?\
 "\tpassed" : "\tfailed"))
#define ELEMENT_SIZE 4
#define NUM_OF_ELEMENTS 20

struct stack_t
{
    size_t elements_size;
    char *head;
    void *tail;
    void *current;
};
	
int main()
{
	stack_t *new_stack = StackCreate(NUM_OF_ELEMENTS, ELEMENT_SIZE);
	
	StackPush(new_stack, (void *)10);
	
	return 0;
}
/*
void TestBArrSetAll()
{
	size_t num1 = 0xFFFFFFFFFFFFFFFF;
	size_t num2 = 0;
	size_t num3 = 0x0123456789ABCDEF;
	
	PRINTTESTRESULTS("BArrSetAll", 1, ((pow(2, BITS_IN_ARRAY) - 1) == 
									 ((double)BArrSetAll(num1))));
									 
	PRINTTESTRESULTS("BArrSetAll", 2, ((pow(2, BITS_IN_ARRAY) - 1) == 
									 ((double)BArrSetAll(num2))));
								
	PRINTTESTRESULTS("BArrSetAll", 3, ((pow(2, BITS_IN_ARRAY) - 1) == 
									 ((double)BArrSetAll(num3))));			 
}
*/
