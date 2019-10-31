#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

struct stack_t
{
    size_t elements_size;
    char *head;
    void *tail;
    void *current;
};

stack_t *StackCreate(size_t num_of_elements, size_t elements_size)
{
	size_t num_of_Bytes = num_of_elements * elements_size;
	char *struct_p = (char *)malloc(num_of_elements * elements_size);
	stack_t *new_stack = (stack_t *)malloc(sizeof(struct_p));	
	if (NULL == struct_p)
	{
		printf("no allocation was made to stack\n");
	}
	if (NULL == struct_p)
	{
		printf("no allocation was made to struct\n");
	}
	
	new_stack -> elements_size = elements_size;
	new_stack -> head = struct_p;
	new_stack -> tail = struct_p + num_of_Bytes - 1;
	new_stack -> current = struct_p;
	
	return new_stack;
}

void StackDestroy(stack_t *stack)
{
	free(stack -> head);
	free(stack);	
}

int StackPush(stack_t *stack, const void *n) 
{
	if (stack -> elements_size == x)
	(char *)current_run = stack -> current;
	(char *)n_run = &n;
	int i = 0;
	
	for (i = 0; i < stack -> elements_size; ++i)
	{
		*((char *)current_run + i) = *((char *)n_run + i);
	}	
	memcpy(stack -> current, n, stack -> elements_size);
	
	
	return 0;
}
