#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */
#include "stack.h" /* stack functions */

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
	char *actual_stack_p = (char *)malloc(num_of_elements * elements_size);
	stack_t *new_stack = (stack_t *)malloc(num_of_Bytes);	
	if (NULL == actual_stack_p)
	{
		printf("no allocation was made to stack\n");
	}
	if (NULL == new_stack)
	{
		printf("no allocation was made to struct\n");
	}
	
	assert(0 != num_of_elements);
	assert(0 != elements_size);
	
	new_stack -> elements_size = elements_size;
	new_stack -> head = actual_stack_p;
	new_stack -> tail = actual_stack_p + num_of_Bytes - 1;
	new_stack -> current = actual_stack_p;
	
	return new_stack;
}

void StackDestroy(stack_t *stack)
{
	assert(NULL != stack);
	
	free(stack -> head);
	free(stack);	
}

int StackPush(stack_t *stack, const void *n) 
{
	assert(NULL != stack);
	
	if (stack -> current == stack -> tail)
	{
		printf("Stack is full\n");
		
		return 1;
	}
	else
	{
		memcpy((stack -> current), n, stack -> elements_size);
		stack -> current = (char *)(stack -> current) + stack -> elements_size;
	}
	
	return 0;
}

void *StackPeek(const stack_t *stack)
{
	assert(NULL != stack);
	
	return ((char *)(stack -> current) - (stack -> elements_size));
}

void StackPop(stack_t *stack)
{
	assert(NULL != stack);
	
	stack -> current = (char *)(stack -> current) - (stack -> elements_size);
}

int StackIsEmpty(const stack_t *stack)
{
	assert(NULL != stack);
	
	return ((stack -> current) == (stack -> head));
}

size_t StackSize(const stack_t *stack)
{
	size_t num_of_consumed_bytes = 
	(char *)(stack -> current) - (char *)(stack -> head);
	
	assert(NULL != stack);
	
	return (num_of_consumed_bytes / (stack -> elements_size));
}


