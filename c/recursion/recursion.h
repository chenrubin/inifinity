/****************************************************************************
*                                                                           *
*                               OL79 - 22/12/19                             *
*                                                                           *	
*                                Recursion                                  *
*                                                                           *
*****************************************************************************/

#ifndef ILRD_RECURSION_H
#define ILRD_RECURSION_H

#include "../../ds/stack/stack.h"
#include "../../ds/slist/slist.h"

typedef int (*compare_func)(void *data1, void *data2);
  
/* The function returns the vlue of the nth element in fibonacci series */
unsigned long IterativeFibonacci(unsigned int n);

/* The function returns the vlue of the nth element in fibonacci series */
unsigned long RecursiveFibonacci(unsigned int n);

/* The function returns the length of the string */
size_t Strlen(const char *str);

/* The function compares two strings (pointed by str1 and str2) */
int Strcmp(const char *str1, const char *str2);

/* The function copies the string pointed by src to dest */
char *Strcpy(char *dest, const char *src);

/* The function appends the string pointed to by src to the end of the string pointed to by dest */
char *Strcat(char *dest, const char *src);

/* function finds the first occurrence of the substring needle in the string haystack.
   The terminating '\0' characters are not compared */
char *Strstr(const char *haystack, const char *needle);

/* The function reverses the order of the nodes, from node and onward,
   untill null */
sl_node_t *Flip(sl_node_t *head);

/* The function sorts the stack according to the sorting function.
*	The function receives a compare function, supplied by the user,
*	which should return 1 if data1 is before data2, 0 if they are equal 
*	and -1 if data2 is before data1. */ 
stack_t *SortStack(stack_t *stack,compare_func func);

#endif
