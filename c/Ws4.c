/****************************	
*	Ws4						*
*	If/else, switch , LUT	*
*							*
*	Author: ChenR			*
*	Reviewer: Maoz			*
*	Date: 10/10/2019		*
****************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* exit */

void IfElseFunction();
void SwitchCaseFunction();
void LutCaseFunction();
void LutToption(); /* funtion that prints "pressed T" */
void LutAoption(); /* funtion that prints "pressed A" */
void LutESCoption(); /* funtion that exits program */
void LutNULLoption(); /* funtion that does nothing */

/* below is the LUT table content defined in advance */
#define NULL27 LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption

#define NULL37 LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption

#define NULL18 LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption

#define NULL171 LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption

void IfElseFunction()
{
	char chr = '\0';
	
	printf("\n****************************\n");
	printf("	 If else way			  \n");
	printf("******************************/\n\n");
	
	system("stty -icanon -echo");
	
	while (1)
	{				
		chr = getchar();
		
		if ('A' == chr)
		{
			printf("A pressed\n");
		}
		else if ('T' == chr)
		{
			printf("T pressed\n");
		}
		else if (27 == chr) /* ecsape key */
		{
			system("stty icanon echo");
			exit(0);
		}
		else
		{
			printf("Continue pressing\n");
		}
	}
}

void SwitchCaseFunction()
{
	char chr = '\0';
	
	printf("\n****************************\n");
	printf("	switch case way			  \n");
	printf("*****************************/\n\n");
	
	system("stty -icanon -echo");
	
	while (1)
	{		
		chr = getchar();
		
		switch (chr)
		{
			case 'A':
				printf("A pressed\n");
				break;
				
			case 'T':
				printf("T pressed\n");
				break;
			
			case 27:
				system("stty icanon echo");
				exit(0);
				break;
			
			default:
				printf("Continue pressing\n");
				break;		
		}
	}
}

void LutCaseFunction()
{	
	char chr = '\0';
	static void (*func_p_array[256])() = {NULL27, LutESCoption, NULL37,
				LutAoption, NULL18, LutToption, NULL171};
	
	printf("\n****************************\n");
	printf("	LUT	way 				  \n");
	printf("*****************************/\n\n");
	
	system("stty -icanon -echo");
	
	while (1)
	{		
		chr = getchar();
		(*func_p_array[(unsigned int)chr])();
	}
}

void LutToption()
{
	printf("T pressed\n");
}

void LutAoption()
{
	printf("A pressed\n");
}

void LutESCoption()
{
	system("stty icanon echo");
	exit(0);
}

void LutNULLoption()
{
	printf("Continue pressing\n");
}

int main()
{
	int n = 0;
	
	printf("Please choose what function to test\n");
	printf("1 - if_else, 2 - switch_case, 3 - LUT\n");
	
	while (1)
	{
		scanf("%d", &n);
	
		if (1 == n)
		{
			IfElseFunction();
		}
		else if (2 == n)
		{
			SwitchCaseFunction();
		}
		else if (3 == n)
		{
			LutCaseFunction();
		}
		else
		{
			printf("Choose only 1,2 or 3\n");
		}
	}

	return 0;
}
