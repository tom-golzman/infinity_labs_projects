#pragma once

/*----------------------define----------------------*/
#define MAX_NAME_LENGTH 20

/*---------------------typedef---------------------*/
typedef int sid_t;

typedef enum {
	SID_NOT_FOUND = -3,
	FILE_NOT_FOUND,
	ERROR,
	SUCCESS
} status_t;

typedef struct{
	float sociology;
	float psychology;
	float literature;
} humanistic_grades_t;

typedef struct {
	float math;
	float chemistry;
	float physics;
} real_grades_t;

typedef struct {
	humanistic_grades_t humanistic;
	real_grades_t real;
	float sports;
} grades_t;

typedef struct{
	char first_name[MAX_NAME_LENGTH];
	char last_name[MAX_NAME_LENGTH];
	grades_t grades;
} student_t;

/*---------------------Functions Forward Decleration---------------------*/
sid_t SaveStudent(student_t* student, const char* file_path);
status_t LoadStudent(sid_t sid, const char* file_path, student_t* returned_student);

