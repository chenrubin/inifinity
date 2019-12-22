/************************************
*		Author: ChenR				  *
*		Reviewer: YonatanV			  *
*		calc						  *
*		22/12/2019					  *
*									  *
************************************/

#include <stdlib.h> /* strtod */
#include <math.h> /* strtod */
#include <string.h> /* strlen */
 
#include "calc.h"
#include "../stack/stack.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

#define LUT_PRECEDENCE_POS(x) (x) - ('(')
#define UNUSE(var) (void)(var)
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
#define ASCII_OF_POWER ('^')
#define ASCII_OF_OPEN_PARENTHESES ('(')
#define PRECEDENCE_LUT_SIZE (ASCII_OF_POWER - ASCII_OF_OPEN_PARENTHESES + 1)												
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

typedef union char_or_double
{
	double num;
	char ch;
} char_or_double_t;

typedef struct parser_tagged_union
{
	enum {number = 0, character = 1} type_flag;
	char_or_double_t type;
}tagged_uni_t;

typedef status_t (*func)(tagged_uni_t *parser_res,
						 stack_t *op_stack, 
						 stack_t *num_stack,  
						 status_t *status);
typedef double (*calculate)(double num1, double num2, status_t *status);

typedef struct state_machine_lut
{
	func function;
	state_t next_state;
} sm_lut_t;

/* add two numbers */
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
static status_t EndCalc(tagged_uni_t *parser_res, 
					stack_t *op_stack, 
					stack_t *num_stack, 
					status_t *status);

/* calulate top two numbers in num_stack with the top operator in op_stack */
static void PopTwoFromNumStackAndCalcIMP(stack_t *op_stack, 
										 stack_t *num_stack, 
										 status_t *status);

/* calls PopTwoFromNumStackAndCalcIMP until reaching ')' */
static status_t HandleClosingParatheses(tagged_uni_t *parser_res, 
										stack_t *op_stack, 
										stack_t *num_stack,
										status_t *status);

/* push operator to op_stack wihtout checking precedence */
static status_t PushToOpNoPrecedenceIMP(tagged_uni_t *parser_res, 
										stack_t *op_stack, 
										stack_t *num_stack,
	 									status_t *status);

/* push operator to op_stack after checking precedence */
static status_t PushToOpWithPrecedenceIMP(tagged_uni_t *parser_res, 
										  stack_t *op_stack, 
										  stack_t *num_stack,
										  status_t *status);

/* returns 1 if ch1 precedence is lower the ch2 precedence and 0 if not */
static int IsPrecedenceLowerIMP(char ch1, char ch2);

/* checks if string is valid parantheses wise */
static int IsParenthesesValidIMP(const char *exp);

/* initialize state machine LUT */
static void InitializeLUTIMP();

/* Error function returns ERROR */
static status_t ErrorFuncIMP(tagged_uni_t *parser_res, 
							 stack_t *op_stack, 
							 stack_t *num_stack, 
							 status_t *status);

/* push number to num stack */
static status_t PushToNumIMP(tagged_uni_t *parser_res,
							 stack_t *op_stack, 
							 stack_t *num_stack, 
							 status_t *status);

/* returns either digit or character from string and updates union accordingly*/
static tagged_uni_t ParserIMP(char **exp, state_t state);

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

status_t Calc(const char *exp, double *res)
{
	state_t state = WAIT_FOR_NUM;
	size_t len = strlen(exp) + 1;
	size_t steps = 0;
	status_t current_status = SUCCESS;
	tagged_uni_t parser_res = {0};
	stack_t *num_stack = NULL;
	stack_t *op_stack = NULL;

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
		
		return ALLOC_FAIL;
	}
		
	while ((steps < len) && (SUCCESS == current_status))
	{
		char current_char = *exp;
		char *location = (char *)exp;
		
		parser_res = ParserIMP((char **)&exp, state);
		steps += (size_t)(exp - location);
		current_status = LUT[state][(int)current_char].function(&parser_res, 
															 op_stack, 
															 num_stack,
															 &current_status);
		state = LUT[state][(int)current_char].next_state;

		if (parser_res.type_flag == character)
		{
			++exp;
			++steps;
		}
	}

	if (SUCCESS == current_status)
	{
		*res = *(double *)StackPeek(num_stack);
	}
	
	StackDestroy(num_stack);
	StackDestroy(op_stack);
	
	return current_status;
}

static status_t PushToNumIMP(tagged_uni_t *parser_res, 
							 stack_t *op_stack, 
							 stack_t *num_stack, 
							 status_t *status)
{
	UNUSE(op_stack);
	
	if ((parser_res -> type_flag) == number)
	{
		StackPush(num_stack, &parser_res->type.num);
		*status |= SUCCESS;
	}
	else
	{
		*status = INVALID_EXP;
	}
	
	return *status;
}

static status_t HandleClosingParatheses(tagged_uni_t *parser_res, 
										stack_t *op_stack, 
										stack_t *num_stack, 
										status_t *status)
{
	UNUSE(parser_res);
	
	while ('(' != *(char *)StackPeek(op_stack))
	{
		PopTwoFromNumStackAndCalcIMP(op_stack, num_stack, status);
	}
	
	StackPop(op_stack);
	
	return SUCCESS;
}

static status_t ErrorFuncIMP(tagged_uni_t *parser_res, 
							 stack_t *op_stack, 
							 stack_t *num_stack, 
							 status_t *status)
{
	UNUSE(parser_res);
	UNUSE(status);
	UNUSE(op_stack);
	UNUSE(num_stack);
	
	return INVALID_EXP;
}

static status_t EndCalc(tagged_uni_t *parser_res, 
					stack_t *op_stack, 
					stack_t *num_stack, 
					status_t *status)
{	
	UNUSE(parser_res);
	
	while (!StackIsEmpty(op_stack))
	{
		PopTwoFromNumStackAndCalcIMP(op_stack, num_stack, status);
	}
	
	return *status;
}

static status_t PushToOpWithPrecedenceIMP(tagged_uni_t *parser_res,
										  stack_t *op_stack, 
										  stack_t *num_stack, 
										  status_t *status)
{			
	UNUSE(num_stack);
	
	if ((parser_res -> type_flag) == character)
	{
		while ((StackSize(op_stack) > 0) && 
		   	   (IsPrecedenceLowerIMP(parser_res->type.ch,
		   	   					  *(char *)StackPeek(op_stack))))
		{
			PopTwoFromNumStackAndCalcIMP(op_stack, num_stack, status);
		}
		StackPush(op_stack, &parser_res->type.ch);
		*status |= SUCCESS;
	}
	else
	{
		*status = INVALID_EXP;
	}
	
	return *status;
}

static status_t PushToOpNoPrecedenceIMP(tagged_uni_t *parser_res, 
										stack_t *op_stack, 
										stack_t *num_stack, 
										status_t *status)
{			
	UNUSE(num_stack);
	
	{
		StackPush(op_stack, &parser_res->type.ch);
		*status |= SUCCESS;
	}
	
	return *status;
}

static double AddNumbersIMP(double num1, double num2, status_t *status)
{
	*status |= SUCCESS;
	
	return (num1 + num2);
}

static double SubtractNumbersIMP(double num1, double num2, status_t *status)
{
	*status |= SUCCESS;
	
	return (num1 - num2);
}

static double MultiplyNumbersIMP(double num1, double num2, status_t *status)
{
	*status |= SUCCESS;
	
	return (num1 * num2);
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
		*status |= SUCCESS;
		
		return (num1 / num2);
	}
}

static double PowerOfNumbersIMP(double num1, double num2, status_t *status)
{
	*status |= SUCCESS;
	
	if ((0 == num1) && (0 == num2))
	{
		*status = INVALID_EXP;
	}

	return pow(num1, num2);
}

static int IsPrecedenceLowerIMP(char ch1, char ch2)
{
	int ch1_prec = precedence_LUT[LUT_PRECEDENCE_POS(ch1)].precedence;
	int ch2_prec = precedence_LUT[LUT_PRECEDENCE_POS(ch2)].precedence;
	int ch1_assoc = precedence_LUT[LUT_PRECEDENCE_POS(ch1)].assoc;
	
	return ((ch1_prec < ch2_prec) || 
			((ch1_prec == ch2_prec) &&	(ch1_assoc == LEFT)));
}	

static int IsParenthesesValidIMP(const char *exp)
{

	size_t open_counter = 0;
	size_t close_counter = 0;
	char *str_runner = (char *)exp;

	while ('\0' != *str_runner)
	{
	    if ('(' == *str_runner)
	    {
	        ++open_counter;
	    }
	    else if (')' == *str_runner)
	    {
	        ++close_counter;
	    }
	    ++str_runner;
	}

	if (open_counter != close_counter)
	{
		return 0;
	}

	return 1;
}

static void PopTwoFromNumStackAndCalcIMP(stack_t *op_stack, 
										 stack_t *num_stack, 
										 status_t *status)
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
	sm_lut_t final_calculation = {EndCalc, WAIT_FOR_OP};

	for (i = 0; i < 10; ++i)
	{
		LUT[WAIT_FOR_NUM][i + '0'] = push_wfo;
	}
	LUT[WAIT_FOR_NUM]['-'] = push_wfo;
	LUT[WAIT_FOR_NUM]['+'] = push_wfo;
	LUT[WAIT_FOR_NUM]['.'] = push_wfo;
	LUT[WAIT_FOR_NUM]['('] = push_no_prec_to_wfn;
	LUT[WAIT_FOR_OP]['+'] = push_prec_wfn;
	LUT[WAIT_FOR_OP]['-'] = push_prec_wfn;
	LUT[WAIT_FOR_OP]['*'] = push_prec_wfn;
	LUT[WAIT_FOR_OP]['/'] = push_prec_wfn;
	LUT[WAIT_FOR_OP]['^'] = push_prec_wfn;
	LUT[WAIT_FOR_OP][')'] = handle_close_par_to_wfo;
	LUT[WAIT_FOR_OP]['\0'] = final_calculation;
}	

static tagged_uni_t ParserIMP(char **exp, state_t state)
{
	tagged_uni_t parser_union = {0};
	double num = 0;
	
	if ((WAIT_FOR_NUM == state) && ('(' != **exp))
	{
		num = strtod(*exp, exp);
		parser_union.type_flag = number;
		parser_union.type.num = num;
	}
	else
	{
		parser_union.type_flag = character;
		parser_union.type.ch = **exp;
	}
		
	return parser_union;
}
