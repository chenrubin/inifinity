#include <stdio.h> /* printf */
#include <stdlib.h> /* exit */

void IfElseFunction();
void SwitchCaseFunction();
void LutCaseFunction();
/*void (*pf[])() CreateLutTable();*/
void LutToption();
void LutAoption();
void LutESCoption();
void LutNULLoption();

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
			exit(0);
		}
	}
}

void SwitchCaseFunction()
{
	char chr = '\0';
	
	printf("\n****************************\n");
	printf("	If else way				  \n");
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
				exit(0);
				break;			
		}
	}
}

void LutCaseFunction()
{	
	void (*func_p_array[256])();
	int i = 0;
	char chr = '\0';
	
	printf("\n****************************\n");
	printf("	LUT	way 				  \n");
	printf("*****************************/\n\n");
	
	for (i = 0 ; i < 256 ; ++i)
	{
		func_p_array[i] = LutNULLoption;
	}
	
	func_p_array['A'] = LutAoption;
	func_p_array['T'] = LutToption;
	func_p_array[27] = LutESCoption;
	
	
	while (1)
	{		
		chr = getchar();
		
		(*func_p_array[(unsigned int)chr])();
	}
}
/*
void (*pf[])() CreateLutTable()
{
	void (*pf[256])(int);
	
	*(pf + 'A') = LutAoption();
	*(pf + 'T') = LutToption();
	*(pf + 27) = LutESCoption();
	
	return pf;
}
*/
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
	exit(0);
}

void LutNULLoption()
{
	;
}

int main()
{
	IfElseFunction();
	SwitchCaseFunction();
	LutCaseFunction();

	return 0;
}
