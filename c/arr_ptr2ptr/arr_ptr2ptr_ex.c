#include <stddef.h> /* For size_t */
#include <stdlib.h> /* For malloc */
#include <assert.h> 
#include <stdio.h> /* printf */
#include <ctype.h> /* tolower */
#include <string.h> /* strlen */
#include "../pointers_pt.2/strings.h" /* StrDup */

extern char** environ; /* Access the environment variables */

void SumRows(const int* table[], long* sums, size_t rows, size_t columns)
{
	size_t i = 0, j = 0;
	long row_sum = 0;
	const int* row_ptr = *table;
	
	for (i = 0; i < rows; i++)
	{
		row_sum = 0;
		for (j = 0; j < columns; j++)
		{
			row_sum += table[i][j];	
		}
		
		*sums = row_sum;
		sums++;
		row_ptr++;
	}	
}

size_t StayAlive(size_t size)
{
	size_t* arr = (size_t*)malloc(sizeof(size_t) * size);
	size_t i = 0;
	
	arr[size-1] = 0;
	
	for (i = 0; i < size - 1; i++)
	{
		arr[i] = i + 1;
	}
	
	i = 0;
	
	while (arr[i] != i)
	{
		arr[i] = arr[arr[i]];
		i = arr[i];
	} 
	
	free(arr);
	
	arr = NULL;
	
	printf("\nThe last soldier is: %lu", i);
	
	return (i);
}

void PrintDataTypes(void)
{
	printf("Sizeof(short int) = %lu\n", sizeof(short int));
	printf("Sizeof(unsigned short int) = %lu\n", sizeof(unsigned short int));
	printf("Sizeof(unsigned int) = %lu\n", sizeof(unsigned int));
	printf("Sizeof(int) = %lu\n", sizeof(int));
	printf("Sizeof(long int) = %lu\n", sizeof(long int));
	printf("Sizeof(unsigned long int) = %lu\n", sizeof(unsigned long int));
	printf("Sizeof(signed char) = %lu\n", sizeof(signed char));
	printf("Sizeof(unsigned char) = %lu\n", sizeof(unsigned char));
	printf("Sizeof(float) = %lu\n", sizeof(float));
	printf("Sizeof(double) = %lu\n", sizeof(double));
	printf("Sizeof(long double) = %lu\n", sizeof(long double)); 
}
/*
static char* StrDup(const char* str)
{
	size_t len = strlen(str) + 1;
	char* dup = (char*)malloc(len * sizeof(char)); 
	
	assert(str != NULL);
	assert(dup != NULL);
	
	strncpy(dup, str, len);
	
	return (dup);
}
*/
size_t GetEnvironSize()
{
	size_t env_vars_size = 0, i = 0;
	
	while (environ[i] != NULL)
	{
		i++;
		env_vars_size++;
	}
	
	return (env_vars_size);
}

void FreeBuffer(char** arr, size_t size)
{
	size_t i = 0;
	
	for (i = 0; i < size; i++)
	{
		free(arr[i]);
		arr[i] = NULL;
	}
	
	arr = NULL;
}

char* EnvVarToLower(char* env_var)
{
	size_t env_var_runner = 0;
	
	while (env_var[env_var_runner] != '\0')
	{
		env_var[env_var_runner] = tolower(env_var[env_var_runner]);
		env_var_runner++;
	}
	
	return (env_var);
}

int PrintEnvVars(void)
{
	size_t env_vars_size = 0, i = 0;
	char** environ_copy = NULL;
	char* curr_env_var = NULL;
		
	env_vars_size = GetEnvironSize();
	
	environ_copy = (char**)malloc(sizeof(char*) * (env_vars_size + 1)); /*+1 for NULL */
	
	if (environ_copy == NULL)
	{
		return (-1);
	}
	
	printf("Environment variables in lower case:\n");
	
	for (i = 0; i < env_vars_size; i++)
	{
		environ_copy[i] = StrDup(environ[i]);
		
		if (environ_copy[i] == NULL)
		{
			FreeBuffer(environ_copy, i);
			return (-1);
		}
		
		curr_env_var = environ_copy[i];

		curr_env_var = EnvVarToLower(curr_env_var);
		
		printf("%s\n", environ_copy[i]);
	}
	
	environ_copy[env_vars_size] = NULL;
	
	FreeBuffer(environ_copy, env_vars_size);

	free(environ_copy);
	environ_copy = NULL;
	
	return (0);
}












