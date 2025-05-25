#ifndef __ILRD_CALCULATOR_H__
#define __ILRD_CALCULATOR_H__

/************************************includes************************************/

/************************************typedef************************************/
typedef enum {
	SUCCESS = 0,		/* Succeded at Calculating the expression */
	SYNTAX_ERROR,		/* Illegal expression based on syntax. stuff like 2 non-unary operators one after another */
	ARITHMETIC_ERROR,	/* Illegal expressions mathematically, stuff like x/0 or (-|x|) ^ (1/(2*y)) */
	INTERNAL_ERROR,		/* Technical problems within the function like malloc fail */
	FLOW_ERROR
} calc_status_t;

/************************************define************************************/

/************************************Functions************************************/
/**
	@DESCRIPTION: Calculates an expression
	@PARAMETERS: 
		- char* expression - the expression we want to calculate
		- double* result - where we save the result of the expression
	@RETURNS: calc_status_t - status representing if the calculation succeeded or failed
	@COMPLEXITY:
		- Time: O(n)
		- Space: O(n)
	@LIMITATIONS: Undefined behavior if expression is NULL or if result is NULL. 
	@NOTES: Supported operators are: +, -, *, /
**/
calc_status_t Calculate(const char* expression, double* result);

#endif /*__ILRD_CALCULATOR_H__*/
