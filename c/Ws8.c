/********************************
*	Author: ChenR				*
*	Reviewer: Dvir				*
*	Ws8							*
*	Structs						*
*								*
********************************/

#include <stdio.h> /* printf */
#include <string.h> /* strcpy */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

/* Excersize 2 */
#define MAX2(a,b) (a) > (b) ? a : b
#define MAX3(a,b,c) (a) > (b) ? (a > c ? a : c) : (b > c ? b : c)

/* Excersize 3 */
#define SIZEOF_VAR(x) (char *)(&x+1) - (char *)(&x)

/* Excersize 4 */
#define SIZEOF_TYPE(type) (long)((type *)0 + 1)

#define SIZE 3
#define ALLOCATED_SIZE 20
#define checkResultOneNum(num, res, expres)\
		{\
			if (res == expres) \
			{\
				printf("PASSED for %d\n", num);\
			}\
			else \
			{ \
				printf("FAILED for %d\n", num);\
				printf("res = %ld, expres = %ld\n", res, expres);\
			}\
		}

#define checkResultTwoNum(num1, num2, res, expres)\
		{\
			if (res == expres)\
			{\
				printf("PASSED for %d, %d\n", num1, num2);\
			} \
			else \
			{ \
				printf("FAILED for %d, %d\n", num1, num2);\
				printf("res = %d, expres = %d\n", res, expres);\
			}\
		}
		
#define checkResultThreeNum(num1, num2, num3, res, expres)\
		{\
			if (res == expres)\
			{\
				printf("PASSED for %d, %d, %d\n", num1, num2, num3);\
			}\
			else \
			{\
				printf("FAILED for %d, %d, %d\n", num1, num2, num3);\
				printf("res = %d, expres = %d\n", res, expres);\
			}\
		}					
/* struct which will be used for Excersize 1 */
typedef struct element
{
	void *data;
	void(*PrintFunc)(void *);
	void(*AddFunc)(void **, int);
} element_t;

/* printf for integer */
void PrintElementInt(void *data);

/* printf for float */
void PrintElementFloat(void *data);

/* printf for string */
void PrintElementString(void *data);

/* Add int variable */
void MathAddInt(void **data, int add);

/* Add float variable */
void MathAddFloat(void **data, int add);

/* Add variable to string*/
void StringAdd(void **data, int add);

/* free allocated memory */
void FreeElement(void *data);

/* Test functions */
void TestElementArray();
void TestMaxMacros();
void TestSizeofVarMacro();
void TestSizeofTypeMacro();

int main()
{	
	TestElementArray();
	TestMaxMacros();
	TestSizeofVarMacro();
	TestSizeofTypeMacro();	

	return 0;
} 

void PrintElementInt(void *data)
{
	assert(data);
	printf("%d\n", *(int *)&data);
}

void PrintElementFloat(void *data)
{
	assert(data);
	printf("%f\n", *(float *)&data);
}

void PrintElementString(void *data)
{
	assert(data);
	printf("%s\n", (char *)data);
}

void MathAddInt(void **data, int add)
{
	assert(data);
	*((int *)data) += add;
}

void MathAddFloat(void **data, int add)
{
	assert(data);
	*((float *)data) += (float)add;
}

void StringAdd(void **data, int add)
{
	assert(data);
	*data = realloc(*data, sizeof(strlen((char *)*data) + 10));
	sprintf(*data, "%s%d", (char *)*data, add);
}

void FreeElement(void *data)
{
	assert(data);
	free(data);
}

void TestElementArray()
{
	int i = 0;
	element_t e_array[SIZE];
	
	printf("\n/*******************************************\n");
	printf("		Testing element array				    ");
	printf("\n*******************************************/\n");
	
	*((int *)&(e_array[0].data)) = 10;
	e_array[0].PrintFunc = PrintElementInt;
	e_array[0].AddFunc = MathAddInt;
	
	*((float *)&(e_array[1].data)) = 2.5;
	e_array[1].PrintFunc = PrintElementFloat;
	e_array[1].AddFunc = MathAddFloat;
	
	e_array[2].data = malloc(ALLOCATED_SIZE * sizeof(char));
	if (NULL == e_array[2].data)
	{
		printf("returned NULL\n");
	}
	strcpy(e_array[2].data, "Hello world!!!");
	e_array[2].PrintFunc = PrintElementString;
	e_array[2].AddFunc = StringAdd;
	
	for (i = 0; i < SIZE; ++i)
	{		
		e_array[i].PrintFunc(e_array[i].data);
	}
	
	for (i = 0; i < SIZE; ++i)
	{		
		e_array[i].AddFunc(&(e_array[i].data), 10);
	}
	
	for (i = 0; i < SIZE; ++i)
	{	
		e_array[i].PrintFunc(e_array[i].data);
	}
	
	free(e_array[2].data);
}

void TestMaxMacros()
{
	int i = 0;
	int input1_max2[] = {3,2,-1};
	int input2_max2[] = {2,3,10};
	int input1_max3[] = {3,-9,3};
	int input2_max3[] = {-2,3,7};
	int input3_max3[] = {7,2,9000};
	int expres_max2[] = {3,3,10};
	int expres_max3[] = {7,3,9000};
	
	printf("\n/*******************************************\n");
	printf("		Testing MAX macros				    	");
	printf("\n*******************************************/\n");
	
	for (i = 0; i < SIZE; ++i)
	{
		checkResultTwoNum(input1_max2[i], input2_max2[i],
						MAX2(input1_max2[i], input2_max2[i]),expres_max2[i]);
	}
	for (i = 0; i < SIZE; ++i)
	{
		checkResultThreeNum(input1_max3[i], input2_max3[i], input3_max3[i],
						MAX3(input1_max3[i], input2_max3[i], input3_max3[i]),
						expres_max3[i]);
	}
}

void TestSizeofVarMacro()
{
	int x = 20;
	float y = 20;
	long z = 20;
	
	printf("\n/*******************************************\n");
	printf("		Testing SIZEOF_VAR macro		    	");
	printf("\n*******************************************/\n");
	
	checkResultOneNum(x, SIZEOF_VAR(x), sizeof(x));
	checkResultOneNum(x, SIZEOF_VAR(y), sizeof(y));
	checkResultOneNum(x, SIZEOF_VAR(z), sizeof(z));
}

void TestSizeofTypeMacro()
{
	printf("\n/*******************************************\n");
	printf("		Testing SIZEOF_TYPE macro		    	");
	printf("\n*******************************************/\n");
	
	printf("sizeof(int) == %ld, SIZEOF_TYPE(int) = %ld\n",
			sizeof(int), SIZEOF_TYPE(int));
	printf("sizeof(float) == %ld, SIZEOF_TYPE(float) = %ld\n",
			sizeof(float), SIZEOF_TYPE(float));
	printf("sizeof(long) == %ld, SIZEOF_TYPE(long) = %ld\n",
			sizeof(long), SIZEOF_TYPE(long));
	printf("sizeof(element) == %ld, SIZEOF_TYPE(element) = %ld\n",
			sizeof(element_t), SIZEOF_TYPE(element_t));				
}
