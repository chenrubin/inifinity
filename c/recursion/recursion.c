/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		recursion					  *
*		7/11/2019					  *
*									  *
************************************/

#include <stdio.h> /* printf */
#include <string.h> /*RecStrcmp, RecStrlen, RecStrcpy, RecStrcat, RecStrstr */
#include <stdlib.h> /*malloc*/
#include <assert.h> /*malloc*/

#include "recursion.h"
#include "MyUtils.h" /* MAX2,MIN2 */

static void CopyElementAndPopFromStackIMP(void *element , stack_t *stack, size_t element_size);

unsigned long IterativeFibonacci(unsigned int n)
{
	unsigned long i = 0;
	unsigned long lower_element = 0;
	unsigned long higher_element = 1;
	unsigned long res = 0;
	
	if (0 == n || 1 == n)
	{
		return n;
	}
	else
	{
		for (i = 2; i <= n; ++i)
		{
			res = lower_element + higher_element;
			lower_element = higher_element;
			higher_element = res;
		}
	}
	
	return res;
}

unsigned long RecursiveFibonacci(unsigned int n)
{
	if (0 == n || 1 == n)
	{
		return n;
	}
	else
	{
		return RecursiveFibonacci(n - 1) + RecursiveFibonacci(n - 2);
	}
}

size_t RecStrlen(const char *str)
{
	if ('\0' == *str)
	{
		return 0;
	}
	else
	{
		return (1 + RecStrlen(str + 1));
	}
}

int RecStrcmp(const char *str1, const char *str2)
{
	assert(str1);
	assert(str2);
	
	if ('\0' == *str1 || '\0' == *str2)
	{
		return (*str1 - *str2);
	}
	else
	{
		return (RecStrcmp(str1 + 1, str2 + 1) | (*str1 - *str2));
	}
}

char *RecStrcpy(char *dest, const char *src)
{
	assert(dest);
	assert(src);
	
	if ('\0' == *src)
	{
		*dest  = *src;
	}
	else
	{
		return (RecStrcpy(dest + 1, src + 1));
	}
	
	return dest;
}

char *RecStrcat(char *dest, const char *src)
{	
	assert(dest);
	assert(src);
	
	*(dest + RecStrlen(dest)) = *src;
	
	if ('\0' == *src)
	{
		return dest;
	}
	else
	{
		return (RecStrcat(dest, src + 1));
	}
}

char *RecStrstr(const char *haystack, const char *needle)
{
	assert(haystack);
	assert(needle);
	
	if ('\0' == *needle)
	{
		return (char *)haystack;
	}
	else if ('\0' == *haystack)
	{
		return NULL;
	}
	if (*needle == *haystack && 
	   (RecStrstr(haystack + 1, needle + 1) == haystack + 1))
	{
		return (char *)haystack;
	}
	
	return(RecStrstr(haystack + 1, needle));
}

sl_node_t *RecFlip(sl_node_t *head)
{	
	sl_node_t *temp = NULL;
	sl_node_t *begin = NULL;

	assert(head);
		
	if (NULL == (head -> next))
	{
		return head;
	}
	else
	{
		temp = head -> next;
		begin = RecFlip(head -> next);
		temp -> next = head;
		head -> next = NULL;
		return begin;
	}
}

stack_t *RecSortStack(stack_t *stack, size_t element_size, cmp_func_t func)
{
	int compare = 0;
	void *element1 = NULL;
	void *element2 = NULL;
	
	assert(stack);
	assert(func);
		
	element1 = (void *)malloc(element_size);
	if (NULL == element1)
	{
		return NULL;
	}
	element2 = (void *)malloc(element_size);
	if (NULL == element2)
	{
		return NULL;
	}

	if (1 == StackSize(stack))
	{
		free(element1);
		free(element2);
		
		return stack;
	}
	
	CopyElementAndPopFromStackIMP(element1 , stack, element_size);
	RecSortStack(stack, element_size, func);
	compare = func(element1, StackPeek(stack));
	if (compare <= 0)
	{
		StackPush(stack, element1);
	}
	else
	{
		CopyElementAndPopFromStackIMP(element2 , stack, element_size);
		StackPush(stack, element1);
		StackPush(stack, element2);
		RecSortStack(stack, element_size, func);
	}
	
	free(element1);
	free(element2);
	
	return stack;
}

static void CopyElementAndPopFromStackIMP(void *element , stack_t *stack, size_t element_size)
{
	memcpy(element,StackPeek(stack), element_size);
	StackPop(stack);
}
