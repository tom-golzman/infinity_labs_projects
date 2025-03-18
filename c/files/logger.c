/**
	Written by: Tom Golzman
	Date: 12/03/2025
	Reviewed by: Yuval Almog
**/

#include <stddef.h> /* For size_t */
#include <stdio.h> /* For printf */
#include <string.h> /* For strcmp */
#include <stdlib.h> /* For malloc */

#define ROW_MAX_SIZE 1024

/*---------------------typedef---------------------*/
typedef enum
{
	SUCCESS = 0,
	CONTINUE,
	FILE_NOT_FOUND,
	OPEN_ERROR,
    	CLOSE_ERROR,
    	DELETE_ERROR,
    	WRITE_ERROR,
    	READ_ERROR,
    	ERROR
} status_code_t;
typedef int (*compare_t)(const char* s1, const char* s2);
typedef status_code_t (*operation_t)(const char* str, const char* file_path);
typedef struct
{
	const char* const command;
	compare_t compare;
	operation_t invoke;
} command_handler_t;

/*---------------------Handlers Functions Forward Decleration---------------------*/
static int StringsCompare(const char* str1, const char* str2);
static int CharCompare(const char* str1, const char* str2);
static int AlwaysEqual(const char* str1, const char* str2);
static status_code_t ExitOperation(const char* str, const char* file_path);
static status_code_t RemoveOperation(const char* str, const char* file_path);
static status_code_t CountOperation(const char* str, const char* file_path);
static status_code_t WriteToStartOperation(const char* str, const char* file_path);
static status_code_t WriteToEndOperation(const char* str, const char* file_path);

/*-------------------------Handlers Array initialization-------------------------*/
static command_handler_t handlers[5] = 
	{
        {"-exit\n"  , StringsCompare, ExitOperation        },
        {"-remove\n", StringsCompare, RemoveOperation      },
        {"-count\n" , StringsCompare, CountOperation       },
        {"<"        , CharCompare   , WriteToStartOperation},
        {""         , AlwaysEqual   , WriteToEndOperation  }
    	};

/*---------------------main---------------------*/
int main(int argc, const char** argv)
{
	const char* file_path = argv[1];	
	char string_input[ROW_MAX_SIZE];
	status_code_t status = SUCCESS;
	size_t i = 0;
	
	printf("Please enter a string:\n");

	while (1)
	{
		if(NULL == fgets(string_input, 1024, stdin))
		{
			printf("fgets ERROR");
			return (READ_ERROR);
		}
		
		for (i = 0; i < 5; i++)
		{	
			if (handlers[i].compare(string_input, handlers[i].command) == 0)
			{
				status = handlers[i].invoke(string_input, file_path);
				if (SUCCESS == status && 0 == i)
				{
					return 0;
				}
				break;
			}
		}
	}

	return (0);
}

/*---------------------Handlers Functions---------------------*/

static int StringsCompare(const char* str1, const char* str2)
{
	if (0 == strcmp(str1, str2))
	{
		return (SUCCESS);
	}
	
	return (ERROR);
}

static int CharCompare(const char* str1, const char* str2)
{
	if (strncmp(str1, str2, strlen(str2)) == 0)
	{
		return (SUCCESS);
	}
	
	return (ERROR);
}

static int AlwaysEqual(const char* str1, const char* str2)
{
	return (SUCCESS);
}

static status_code_t ExitOperation(const char* str, const char* file_path)
{
	printf("Exiting the program\n");
	
	return (SUCCESS);
}

static status_code_t RemoveOperation(const char* str, const char* file_path)
{
	if (remove(file_path) == 0)
	{
		printf("File successfully removed!\n");
		return (SUCCESS);
	}
	
	printf("Error removing file!\n");
	
	return (DELETE_ERROR);
}

static status_code_t CountOperation(const char* str, const char* file_path)
{
	size_t count = 0;
	char buffer[ROW_MAX_SIZE];
	FILE* file;
	
	file = fopen(file_path, "r");
	if (NULL == file)
	{
		printf("Error opening file \'%s\' !\n", file_path);
		return (OPEN_ERROR);
	}
	
	while (fgets(buffer, ROW_MAX_SIZE, file) != NULL)
	{
		count++;
	}
	
	printf("Number of lines in file \'%s\': %ld\n", file_path, count);
	fclose(file);
	
	return (SUCCESS);
}

static status_code_t WriteToStartOperation(const char* str, const char* file_path)
{
	char* buffer;
	long file_size = 0;
	FILE* file;
	
	file = fopen(file_path, "r+");
	if (NULL == file)
	{
		printf("Error opening file \'%s\' !\n", file_path);
		return (OPEN_ERROR);
	}
	
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	buffer = (char*)malloc(file_size * (sizeof(char) + 1));
	if (NULL == buffer)
	{
		printf("Malloc failed\n");
		fclose(file);
		return (ERROR);
	}

	fread(buffer, sizeof(char), file_size, file);
	fseek(file, 0, SEEK_SET);
	fwrite(str, sizeof(char), strlen(str), file);
	fwrite(buffer, sizeof(char), file_size, file);
	
	free(buffer);
	fclose(file);
	
	printf("String successfully added to the start of \'%s\' !\n", file_path);

	return (SUCCESS);	
}

static status_code_t WriteToEndOperation(const char* str, const char* file_path)
{
	FILE* file;
	
	file = fopen(file_path, "a");
	if (NULL == file)
	{
		printf("Error opening file!\n");
		return (OPEN_ERROR);
	}
	
	fwrite(str, sizeof(char), strlen(str), file);
	fclose(file);
	
	printf("String successfully added to \'%s\' !\n", file_path);
	
	return (SUCCESS);
}
