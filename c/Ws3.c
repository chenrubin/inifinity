/********************************************
*	Author : ChenR							*
*	Reviewer : EyalF						*
*	WS3 									*
*	inluding following tests:				*
*											*
*	test copy of envioronment variables		*
*	testing Jusiphus algorithm				*
*	printing c types						*
*											*									
********************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc/ calloc */
#include <string.h> /* strcmp, strcpy */
#include <ctype.h> /* tolower */
#include <assert.h> /* assert */

void Printenv(char *env[]);
char **Copyenv(char *env[], int counter);
void FreeEnv(char *env[]);
void FreeEnvExtra(char *env[], int location);
char *Mycalloc(char **env, int location);
int Josephus(int num);
int JosephusBetter(int num);

void TestCopyEnv();
void TestJosephus();
void Testctypes();


void Printenv(char *env[]) /* print env */
{
	int i = 0;
	
	while (0 != *(env + i))
	{
		printf("%s\n", *(env + i));
		++i;
	}
}

char **Copyenv(char *env[], int counter) /* copy env and chanhge to lower */
{
	int pointer_index = 0;
	int string_index = 0;
	
	/* array of pointer to char */
	char **cp_env = (char **)calloc(counter + 1, sizeof(char *));
	
	/* each string has malloc */
	for (pointer_index = 0 ; pointer_index < counter ; ++pointer_index) /* need to change */
	{
		*(cp_env + pointer_index) = 
		(char *)calloc(strlen(*(env + pointer_index)), sizeof(*(env + pointer_index)));
	}
	
	/* copy everything */
	for (pointer_index = 0 ; pointer_index < counter ; ++pointer_index)
	{
		*(cp_env + pointer_index) = strcpy(*(cp_env + pointer_index), *(env + pointer_index));
	}
	
	/* change to lower */
	for (pointer_index = 0 ; pointer_index < counter ; ++pointer_index)
	{
		string_index = 0;
		
		/* iterate over each string and change */ 
		while ('\0' != *(*(cp_env + pointer_index) + string_index))
		{
			*(*(cp_env + pointer_index) + string_index) = 
			tolower(*(*(env + pointer_index) + string_index));
			++string_index;
		}
	}
	
	return cp_env;	
}

char **CopyenvExtra(char *env[], int counter) /* copy env and chanhge to lower */
{
	int pointer_index = 0;
	int string_index = 0;
	
	/* array of pointer to char */
	char **cp_env = (char **)calloc(counter + 1, sizeof(char *));
	
	/* each string has malloc */
	for (pointer_index = 0 ; pointer_index < counter ; ++pointer_index) /* need to change */
	{						
		*(cp_env + pointer_index) = Mycalloc(env, pointer_index);
		
		if (NULL == *(cp_env + pointer_index))
		{
			printf("pointer index = %d got NULL\n", pointer_index);
			FreeEnvExtra(cp_env, pointer_index);
			return NULL;
		}
	}
	
	/* copy everything */
	for (pointer_index = 0 ; pointer_index < counter ; ++pointer_index)
	{
		*(cp_env + pointer_index) = strcpy(*(cp_env + pointer_index), *(env + pointer_index));
	}
	
	/* change to lower */
	for (pointer_index = 0 ; pointer_index < counter ; ++pointer_index)
	{
		string_index = 0;
		
		/* iterate over each string and change */ 
		while ('\0' != *(*(cp_env + pointer_index) + string_index))
		{
			*(*(cp_env + pointer_index) + string_index) = 
			tolower(*(*(env + pointer_index) + string_index));
			++string_index;
		}
	}
	
	return cp_env;	
}

void FreeEnvExtra(char *env[], int location) /* free env */
{
	int pointer_index = 0;
			
	while (pointer_index <= location)
	{
		printf("Free pointer_index = %d\n", pointer_index);
		free(*(env + pointer_index));
		++pointer_index;
	}
	
	free(env);
}

void FreeEnv(char *env[]) /* free env */
{
	int pointer_index = 0;
			
	while (0 != *(env + pointer_index))
	{
		free(*(env + pointer_index));
		++pointer_index;
	}
	
	free(env);
}

char *Mycalloc(char **env, int location)
{
	if (33 == location)
	{
		return NULL;
	}
	else
	{
		return ((char *)calloc(strlen(*(env + location)), 
							   sizeof(*(env + location))));
	}
}

void printArray(int *a, int len)
{
	int i = 0;
	for (i = 0 ; i < len ; ++i)
	{
		printf("%d ", *(a + i));
		if (i == len - 1)
		{
			printf("\n\n");	
		}
	}
}

int JosephusBetter(int num)
{
	int **circle = (int **)malloc(num * sizeof(int *));
	int **runner1 = circle;
	int **runner2 = circle;
	int i = 0;	
	
	for (i = 0; i < num; ++i)
	{
		*(circle + i) = (int *)(circle + ((i + 1) % num));
	}
	
	while (*runner1 != (int *)runner1)
	{
		runner2 = (int **)*runner1; /* runner 2 is now to be killed */
		*runner1 = *runner2;
		runner1 = (int **)*runner1;
	}
	
	i = (int)(runner1 - circle + 1);
	
	free(circle);
	
	return i;	
}

int Josephus(int num)
{
	int i = 0;
	int dead = 0;
	int *circle = (int *)malloc(num * sizeof(num));
	
	assert(0 < num);
	
	if (num == 1)
	{
		free(circle);
		return 1;
	}
	else
	{
		for (i = 0 ; i < num ; ++i) /* all soldiers are alive */
		{
			*(circle + i) = 1;
		}

		i = 0;
		while (dead < (num - 1))
		{		
			do
			{			
				i = ((i + 1)%num);
			}
			while (*(circle + i) == 0); /* while they are dead */

			*(circle + i) = 0; /* kill */
			++dead;
			i = (i + 1)%num;
			
			while (*(circle + i) == 0) /* while they are dead */
			{	
				i = ((i + 1)%num);
			}
		}
	}
	
	for (i = 0 ; i < num ; i++)
	{
		if (circle[i] == 1)
		{
			free(circle);
			return ((i + 1) % num);
		}
	}
	
	return 0;	
}

void TestCopyEnv(int argc, char *argv[], char *env[])
{
	int counter = 0;
	char **new_env = NULL;
	
	printf("/******************************************************************\n");
	printf("			Testing coppy env										\n");		
	printf("*******************************************************************/\n\n");
	
	while (0 != *(env + counter)) /* determine length */
	{
		++counter;
	}
	
	new_env = Copyenv(env, counter);
	Printenv(env);
	printf("\n\n");
	Printenv(new_env);
	FreeEnv(new_env);
			
	/* in case you wish to test below put the rest of functio in a comment */
	
/*	printf("!!!attempt with memory free control!!!\n\n");
	
	new_env = CopyenvExtra(env, counter); */	
	
	++argc;
	++argv;
}

void TestJosephus()
{
	#define num1 100
	#define num2 1000
	#define num3 1000000
	#define num4 1
	#define num5 9
	
	int res1 = 0;
	int res2  = 0;
	
	printf("/******************************************************************\n");
	printf("			Testing Josephus										\n");		
	printf("*******************************************************************/\n\n");
	
	res1 = Josephus(num1);
	res2 = JosephusBetter(num1);
	
	if (res1 == res2)
	{
		printf("PASSED for %d\n", num1);
	}
	else
	{
		printf("FAILED for %d\n", num1);
		printf("res1 = %d, res2 = %d\n", res1, res2);
	}
	
	res1 = Josephus(num2);
	res2 = JosephusBetter(num2);
	
	if (res1 == res2)
	{
		printf("PASSED for %d\n", num2);
	}
	else
	{
		printf("FAILED for %d\n", num2);
		printf("res1 = %d, res2 = %d\n", res1, res2);
	}
	
	res1 = Josephus(num3);
	res2 = JosephusBetter(num3);
	
	if (res1 == res2)
	{
		printf("PASSED for %d\n", num3);
	}
	else
	{
		printf("FAILED for %d\n", num3);
		printf("res1 = %d, res2 = %d\n", res1, res2);
	}
	
	res1 = Josephus(num4);
	res2 = JosephusBetter(num4);
	
	if (res1 == res2)
	{
		printf("PASSED for %d\n", num4);
	}
	else
	{
		printf("FAILED for %d\n", num4);
		printf("res1 = %d, res2 = %d\n", res1, res2);
	}
	
	res1 = Josephus(num5);
	res2 = JosephusBetter(num5);
	
	if (res1 == res2)
	{
		printf("PASSED for %d\n", num5);
	}
	else
	{
		printf("FAILED for %d\n", num5);
		printf("res1 = %d, res2 = %d\n", res1, res2);
	}
}

void Testctypes()
{
	printf("/******************************************************************\n");
	printf("			Testing c types										\n");		
	printf("*******************************************************************/\n\n");
	
	printf("sizeof(char) = %ld\n", sizeof(char));
	printf("sizeof(unsigned char) = %ld\n", sizeof(char));
	printf("sizeof(int) = %ld\n", sizeof(int));
	printf("sizeof(unsigned int) = %ld\n", sizeof(unsigned));
	printf("sizeof(short) = %ld\n", sizeof(short));
	printf("sizeof(unsigned short) = %ld\n", sizeof(unsigned short));
	printf("sizeof(long) = %ld\n", sizeof(long));
	printf("sizeof(unsigned long) = %ld\n", sizeof(unsigned long));
	printf("sizeof(float) = %ld\n", sizeof(float));
	printf("sizeof(double) = %ld\n", sizeof(double));
	printf("sizeof(long double) = %ld\n", sizeof(long double));
	printf("sizeof(void *) = %ld\n\n", sizeof(void *));
}

int main(int argc, char *argv[], char *env[])
{
	TestCopyEnv(argc, argv, env);
	
	TestJosephus();
	
	Testctypes();
	
	return 0;
}
