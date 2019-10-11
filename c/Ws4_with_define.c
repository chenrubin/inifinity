/****************************	
*	Ws4						*
*	If/else, switch , LUT	*
*							*
*	Author: ChenR			*
*	Reviewer: EyalF			*
*	Date: 10/10/2019		*
****************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* exit */
#include <unistd.h>

void IfElseFunction();
void SwitchCaseFunction();
void LutCaseFunction();
void LutToption(); /* funtion that prints "pressed T" */
void LutAoption(); /* funtion that prints "pressed A" */
void LutESCoption(); /* funtion that exits program */
void LutNULLoption(); /* funtion that does nothing */

/* below is the LUT table content defined in advance */
#define FIRSTNULL LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption

#define SECONDNULL LutNULLoption, LutNULLoption, LutNULLoption, \
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption

#define THIRDNULL LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption

#define LASTNULL LutNULLoption, LutNULLoption, LutNULLoption, LutNULLoption,\
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
		
		if (chr == 'A')
		{
			printf("A pressed\n");
		}
		else if (chr == 'T')
		{
			printf("T pressed\n");
		}
		else if (chr == 27) /* ecsape key */
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
	static void (*func_p_array[256])() = {FIRSTNULL, LutESCoption, SECONDNULL,
				LutAoption, THIRDNULL, LutToption, LASTNULL};
	
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
	printf("\nT pressed\n");
}

void LutAoption()
{
	printf("\nA pressed\n");
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
	
		if (n == 1)
		{
			IfElseFunction();
		}
		else if (n == 2)
		{
			SwitchCaseFunction();
		}
		else if (n == 3)
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
