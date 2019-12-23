/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		recursion					  *
*		7/11/2019					  *
*									  *
************************************/

#include <stdio.h> /* printf */
#include <string.h> /*strcmp, strlen, strcpy, strcat, strstr */

#include "recursion.h"
#include "MyUtils.h" /* MAX2,MIN2 */

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

size_t Strlen(const char *str)
{
	if ('\0' == *str)
	{
		return 0;
	}
	else
	{
		return (1 + Strlen(str + 1));
	}
}

int Strcmp(const char *str1, const char *str2)
{
	if ('\0' == *str1 || '\0' == *str2)
	{
		return (*str1 - *str2);
	}
	else
	{
		return (Strcmp(str1 + 1, str2 + 1) | (*str1 - *str2));
	}
}

char *Strcpy(char *dest, const char *src)
{
	if ('\0' == *src)
	{
		*dest  = *src;
	}
	else
	{
		return (Strcpy(dest + 1, src + 1));
	}
	
	return dest;
}

char *Strcat(char *dest, const char *src)
{	
	*(dest + strlen(dest)) = *src;
	
	if ('\0' == *src)
	{
		return dest;
	}
	else
	{
		return (Strcat(dest, src + 1));
	}
}

char *Strstr(const char *haystack, const char *needle)
{
	if ('\0' == *needle)
	{
		return (char *)haystack;
	}
	else if ('\0' == *haystack)
	{
		return NULL;
	}
	if (*needle == *haystack && 
	   (Strstr(haystack + 1, needle + 1) == haystack + 1))
	{
		return (char *)haystack;
	}
	
	return(Strstr(haystack + 1, needle));
}

sl_node_t *Flip(sl_node_t *head)
{
	sl_node_t *temp = NULL;
	sl_node_t *begin = NULL;
	
	if (NULL == (head -> next))
	{
		return head;
	}
	else
	{
		temp = head -> next;
		begin = Flip(head -> next);
		temp -> next = head;
		head -> next = NULL;
		return begin;
	}
}

stack_t *SortStack(stack_t *stack,compare_func func)
{
	stack_t *second_stack = StackCreate(StackSize(stack), sizeof(int));
}
