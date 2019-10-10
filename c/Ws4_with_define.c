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
void LutToption();
void LutAoption();
void LutESCoption();
void LutNULLoption();

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
		   		
void (*func_p_array[256])() = {FIRSTNULL, LutESCoption, SECONDNULL ,LutAoption, THIRDNULL, LutToption, LASTNULL};

void IfElseFunction()
{
	char chr = '\0';
	
	while (1)
	{		
		printf("\n****************************\n");
		printf("	 If else way			  \n");
		printf("******************************/\n\n");
		
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
			printf("\nContinue pressing\n");
		}
	}
}

void SwitchCaseFunction()
{
	char chr = '\0';
	
	printf("\n****************************\n");
	printf("	switch case way			  \n");
	printf("*****************************/\n\n");
	
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
				printf("\nContinue pressing\n");			
		}
	}
}

void LutCaseFunction()
{	
	char chr = '\0';
	
	printf("\n****************************\n");
	printf("	LUT	way 				  \n");
	printf("*****************************/\n\n");
	
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
	printf("\nContinue pressing\n");
}

int main()
{
	system("stty -icanon -echo");
/*	IfElseFunction();
	SwitchCaseFunction();
*/	LutCaseFunction();

	return 0;
}
