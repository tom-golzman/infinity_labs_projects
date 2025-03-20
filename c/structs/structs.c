/**
	Written By: Tom Golzman
	Date: 19/03/2025
	Reviewed By: Amir Granot
**/

/*----------------------includes----------------------*/
#include <stdio.h> /* For printf */
#include <stdlib.h> /* For realloc */
#include <float.h> /* For FLT_MAX */
#include <string.h> /* For strlen */
#include <assert.h> /* For assert */

/*----------------------define----------------------*/
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define MAX3(a, b, c) (MAX(MAX(a, b), c))
#define SIZEOF_VAR(var) ((char*)(&var + 1) - (char*)(&var))
#define SIZEOF_TYPE(type) ((size_t)((type*)0 + 1))
#define INT_MAX 2147483647
#define SUCCESS 0
#define FAIL 1
#define ARRAY_SIZE 3
 
/*---------------------typedef---------------------*/
typedef long var_to_ptr_size_t;
typedef void (*print_t)(void* data);
typedef int (*add_t)(void* data, int num);
typedef int (*cleanup_t)(void* data);
typedef struct {
	void* data;
	print_t print;
	add_t add;
	cleanup_t cleanup;
} elem_t;

/*---------------------Functions Forward Decleration---------------------*/
void InitElementsArray(elem_t* elements, int* x, float* f, char* str);
void PrintArray(elem_t* elements_array);
int AddToArray(elem_t* elements_array, int num_to_add);
int CleanupArray(elem_t* elements_array);
void PrintInt(void* data);
void PrintFloat(void* data);
void PrintString(void* data);
int AddInt(void* data, int num);
int AddFloat(void* data, int num);
int AddString(void* data, int num);
int CleanupNothing(void* data);
int CleanupString(void* data);

/*---------------------main---------------------*/
int main()
{
	elem_t elements_array[ARRAY_SIZE];
	int x = 5;
	float f = 3.7f;
	char* str;
	int* x_ptr = &x;
	float* f_ptr = &f;
	
	str = (char*)malloc(strlen("string") + 1);
	if (NULL == str)
	{
		return (FAIL);
	}
	strcpy(str, "string");
	
	InitElementsArray(elements_array, x_ptr, f_ptr, str);
	PrintArray(elements_array);
	AddToArray(elements_array, 5);
	PrintArray(elements_array);
	CleanupArray(elements_array);
	
	return (SUCCESS);
}

void InitElementsArray(elem_t* elements_array, int* x, float* f, char* str)
{
	assert(NULL != elements_array);
	assert(NULL != x);
	assert(NULL != f);
	assert(NULL != str);
	
	elements_array[0].data = (void*)*(var_to_ptr_size_t*)x;
	elements_array[0].print = PrintInt;
	elements_array[0].add = AddInt;
	elements_array[0].cleanup = CleanupNothing;
	
	elements_array[1].data = (void*)*(var_to_ptr_size_t*)f;
	elements_array[1].print = PrintFloat;
	elements_array[1].add = AddFloat;
	elements_array[1].cleanup = CleanupNothing;
	
	elements_array[2].data = str;
	elements_array[2].print = PrintString;
	elements_array[2].add = AddString;
	elements_array[2].cleanup = CleanupString;
}

void PrintArray(elem_t* elements_array)
{
	int i = 0;
		
	for (i = 0; i < ARRAY_SIZE; i++)
	{
		elements_array[i].print(elements_array[i].data);
	}
}

int AddToArray(elem_t* elements_array, int num_to_add)
{
	int i = 0;
	int status = 0;
	
	for (i = 0; i < ARRAY_SIZE; i++)
	{
		status = elements_array[i].add((&elements_array[i].data), num_to_add);
		if (status == FAIL)
		{
			CleanupArray(elements_array);
			return (FAIL);
		}
	}
	
	return (SUCCESS);
}

int CleanupArray(elem_t* elements_array)
{
	int i = 0;
	int status = 0;
	
	for (i = 0; i < ARRAY_SIZE; i++)
	{
		status = elements_array[i].cleanup(elements_array[i].data);
		if (status == FAIL)
		{
			return (FAIL);
		}
	}
	
	return (SUCCESS);
}

void PrintInt(void* data)
{
	int data_as_int = *(int*) (&data);
	
	printf("Int = %d\n", data_as_int);
}

void PrintFloat(void* data)
{
	float data_as_float = *(float*)(&data);
	
	printf("Float = %f\n", data_as_float);
}

void PrintString(void* data)
{
	char* string = (char*) data;
	
	printf("String = %s\n", string);
}

int AddInt(void* data, int num)
{
	int* data_as_int = (int*) data;
	int result = *data_as_int + num;
	
	if ((INT_MAX - *data_as_int) < num)
	{
		return (FAIL);
	}
	
	*data_as_int = result;
	
	return (SUCCESS);
}

int AddFloat(void* data, int num)
{
	float* data_as_float = (float*) data;
	float result = *data_as_float + (float) num;
	
	if ((FLT_MAX - *data_as_float) < (float) num)
	{
		return (FAIL);
	}
	
	*data_as_float = result;
	
	return (SUCCESS);
}

int AddString(void* data, int num)
{
	char int_buffer[10];
	char* temp = NULL;
	char* cast_data = *(char**) data;
	size_t data_len = strlen(cast_data);
	int num_len = sprintf(int_buffer, "%d", num);
	
	temp = realloc(cast_data, (size_t) (data_len + num_len + 1));
	if (NULL == temp)
	{
		return (FAIL);
	}
	
	*(char**) data = temp;
	
	sprintf(*(char**) data + data_len, "%d", num);
	
	return (SUCCESS);
}

int CleanupNothing(void* data)
{
	(void) data;
	return (SUCCESS);
}

int CleanupString(void* data)
{
	if (NULL == data)
	{
		return (FAIL);
	}
	
	free((char*) data);
	
	return (SUCCESS);
}
