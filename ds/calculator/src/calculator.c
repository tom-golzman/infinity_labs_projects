/**
	Written By: Tom Golzman
	Date: 21/05/2025
	Reviewed By: Amir Granot
**/

/************************************includes************************************/
#include <string.h>	/* strlen() */
#include <assert.h> /* assert */
#include <float.h>	/* DBL_MAX */
#include <stdlib.h>	/* strtod() */
#include <errno.h>	/* errno, ERANGE */
#include <ctype.h>	/* isdigit() */

#include "stack.h"	/* stack_t */
#include "calculator.h"

/************************************define************************************/
#define TRUE	(1)
#define FALSE	(0)
#define SUCCESS	(0)
#define FAIL	(1)

#define ASCII_SIZE (128)
/************************************typedef************************************/
typedef enum {
	FIRST_CHAR = 0,
	HANDLE_OPERAND,
	HANDLE_OPERATOR,
	HANDLE_CLOSE_PARENTHESIS,
	ERROR
} calc_state_t;

typedef enum {
	PLUS =	4,
	MINUS = 4,
	MUL = 	3,
	DIV = 	3
} calc_operator_t;

typedef struct {
	stack_t* operands;
	stack_t* operators;
	calc_status_t status;
} calc_managment_t;

typedef calc_state_t (*calc_state_handler_t)(const char** input, calc_managment_t* manager);
typedef calc_status_t (*calc_op_handler_t)(double left_operand, double right_operand, double* result);

/************************************Private Functions Forward Decleration************************************/
static calc_state_t state_transition_table[ASCII_SIZE] = {0};
static void InitStateTransitionTable();
static int is_state_table_initialized = FALSE;

static calc_state_t FirstCharHandler(const char** input, calc_managment_t* manager);
static calc_state_t OperatorHandler(const char** input, calc_managment_t* manager);
static calc_state_t OperandHandler(const char** input, calc_managment_t* manager);
static calc_state_t CloseParenthesisHandler(const char** input, calc_managment_t* manager);

static calc_status_t InitManager(calc_managment_t** manager, size_t expression_size);
static void DestroyManager(calc_managment_t* manager);

static calc_status_t OpPlus(double left_operand, double right_operand, double* result);
static calc_status_t OpMinus(double left_operand, double right_operand, double* result);
static calc_status_t OpMul(double left_operand, double right_operand, double* result);
static calc_status_t OpDiv(double left_operand, double right_operand, double* result);
static void Evaluate(calc_managment_t* manager);

static int GetPrecedence(char op);
static int IsHigherPrecedence(const char current_op, const char last_op);

/************************************Functions************************************/
calc_status_t Calculate(const char* expression, double* result)
{
	static const calc_state_handler_t handlers[] = { FirstCharHandler, OperandHandler, OperatorHandler, CloseParenthesisHandler };
	
	calc_managment_t* manager = NULL;
	calc_state_t current_state = FIRST_CHAR;
	size_t expression_size = 0;
	calc_status_t status = SUCCESS;
	calc_status_t init_status = SUCCESS;
	
	assert(NULL != expression);
	assert(NULL != result);

	expression_size = strlen(expression);
	
	init_status = InitManager(&manager, expression_size);
	if (SUCCESS != init_status)
	{
		return (init_status);
	}
	
	if (FALSE == is_state_table_initialized)
	{
		InitStateTransitionTable();
		is_state_table_initialized = TRUE;
	}

	while (*(char*)expression != '\0' && SYNTAX_ERROR != manager->status)
	{
		current_state = handlers[current_state](&expression, manager);
	}

	if (ERROR == current_state)
	{
		status = manager->status;
		DestroyManager(manager);
		
		return (status);
	}
	if (HANDLE_OPERATOR == current_state)
	{
		DestroyManager(manager);
	
		return (SYNTAX_ERROR);
	}
	
	while (!IsEmpty(manager->operators) && SUCCESS == status && Count(manager->operands) > 1)
	{
		Evaluate(manager);
		status = manager->status;
	}
	if (SUCCESS == status)
	{
		*result = *(double*)Pop(manager->operands);
		
		if (!IsEmpty(manager->operands) || !IsEmpty(manager->operators))
		{
			status = SYNTAX_ERROR;
		}
	}
	
	DestroyManager(manager);

	return (status);
}

static calc_status_t InitManager(calc_managment_t** manager, size_t expression_size)
{
	stack_t* operands = NULL;
	stack_t* operators = NULL;
	calc_managment_t* local_manager = NULL;
	
	assert(NULL != manager);	
	
	operands = Create(expression_size, sizeof(double));
	if (NULL == operands)
	{
		return (INTERNAL_ERROR);
	}

	operators = Create(expression_size, sizeof(char));
	if (NULL == operators)
	{
		Destroy(operands);
		operands = NULL;

		return (INTERNAL_ERROR);
	}

	local_manager = (calc_managment_t*)malloc(sizeof(*local_manager));
	if (NULL == manager)
	{
		Destroy(operands);
		operands = NULL;
		Destroy(operators);
		operators = NULL;
		
		return (INTERNAL_ERROR);
	}
	
	local_manager->operands = operands;
	local_manager->operators = operators;
	local_manager->status = SUCCESS;
	
	*manager = local_manager;
	
	return (SUCCESS);
}

static void DestroyManager(calc_managment_t* manager)
{
	assert(NULL != manager);
	
	Destroy(manager->operands);
	Destroy(manager->operators);
	free(manager);
	manager = NULL;
}

static void InitStateTransitionTable()
{
	int i = 0;
	
	for (i = 0; i < 128; i++)
	{
		state_transition_table[i] = ERROR;
	}
	
	state_transition_table['('] = HANDLE_OPERATOR;
	state_transition_table['+'] = HANDLE_OPERAND;
	state_transition_table['-'] = HANDLE_OPERAND;

	state_transition_table['*'] = HANDLE_OPERATOR;
	state_transition_table['/'] = HANDLE_OPERATOR;
	state_transition_table['+'] = HANDLE_OPERATOR;
	state_transition_table['-'] = HANDLE_OPERATOR;
	state_transition_table[')'] = HANDLE_CLOSE_PARENTHESIS;
	state_transition_table['\0'] = HANDLE_OPERAND;
	
	state_transition_table['('] = HANDLE_OPERATOR;
}

static calc_state_t FirstCharHandler(const char** input, calc_managment_t* manager)
{
	const char* current_char = NULL;
	calc_state_t state = ERROR;
	
	assert(NULL != input);
	assert(NULL != *input);
	assert(NULL != manager);
	
	current_char = *input;
	
	if (isdigit(*current_char))
	{
		return (HANDLE_OPERAND);
	}

	state = state_transition_table[(int)*current_char];
	if (ERROR == state)
	{
		manager->status = SYNTAX_ERROR;
	}
	
	return (state);
}

static calc_state_t OperandHandler(const char** input, calc_managment_t* manager)
{
	double current_operand = 0;
	calc_state_t state = ERROR;
		
	assert(NULL != input);
	assert(NULL != *input);
	assert(NULL != manager);

	errno = 0;
	current_operand = strtod(*input, (char**)input);
	
	if (ERANGE == errno)
	{
		manager->status = FLOW_ERROR;
		
		return (ERROR);
	}
	
	Push(manager->operands, &current_operand);
	
	state = state_transition_table[(int)**input];
	if (ERROR == state)
	{
		manager->status = SYNTAX_ERROR;
	}
	
	return (state);
}

static calc_state_t OperatorHandler(const char** input, calc_managment_t* manager)
{
	const char* current_operator = NULL;	
	calc_state_t state = ERROR;	
	
	assert(NULL != input);
	assert(NULL != *input);
	assert(NULL != manager);
	
	current_operator = *input;

	while (!IsEmpty(manager->operators) && !IsHigherPrecedence(*current_operator, *(const char*)Peek(manager->operators)))
	{
		Evaluate(manager);
		if (SUCCESS != manager->status)
		{
			return (ERROR);
		}
	}

	Push(manager->operators, (char*)current_operator);

	(*input)++;

	if (isdigit(**input))
	{
		return (HANDLE_OPERAND);
	}
	
	state = state_transition_table[(int)**input];
	if (ERROR == state)
	{
		manager->status = SYNTAX_ERROR;
	}
	
	return (state);
}

static calc_state_t CloseParenthesisHandler(const char** input, calc_managment_t* manager)
{
	char* current_operator = NULL;
	int open_parenthesis_was_found = FALSE;
	calc_state_t state = ERROR;	
	
	assert(NULL != input);
	assert(NULL != *input);
	assert(NULL != manager);

	while (!IsEmpty(manager->operators) && SUCCESS == manager->status)
	{
		current_operator = Peek(manager->operators);

		if (*current_operator == '(')
		{
			Pop(manager->operators);
			open_parenthesis_was_found = TRUE;
			break;
		}

		Evaluate(manager);
		if (SUCCESS != manager->status)
		{
			return (ERROR);
		}
	}
	 
	if (FALSE == open_parenthesis_was_found)
	{
		manager->status = SYNTAX_ERROR;
		
		return (ERROR);	
	}
	
	(*input)++;
	
	state = state_transition_table[(int)**input];
	if (ERROR == state)
	{
		manager->status = SYNTAX_ERROR;
	}
	
	return (state);
}

static void Evaluate(calc_managment_t* manager)
{
	static calc_op_handler_t operators[ASCII_SIZE];
	static int initialized = FALSE;
	double left_operand = 0;
	double right_operand = 0;
	char operator = '\0';
	double temp_result = 0;
	
	assert(NULL != manager);
	
	if (FALSE == initialized)
	{
		operators['+'] = OpPlus;
		operators['-'] = OpMinus;
		operators['*'] = OpMul;
		operators['/'] = OpDiv;
		initialized = TRUE;
	}
	
	assert(!IsEmpty(manager->operators));
	assert(!IsEmpty(manager->operands));
	
	operator = *(char*)Pop(manager->operators);
	right_operand = *(double*)Pop(manager->operands);
	left_operand = *(double*)Pop(manager->operands);
	
	manager->status = operators[(int)operator](left_operand, right_operand, &temp_result);
	Push(manager->operands, &temp_result);
}

static calc_status_t OpPlus(double left_operand, double right_operand, double* result)
{
	assert(NULL != result);

	*result = left_operand + right_operand;
	
	return (SUCCESS);
}

static calc_status_t OpMinus(double left_operand, double right_operand, double* result)
{
	assert(NULL != result);
	
	*result = left_operand - right_operand;
	
	return (SUCCESS);
}

static calc_status_t OpMul(double left_operand, double right_operand, double* result)
{
	assert(NULL != result);
	
	*result = left_operand * right_operand;
	
	return (SUCCESS);
}

static calc_status_t OpDiv(double left_operand, double right_operand, double* result)
{
	assert(NULL != result);

	if (0 == right_operand)
	{
		return (ARITHMETIC_ERROR);
	}
	
	*result = (left_operand / right_operand);
	
	return (SUCCESS);
}

static int GetPrecedence(char op)
{
	switch (op)
	{
		case '+':
			return (PLUS);
		case '-':
			return (MINUS);
		case '*':
			return (MUL);
		case '/':
			return (DIV);
		default:
			return (FAIL);
	}
}

static int IsHigherPrecedence(const char current_op, const char last_op)
{
	if (current_op == '(' || last_op == '(')
	{
		return (TRUE);
	}
	
	return (GetPrecedence(current_op) <= GetPrecedence(last_op));
}
