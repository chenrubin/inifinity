/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		recursion					  *
*		7/11/2019					  *
*									  *
************************************/

#include <stdio.h> /* printf */

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
