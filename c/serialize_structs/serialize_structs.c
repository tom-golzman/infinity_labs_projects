/**
	Written By: Tom Golzman
	Date: 20/03/2025
	Reviewed By: Amir Granot
**/

/*----------------------includes----------------------*/
#include <stdio.h> /* For printf */
#include <string.h> /* For strcpy */
#include "serialize_structs.h"

/*----------------------define----------------------*/
#define NUM_STUDENTS_TO_READ 10
#define STUDENT_SIZE ((sizeof(sid_t))+(sizeof(char)*MAX_NAME_LENGTH*2)+(sizeof(float)*3)+(sizeof(float)*3)+(sizeof(float)*1))

/*---------------------typedef---------------------*/
typedef struct {
	sid_t sid;
	student_t student;
} wrapped_student_t;

/*---------------------Functions Forward Decleration---------------------*/
sid_t SaveStudent(student_t* student, const char* file_path);
sid_t GenerateSid(const char* file_path);
wrapped_student_t CreateNewWrappedStudent(student_t* student_to_wrap, sid_t sid);
status_t SaveStudentToFile(wrapped_student_t wrapped_student, const char* file_path);
status_t SaveSid(sid_t sid, FILE* file);
status_t SaveStudentName(char* name, FILE* file);
status_t SaveGrades(grades_t* grades, FILE* file);
status_t SaveHumanisticGrades(humanistic_grades_t* humanistic_grades, FILE* file);
status_t SaveRealGrades(real_grades_t* real_grades, FILE* file);
status_t SaveFloat(float float_to_save, FILE* file);
status_t LoadStudent(sid_t sid, const char* file_path, student_t* returned_student);
long SearchSidInFile(sid_t sid_to_search, FILE* file);
status_t SaveToRetStudent(student_t* returned_student, FILE* file);

/*-------------------------functions-------------------------*/
sid_t SaveStudent(student_t* student, const char* file_path)
{	
	static sid_t sid = 0;
	status_t status = 0;
	wrapped_student_t wrapped_student;
	
	if (sid == 0)
	{
		sid = GenerateSid(file_path);
		if (ERROR == sid)
		{
			return (ERROR);
		}
	}
	
	wrapped_student = CreateNewWrappedStudent(student, sid);
	status = SaveStudentToFile(wrapped_student, file_path);
	
	if (SUCCESS != status)
	{
		return (status);
	}
	
	sid++;
	
	return (wrapped_student.sid);
}

sid_t GenerateSid(const char* file_path)
{
	FILE* file;
	sid_t sid = 0;
	long file_size = 0;
	
	file = fopen(file_path, "rb");
	if (NULL == file)
	{
		return(ERROR);
	}
	
	/* Checks if the file is empty */
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	
	if (file_size == 0)
	{
		sid = 1;
		if (EOF == fclose(file))
		{
			return(ERROR);
		}
		
		return (sid);
	}
	
	fseek(file, -(long)STUDENT_SIZE, SEEK_END);
	fread(&sid, sizeof(sid_t), 1, file);
	
	sid++;

	if (EOF == fclose(file))
	{
		return(ERROR);
	}
	
	return (sid);
}

wrapped_student_t CreateNewWrappedStudent(student_t* student_to_wrap, sid_t sid)
{
	wrapped_student_t wrapped_student;
	
	wrapped_student.sid = sid;
	strcpy(wrapped_student.student.first_name, student_to_wrap->first_name);
	strcpy(wrapped_student.student.last_name, student_to_wrap->last_name);
	wrapped_student.student.grades = student_to_wrap->grades;
	
	return (wrapped_student);
}

status_t SaveStudentToFile(wrapped_student_t wrapped_student, const char* file_path)
{
	FILE* file;
	status_t status = 0;
	
	file = fopen(file_path, "ab");
	if (NULL == file)
	{
		printf("Error opening file!\n");
		
		return (FILE_NOT_FOUND);
	}
	
	if ((SUCCESS != SaveSid(wrapped_student.sid, file)) ||
		(SUCCESS != SaveStudentName(wrapped_student.student.first_name, file)) ||
		(SUCCESS != SaveStudentName(wrapped_student.student.last_name, file)) ||
		(SUCCESS != SaveGrades(&wrapped_student.student.grades, file)))
	{
		if (EOF == fclose(file_to_read))
		{
			return(ERROR);
		}
		
		return (status);
	}
	
	if (EOF == fclose(file))
	{
		return(ERROR);
	}
	
	return (status);
}

status_t SaveSid(sid_t sid, FILE* file)
{
	status_t status = 0;
	
	fseek(file, 0, SEEK_END);
	
	status = fwrite(&sid, sizeof(sid_t), 1, file);
	if (status != 1)
	{
		return (ERROR);
	}
	
	return (SUCCESS);
}

status_t SaveStudentName(char* name, FILE* file)
{
	status_t status = 0;
	char padded_name[MAX_NAME_LENGTH] = {0};
	
	strcpy(padded_name, name);
	
	status = fwrite(padded_name, sizeof(char), MAX_NAME_LENGTH, file);
	
	if (MAX_NAME_LENGTH != status)
	{
		return (ERROR);
	}
	
	return (SUCCESS);
}

status_t SaveGrades(grades_t* grades, FILE* file)
{
	status_t status = 0;
	
	if ((SUCCESS != SaveHumanisticGrades(&grades->humanistic, file))||
		(SUCCESS != SaveRealGrades(&grades->real, file)) ||
		(SUCCESS != SaveFloat(grades->sports, file)))
	{
		return (ERROR);
	}
	
	return (SUCCESS);
}

status_t SaveHumanisticGrades(humanistic_grades_t* humanistic_grades, FILE* file)
{
	status_t status = 0;
	
	status = ;
	if ((SUCCESS != SaveFloat(humanistic_grades->sociology, file)) ||
		(SUCCESS != SaveFloat(humanistic_grades->psychology, file)) ||
		(SUCCESS != SaveFloat(humanistic_grades->literature, file)))
	{
		return (ERROR);
	}
	
	return (SUCCESS);
}

status_t SaveRealGrades(real_grades_t* real_grades, FILE* file)
{
	status_t status = 0;
	
	status = ;
	if ((SUCCESS != SaveFloat(real_grades->math, file)) ||
		(SUCCESS != SaveFloat(real_grades->chemistry, file)) ||
		(SUCCESS != SaveFloat(real_grades->physics, file)))
	{
		return (ERROR);
	}
	
	return (SUCCESS);
}

status_t SaveFloat(float float_to_save, FILE* file)
{
	status_t status = 0;

	status = fwrite(&float_to_save, sizeof(float), 1, file);
	if (status != 1)
	{
		return (ERROR);
	}
	
	return (SUCCESS);	
}

status_t LoadStudent(sid_t sid, const char* file_path, student_t* returned_student)
{
	FILE* file_to_read;
	long student_position = 0;
	status_t status = SUCCESS;
	
	file_to_read = fopen(file_path, "rb");
	if (NULL == file_to_read)
	{
		return (FILE_NOT_FOUND);
	}
	
	student_position = SearchSidInFile(sid, file_to_read);
	if (SID_NOT_FOUND == student_position)
	{
		if (EOF == fclose(file_to_read))
		{
			return(ERROR);
		}
		
		return (SID_NOT_FOUND);
	}
	
	fseek(file_to_read, student_position, SEEK_SET);
	status = SaveToRetStudent(returned_student, file_to_read);
	
	if (EOF == fclose(file_to_read))
	{
		return(ERROR);
	}
	
	return (status);
} 

long SearchSidInFile(sid_t sid_to_search, FILE* file)
{
	wrapped_student_t students_buffer[NUM_STUDENTS_TO_READ];
	size_t students_read = 10, i = 0;
	long position = 0;
	
	while (students_read > 0)
	{
		students_read = fread(students_buffer, STUDENT_SIZE, NUM_STUDENTS_TO_READ, file);
		for (i = 0; i < students_read; i++)
		{
			if (students_buffer[i].sid == sid_to_search)
			{
				return (position + (i * STUDENT_SIZE) + sizeof(sid_t));
			}
			position = ftell(file);
		}
	}
	
	return (SID_NOT_FOUND);
}

status_t SaveToRetStudent(student_t* returned_student, FILE* file)
{
	if (fread(returned_student, STUDENT_SIZE, 1, file) != 1)
	{
		return (ERROR);
	}
	
	return (SUCCESS);
}
