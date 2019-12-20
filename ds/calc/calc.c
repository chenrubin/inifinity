/************************************
*		Author: ChenR				  *
*		Reviewer: EyalR				  *
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
#define SM_LUT_10 {ErrorFuncIMP, WAIT_FOR_NUM},{ErrorFuncIMP, WAIT_FOR_NUM},   \
				  {ErrorFuncIMP, WAIT_FOR_NUM},{ErrorFuncIMP, WAIT_FOR_NUM},   \
				  {ErrorFuncIMP, WAIT_FOR_NUM},{ErrorFuncIMP, WAIT_FOR_NUM},   \
				  {ErrorFuncIMP, WAIT_FOR_NUM},{ErrorFuncIMP, WAIT_FOR_NUM},   \
				  {ErrorFuncIMP, WAIT_FOR_NUM},{ErrorFuncIMP, WAIT_FOR_NUM}
#define SM_LUT_100 SM_LUT_10,SM_LUT_10,SM_LUT_10,SM_LUT_10,SM_LUT_10,		   \
		SM_LUT_10,SM_LUT_10,SM_LUT_10,SM_LUT_10,SM_LUT_10
#define SM_LUT_256 SM_LUT_100,SM_LUT_100,SM_LUT_10,SM_LUT_10,SM_LUT_10, 	   \
		SM_LUT_10,SM_LUT_10,{ErrorFuncIMP, WAIT_FOR_NUM},					   \
		{ErrorFuncIMP, WAIT_FOR_NUM},{ErrorFuncIMP, WAIT_FOR_NUM},			   \
		{ErrorFuncIMP, WAIT_FOR_NUM},{ErrorFuncIMP, WAIT_FOR_NUM},	   		   \
		{ErrorFuncIMP, WAIT_FOR_NUM}
#define CALCULATE_NULL_10 NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 	   \
						  NULL,NULL
#define CALCULATE_NULL_46 CALCULATE_NULL_10,CALCULATE_NULL_10,				   \
						  CALCULATE_NULL_10,CALCULATE_NULL_10,				   \
						  NULL, NULL, NULL, NULL, NULL,NULL
#define PRECEDENCE_NULL_10 {0},{0},{0},{0},{0},{0},{0},{0},{0},{0}
#define PRECEDENCE_NULL_46 PRECEDENCE_NULL_10,PRECEDENCE_NULL_10,			   \
						   PRECEDENCE_NULL_10,PRECEDENCE_NULL_10,			   \
						   {0},{0},{0},{0},{0},{0}
#define PRECEDENCE_LUT_SIZE 55												
#define CALCULATE_LUT_SIZE 53

typedef enum state
{
	WAIT_FOR_NUM = 0,
	WAIT_FOR_OP = 1,
	END = 2,
	ERROR = 3
} state_t;

typedef enum associativity
{
	LEFT = 0,
	RIGHT = 1
} assoc_t;

typedef struct precedence_struct
{
	int precedence;
	assoc_t assoc;
} precedence_t;

typedef status_t (*func)(void *exp, status_t *status);
typedef double (*calculate)(double num1, double num2, status_t *status);

typedef struct state_machine_lut
{
	func function;
	state_t state;
} sm_lut_t;

/* add two numbeers */
static double AddNumbersIMP(double num1, double num2, status_t *status);

/* subtract num2 from num1 */
static double SubtractNumbersIMP(double num1, double num2, status_t *status);

/* multiply two numbers */
static double MultiplyNumbersIMP(double num1, double num2, status_t *status);

/* divide num1 by num2 */
static double DivideNumbersIMP(double num1, double num2, status_t *status);

/* base = num1, power = num2 */
static double PowerOfNumbersIMP(double num1, double num2, status_t *status);

/* handles the end calcultion in which you calulate what is left until
   final result is located in the num)stack */
static void EndCalc(void *exp, status_t *status);

/* calulate top two numbers in num_stack with the top operator in op_stack */
static void PopTwoFromNumStackAndCalcIMP(status_t *status);

/* calls PopTwoFromNumStackAndCalcIMP until reaching ')' */
static status_t HandleClosingParatheses(void *exp, status_t *status);

/* push operator to op_stack wihtout checking precedence */
static status_t PushToOpNoPrecedenceIMP(void *exp, status_t *status);

/* push operator to op_stack after checking precedence */
static status_t PushToOpWithPrecedenceIMP(void *exp, status_t *status);

/* returns 1 if ch1 precedence is lower the ch2 precedence and 0 if not */
static int IsPrecedenceLowerIMP(char ch1, char ch2);

/* checks if string is valid parantheses wise */
static int IsParenthesesValidIMP(const char *exp);

/* initialize state machine LUT */
static void InitializeLUTIMP();

/* Error function returns ERROR */
static status_t ErrorFuncIMP(void *exp, status_t *status);

/* push number to num stack */
static status_t PushToNumIMP(void *exp, status_t *status);	

sm_lut_t LUT[ROWS][COLUMNS] = {{SM_LUT_256}, {SM_LUT_256}};
calculate calc_lut[CALCULATE_LUT_SIZE] = {MultiplyNumbersIMP,AddNumbersIMP,
										  NULL,SubtractNumbersIMP, NULL,
										  DivideNumbersIMP,CALCULATE_NULL_46,
										  PowerOfNumbersIMP};
precedence_t precedence_LUT[PRECEDENCE_LUT_SIZE] = {{1, LEFT},{0},{3, LEFT},
													{2, LEFT},{0},{2, LEFT},
													{0},{3, LEFT},
													PRECEDENCE_NULL_46, 
													{4, RIGHT}};						  

stack_t *num_stack = NULL;
stack_t *op_stack = NULL;

status_t Calc(const char *exp, double *res)
{
	state_t st = WAIT_FOR_NUM;
	status_t current_status = SUCCESS;

	if (!IsParenthesesValidIMP(exp))
	{
		current_status = INVALID_EXP;
	}
	
	InitializeLUTIMP();
	
	num_stack = StackCreate(STACK_SIZE, sizeof(double));
	if (NULL == num_stack)
	{
		return ALLOC_FAIL;
	}
	op_stack = StackCreate(STACK_SIZE, sizeof(char));
	if (NULL == num_stack)
	{
		StackDestroy(num_stack);
		
		current_status = ALLOC_FAIL;
	}
		
	while ((0 != *exp) && SUCCESS == current_status)
	{
		char current_char = *exp;
		double num = 0;
		
		if (WAIT_FOR_NUM == st)
		{
			if ('(' != current_char)
			{
				num = strtod(exp, (char **)&exp);
				current_status = LUT[st][(int)current_char].function(&num, &current_status);
				st = LUT[st][(int)current_char].state;
			}
			else
			{
				current_status = LUT[st][(int)*exp].function((char **)exp, &current_status);
				st = LUT[st][(int)*exp].state;
				++exp;
			}
		}
		else
		{
			current_status = LUT[st][(int)*exp].function((char **)exp, &current_status);
			st = LUT[st][(int)*exp].state;
			++exp;
		}	
	}
	
	if (SUCCESS == current_status /*&& !StackIsEmpty(op_stack)*/)
	{
		EndCalc(&exp, &current_status);
	}
	if (SUCCESS == current_status)
	{
		*res = *(double *)StackPeek(num_stack);
	}
	
	StackDestroy(num_stack);
	StackDestroy(op_stack);
	
	return current_status;
}

static status_t PushToNumIMP(void *exp, status_t *status)
{
	(void)status;
	
	StackPush(num_stack, (double *)exp);
	
	return SUCCESS;
}

static status_t HandleClosingParatheses(void *exp, status_t *status)
{
	(void)exp;
	
	while ('(' != *(char *)StackPeek(op_stack))
	{
		PopTwoFromNumStackAndCalcIMP(status);
	}
	
	StackPop(op_stack);
	
	return SUCCESS;
}

static status_t ErrorFuncIMP(void *exp, status_t *status)
{
	(void)exp;
	(void)status;
	
	return INVALID_EXP;
}

static void EndCalc(void *exp, status_t *status)
{	
	(void)exp;
	
	while (!StackIsEmpty(op_stack))
	{
		PopTwoFromNumStackAndCalcIMP(status);
	}
}

static status_t PushToOpWithPrecedenceIMP(void *exp, status_t *status)
{			
	while (StackSize(op_stack) > 0 && 
		   IsPrecedenceLowerIMP(*(char *)exp,*(char *)StackPeek(op_stack)))
	{
		PopTwoFromNumStackAndCalcIMP(status);
	}
	
	StackPush(op_stack, (char *)exp);
	
	return SUCCESS;
}

static status_t PushToOpNoPrecedenceIMP(void *exp, status_t *status)
{			
	StackPush(op_stack, (char *)exp);
	
	*status = SUCCESS;
	
	return *status;
}

static double AddNumbersIMP(double num1, double num2, status_t *status)
{
	*status = SUCCESS;
	
	return num1 + num2;
}

static double SubtractNumbersIMP(double num1, double num2, status_t *status)
{
	*status = SUCCESS;
	
	return num1 - num2;
}

static double MultiplyNumbersIMP(double num1, double num2, status_t *status)
{
	*status = SUCCESS;
	
	return num1 * num2;
}

static double DivideNumbersIMP(double num1, double num2, status_t *status)
{
	if (0 == num2)
	{
		*status = DIVISION_BY_ZERO;
		
		return 0;
	}
	else
	{
		*status = SUCCESS;
		
		return num1 / num2;
	}
}

static double PowerOfNumbersIMP(double num1, double num2, status_t *status)
{
	*status = SUCCESS;
	
	return pow(num1, num2);
}

static int IsPrecedenceLowerIMP(char ch1, char ch2)
{
	int ch1_prec = precedence_LUT[ch1 - '('].precedence;
	int ch2_prec = precedence_LUT[ch2 - '('].precedence;
	int ch1_assoc = precedence_LUT[ch1 - '('].assoc;
	
	return ((ch1_prec < ch2_prec) || 
			((ch1_prec == ch2_prec) &&	(ch1_assoc == LEFT)));
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
			if (!StackIsEmpty(par_stack) && 
				('(' == *(char *)StackPeek(par_stack)))

			{
				StackPop(par_stack);
			}
			else
			{
				StackDestroy(par_stack);
				
				return 0;
			}
		}
		
		++runner;
	}
	
	if (0 == StackSize(par_stack))
	{
		StackDestroy(par_stack);
		
		return 1;
	}
	else
	{
		StackDestroy(par_stack);
		
		return 0;
	}
}

static void PopTwoFromNumStackAndCalcIMP(status_t *status)
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
	res = calc_lut[op_stack_top - '*'](second_top, top, status);
	StackPush(num_stack, &res);
	
	*status |= SUCCESS;
}

static void InitializeLUTIMP()
{
	int i = 0;
	
	sm_lut_t push_wfo = {PushToNumIMP, WAIT_FOR_OP};
	sm_lut_t push_no_prec_to_wfn = {PushToOpNoPrecedenceIMP, WAIT_FOR_NUM};
	sm_lut_t push_prec_wfn = {PushToOpWithPrecedenceIMP, WAIT_FOR_NUM};
	sm_lut_t handle_close_par_to_wfo = {HandleClosingParatheses, WAIT_FOR_OP};

	for (i = 0; i < 10; ++i)
	{
		LUT[WAIT_FOR_NUM][i + '0'] = push_wfo;
	}
	LUT[WAIT_FOR_NUM]['-'] = push_wfo;
	LUT[WAIT_FOR_NUM]['+'] = push_wfo;
	LUT[WAIT_FOR_NUM]['('] = push_no_prec_to_wfn;
	LUT[WAIT_FOR_OP]['+'] = push_prec_wfn;
	LUT[WAIT_FOR_OP]['-'] = push_prec_wfn;
	LUT[WAIT_FOR_OP]['*'] = push_prec_wfn;
	LUT[WAIT_FOR_OP]['/'] = push_prec_wfn;
	LUT[WAIT_FOR_OP]['^'] = push_prec_wfn;
	LUT[WAIT_FOR_OP][')'] = handle_close_par_to_wfo;
}
/*
static void *ParseStringIMP(state_t current_state, 
						 char **exp, 
						 double *num, 
						 char *ch)
{	
	if (current_state == WAIT_FOR_NUM)
	{
		*num = strtod(*exp, exp);
	}
	else
	{
		*ch = *exp;
	}
}*/
