/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		calc						  *
*		7/11/2019					  *
*									  *
************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* strtod */

#include "calc.h"
#include "../stack/stack.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

#define STACK_SIZE 30
#define ROWS 2
#define COLUMNS 256
#define ERROR_FUNC_10 ErrorFunc,ErrorFunc,ErrorFunc,ErrorFunc,ErrorFunc,\
					  ErrorFunc,ErrorFunc,ErrorFunc,ErrorFunc,ErrorFunc
#define ERROR_FUNC_100 ERROR_FUNC_10,ERROR_FUNC_10,ERROR_FUNC_10,ERROR_FUNC_10,\
		ERROR_FUNC_10,ERROR_FUNC_10,ERROR_FUNC_10,ERROR_FUNC_10,ERROR_FUNC_10, \
		ERROR_FUNC_10
#define ERROR_FUNC_256 ERROR_FUNC_100,ERROR_FUNC_100,ERROR_FUNC_10, 		\
		ERROR_FUNC_10,ERROR_FUNC_10,ERROR_FUNC_10,ERROR_FUNC_10,ErrorFunc,	\
		ErrorFunc,ErrorFunc,ErrorFunc,ErrorFunc,ErrorFunc
#define NULL_10 NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
#define NULL_46 NULL_10,NULL_10,NULL_10,NULL_10,NULL, NULL, NULL, NULL, NULL, \
		NULL		
		

typedef enum state
{
	WAIT_FOR_NUM = 0,
	WAIT_FOR_OP = 1,
	END = 2,
	ERROR = 3
} sm_state;

typedef sm_state (*func)(const char **exp);
typedef double (*calculate)(double num1, double num2);

static double AddNumbersIMP(double num1, double num2);
static double SubtractNumbersIMP(double num1, double num2);
static double MultiplyNumbersIMP(double num1, double num2);
static double DevideNumbersIMP(double num1, double num2);
static double PowerOfNumbersIMP(double num1, double num2);
static void MoveAllOpsToNumStackIMP();
static sm_state EndCalc(const char **exp);

/* Sends to Error */
sm_state ErrorFunc(const char **exp);

/* push number to num stack */
sm_state PushToNum(const char **exp);

/* push operator to stack while considering precedence */
sm_state PushToOpWithPrecedence(const char **exp);

func LUT[ROWS][COLUMNS] = {{ERROR_FUNC_256}, {ERROR_FUNC_256}};
calculate calc_lut[53] = {MultiplyNumbersIMP,AddNumbersIMP,NULL,SubtractNumbersIMP, NULL,DevideNumbersIMP,
						  NULL_46, PowerOfNumbersIMP};

stack_t *num_stack = NULL;
stack_t *op_stack = NULL;

status Calc(const char *exp, double *res)
{
	sm_state st = WAIT_FOR_NUM;
	int i = 0;
	
	for (i = 0; i < 10; ++i)
	{
		LUT[WAIT_FOR_NUM][i + '0'] = PushToNum;
	}
	LUT[WAIT_FOR_OP]['+'] = PushToOpWithPrecedence;
	LUT[WAIT_FOR_OP]['-'] = PushToOpWithPrecedence;
	LUT[WAIT_FOR_OP]['\0'] = EndCalc;
	
	num_stack = StackCreate(STACK_SIZE, sizeof(double));
	op_stack = StackCreate(STACK_SIZE, sizeof(char));
		
	while (st != ERROR && st != END)
	{
		st = LUT[st][(int)*exp](&exp);
	}
	
	*res = *(double *)StackPeek(num_stack);
	
	free(num_stack);
	free(op_stack);
	
	return SUCCESS;
}

sm_state PushToNum(const char **exp)
{
	double num = strtod(*exp, (char **)exp);
	StackPush(num_stack, &num);
	
	return WAIT_FOR_OP;
}

sm_state ErrorFunc(const char **exp)
{
	(void)exp;
	
	printf("Stupid moron\n");
	
	return ERROR;
}

static sm_state EndCalc(const char **exp)
{
	char operation = '\0';
	double top = 0;
	double bottom = 0;
	double answer = 0;
	
	(void)exp;	
	MoveAllOpsToNumStackIMP();
	operation = *(char *)StackPeek(num_stack);
	StackPop(num_stack);
	top = *(double *)StackPeek(num_stack);
	StackPop(num_stack);
	bottom = *(double *)StackPeek(num_stack);
	StackPop(num_stack);
	
	answer = calc_lut[operation - 42](bottom, top);
	
	StackPush(num_stack, &answer);

	return END;
}

/* need to add accounting of precedence table */
sm_state PushToOpWithPrecedence(const char **exp)
{
	/*char ch = *(char *)StackPeek(op_stack);*/
	char ch = **exp;
	StackPush(op_stack, &ch);
	
	*exp += 1;
	
	return WAIT_FOR_NUM;
}

static double AddNumbersIMP(double num1, double num2)
{
	return num1 + num2;
}

static double SubtractNumbersIMP(double num1, double num2)
{
	return num1 - num2;
}

static double MultiplyNumbersIMP(double num1, double num2)
{
	return num1 * num2;
}

static double DevideNumbersIMP(double num1, double num2)
{
	return num1 / num2;
}

static double PowerOfNumbersIMP(double num1, double num2)
{
	return 0;
}

static void MoveAllOpsToNumStackIMP()
{
	while (!StackIsEmpty(op_stack))
	{
		StackPush(num_stack, StackPeek(op_stack));
		StackPop(op_stack);
	}
}
