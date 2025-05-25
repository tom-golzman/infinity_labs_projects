/************************************includes************************************/
#include <stdio.h> /* printf */

#include "calculator.h"

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define BOLD_TITLE "\033[1;4m"
#define TITLE "\033[4m"

#define TRUE (1)
#define FALSE (0)
#define SUCCESS (0)
#define FAIL (1)

/************************************Functions Forward Decleration************************************/
void TestCalculate();

/************************************main************************************/
int main(void)
{
	TestCalculate();
	
	return (0);
}

/************************************Functions************************************/
void TestCalculate()
{
	char* expression1 = "1.4+2*3/2-3\0";
	double expected_result1 = 1.4;
	double result1 = 0;
	calc_status_t expected_status1 = SUCCESS;
	calc_status_t status1 = 10;

	char* expression2 = "(1.4+2)*3/2\0";
	double expected_result2 = 5.1;
	double result2 = 0;
	calc_status_t expected_status2 = SUCCESS;
	calc_status_t status2 = 10;
	
	char* expression3 = "(8/(1.5+1))*3/2\0";
	double expected_result3 = 4.80000000000000071054;
	double result3 = 0;
	calc_status_t expected_status3 = SUCCESS;
	calc_status_t status3 = 10;
	
	char* expression4 = "(1.4+2*3/2\0";
	calc_status_t expected_status4 = SYNTAX_ERROR;
	calc_status_t status4 = 10;
	
	char* expression5 = "(1.4+2))*3/2\0";
	calc_status_t expected_status5 = SYNTAX_ERROR;
	calc_status_t status5 = 10;
	
	char* expression6 = "1.4+2*3/0\0";
	calc_status_t expected_status6 = ARITHMETIC_ERROR;
	calc_status_t status6 = 10;
	
	
    printf(TITLE "Test: Calculate()\n" RESET);
    
    status1 = Calculate(expression1, &result1);
    if (status1 == expected_status1 && result1 == expected_result1)
    {
    	printf(GREEN "Test 1 PASSED!\n" RESET);
    }
    else
    {
    	printf(RED "Test 1 FAILED!\n" RESET);
    	printf("status = %d , result = %f \n", status1, result1);
    }
    printf("\n");
    
    status2 = Calculate(expression2, &result2);   
    if (status2 == expected_status2 && result2 == expected_result2)
    {
    	printf(GREEN "Test 2 PASSED!\n" RESET);
    }
    else
    {
    	printf(RED "Test 2 FAILED!\n" RESET);
    	printf("status = %d , result = %f \n", status2, result2);
    }
    printf("\n");

    status3 = Calculate(expression3, &result3);    
    if (status3 == expected_status3 && result3 == expected_result3)
    {
    	printf(GREEN "Test 3 PASSED!\n" RESET);
    }
    else
    {
    	printf(RED "Test 3 FAILED!\n" RESET);
    	printf("status = %d , result = %.20f \n", status3, result3);
    }
    printf("\n");

    status4 = Calculate(expression4, &result3);    
    if (status4 == expected_status4)
    {
    	printf(GREEN "Test 4 PASSED!\n" RESET);
    }
    else
    {
    	printf(RED "Test 4 FAILED!\n" RESET);
    	printf("status = %d \n", status4);
    }
    printf("\n");
 
	status5 = Calculate(expression5, &result3);   
    if (status5 == expected_status5)
    {
    	printf(GREEN "Test 5 PASSED!\n" RESET);
    }
    else
    {
    	printf(RED "Test 5 FAILED!\n" RESET);
    	printf("status = %d \n", status5);
    }
    printf("\n");

    status6 = Calculate(expression6, &result3);
    if (status6 == expected_status6)
    {
    	printf(GREEN "Test 6 PASSED!\n" RESET);
    }
    else
    {
    	printf(RED "Test 6 FAILED!\n" RESET);
    	printf("status = %d \n", status6);
    }
}
