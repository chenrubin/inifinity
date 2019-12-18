/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		calc						  *
*		7/11/2019					  *
*									  *
************************************/

#include <stdlib.h> /* strtod */
#include <math.h> /* strtod */
 
#include "calc.h"
#include "../stack/stack.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

#define STACK_SIZE 30
#define ROWS 2
#define COLUMNS 256
#define ERROR_FUNC_10 ErrorFunc,ErrorFunc,ErrorFunc,ErrorFunc,ErrorFunc,	   \
					  ErrorFunc,ErrorFunc,ErrorFunc,ErrorFunc,ErrorFunc
#define ERROR_FUNC_100 ERROR_FUNC_10,ERROR_FUNC_10,ERROR_FUNC_10,ERROR_FUNC_10,\
		ERROR_FUNC_10,ERROR_FUNC_10,ERROR_FUNC_10,ERROR_FUNC_10,ERROR_FUNC_10, \
		ERROR_FUNC_10
#define ERROR_FUNC_256 ERROR_FUNC_100,ERROR_FUNC_100,ERROR_FUNC_10, 		   \
		ERROR_FUNC_10,ERROR_FUNC_10,ERROR_FUNC_10,ERROR_FUNC_10,ErrorFunc,	   \
		ErrorFunc,ErrorFunc,ErrorFunc,ErrorFunc,ErrorFunc
#define NULL_10 NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
#define NULL_46 NULL_10,NULL_10,NULL_10,NULL_10,NULL, NULL, NULL, NULL, NULL,  \
		NULL
#define NULL_STRUCT_10 {0},{0},{0},{0},{0},{0},{0},{0},{0},{0}
#define NULL_STRUCT_44 NULL_STRUCT_10,NULL_STRUCT_10,NULL_STRUCT_10,		   \
		NULL_STRUCT_10,{0},{0},{0},{0}
		
typedef enum state
{
	WAIT_FOR_NUM = 0,
	WAIT_FOR_OP = 1,
	END = 2,
	ERROR = 3
} state_t;

typedef struct precedence_struct
{
	int precedence;
	int associativity;
} precedence_t;

typedef state_t (*func)(const char **exp);
typedef double (*calculate)(double num1, double num2);

static double AddNumbersIMP(double num1, double num2);
static double SubtractNumbersIMP(double num1, double num2);
static double MultiplyNumbersIMP(double num1, double num2);
static double DevideNumbersIMP(double num1, double num2);
static double PowerOfNumbersIMP(double num1, double num2);
static state_t EndCalc(const char **exp);
static int IsPrecedenceLowerIMP(char ch1, char ch2);
static int IsParenthesesValidIMP(const char *exp);
static void PopTwoFromNumStackAndCalcIMP();
static state_t HandleClosingParatheses(const char **exp);
static state_t PushToOpNoPrecedenceIMP(const char **exp);
static state_t PushToOpWithPrecedenceIMP(const char **exp);

/* Sends to Error */
state_t ErrorFunc(const char **exp);

/* push number to num stack */
static state_t PushToNumIMP(const char **exp);

/* push operator to stack while considering precedence */
static state_t PushToOpWithPrecedenceIMP(const char **exp);

func LUT[ROWS][COLUMNS] = {{ERROR_FUNC_256}, {ERROR_FUNC_256}};
calculate calc_lut[53] = {MultiplyNumbersIMP,AddNumbersIMP,NULL,
						  SubtractNumbersIMP, NULL,DevideNumbersIMP,
						  NULL_46, PowerOfNumbersIMP};

precedence_t precedence_LUT[55] = {{1, 0},{0},{3, 0},{2, 0},{0},{2, 0}, {0},
									{3, 0}, NULL_STRUCT_44, {4, 1}};						  
/*
precedence_t precedence_LUT[53] = {{3, 0},{2, 0},{0},{2, 0}, {0},{3, 0},
					  		   	   	NULL_STRUCT_46, {4, 1}};
*/
stack_t *num_stack = NULL;
stack_t *op_stack = NULL;
status current_status = 0;

status Calc(const char *exp, double *res)
{
	state_t st = WAIT_FOR_NUM;
	int i = 0;
	
	current_status = SUCCESS;
	
	if (!IsParenthesesValidIMP(exp))
	{
		return INVALID_EXP;
	}
	
	for (i = 0; i < 10; ++i)
	{
		LUT[WAIT_FOR_NUM][i + '0'] = PushToNumIMP;
	}
	LUT[WAIT_FOR_NUM]['-'] = PushToNumIMP;
	LUT[WAIT_FOR_NUM]['('] = PushToOpNoPrecedenceIMP;
	
	LUT[WAIT_FOR_OP]['+'] = PushToOpWithPrecedenceIMP;
	LUT[WAIT_FOR_OP]['-'] = PushToOpWithPrecedenceIMP;
	LUT[WAIT_FOR_OP]['*'] = PushToOpWithPrecedenceIMP;
	LUT[WAIT_FOR_OP]['/'] = PushToOpWithPrecedenceIMP;
	LUT[WAIT_FOR_OP]['^'] = PushToOpWithPrecedenceIMP;
	LUT[WAIT_FOR_OP][')'] = HandleClosingParatheses;
	LUT[WAIT_FOR_OP]['\0'] = EndCalc;
	
	num_stack = StackCreate(STACK_SIZE, sizeof(double));
	if (NULL == num_stack)
	{
		return ALLOC_FAIL;
	}
	op_stack = StackCreate(STACK_SIZE, sizeof(char));
	if (NULL == num_stack)
	{
		StackDestroy(num_stack);
		return ALLOC_FAIL;
	}
		
	while (st != ERROR && st != END)
	{
		st = LUT[st][(int)*exp](&exp);
	}
	
	*res = *(double *)StackPeek(num_stack);
	
	StackDestroy(num_stack);
	StackDestroy(op_stack);
	
	return current_status;
}

static state_t PushToNumIMP(const char **exp)
{
	double num = strtod(*exp, (char **)exp);
	StackPush(num_stack, &num);
	
	return WAIT_FOR_OP;
}

static state_t HandleClosingParatheses(const char **exp)
{
	(void)exp;
	
	while ('(' != *(char *)StackPeek(op_stack))
	{
		PopTwoFromNumStackAndCalcIMP();
	}
	
	StackPop(op_stack);
	*exp += 1;
	current_status = SUCCESS;
	
	return WAIT_FOR_OP;
}

state_t ErrorFunc(const char **exp)
{
	(void)exp;
	
	current_status = INVALID_EXP;
	
	return ERROR;
}

static state_t EndCalc(const char **exp)
{	
	(void)exp;
	
	while (!StackIsEmpty(op_stack))
	{
		PopTwoFromNumStackAndCalcIMP();
	}

	return END;
}

static state_t PushToOpWithPrecedenceIMP(const char **exp)
{			
	while (StackSize(op_stack) > 0 && 
		   IsPrecedenceLowerIMP(**exp,*(char *)StackPeek(op_stack)))
	{
		PopTwoFromNumStackAndCalcIMP();
	}
	
	StackPush(op_stack, *exp);
	
	*exp += 1;
	
	return WAIT_FOR_NUM;
}

static state_t PushToOpNoPrecedenceIMP(const char **exp)
{			
	StackPush(op_stack, *exp);
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
	if (0 == num2)
	{
		current_status = DIVISION_BY_ZERO;
	}
	
	return num1 / num2;
}

static double PowerOfNumbersIMP(double num1, double num2)
{
	return pow(num1, num2);
}

static int IsPrecedenceLowerIMP(char ch1, char ch2)
{
	int ch1_prec = precedence_LUT[ch1 - '('].precedence;
	int ch2_prec = precedence_LUT[ch2 - '('].precedence;
	int ch1_assoc = precedence_LUT[ch1 - '('].associativity;
	
	return ((ch1_prec < ch2_prec) || 
			((ch1_prec == ch2_prec) &&	(ch1_assoc == 0)));
}	

static int IsParenthesesValidIMP(const char *exp)
{
	stack_t *par_stack = StackCreate(STACK_SIZE, sizeof(char));
	char *runner = (char *)exp;
	
	while ('\0' != *runner)
	{
		if ('(' == *runner)
		{
			StackPush(par_stack, runner);
		}
		else if (')' == *runner)
		{
			if ('(' == *(char *)StackPeek(par_stack))
			{
				StackPop(par_stack);
			}
			else
			{
				return 0;
			}
		}
		
		++runner;
	}
	
	StackDestroy(par_stack);
	
	return 1;
}

static void PopTwoFromNumStackAndCalcIMP()
{
	char op_stack_top = '\0';
	double top = 0;
	double second_top = 0;
	double res = 0;

	op_stack_top = *(char *)StackPeek(op_stack);
	StackPop(op_stack);
	top = *(double *)StackPeek(num_stack);
	StackPop(num_stack);
	second_top = *(double *)StackPeek(num_stack);
	StackPop(num_stack);
	res = calc_lut[op_stack_top - '*'](second_top, top);
	StackPush(num_stack, &res);
}
