/*----------------------includes----------------------*/
#include <stdio.h> /* For printf */
#include <string.h> /* For strcpy */
#include "serialize_structs.h"

/*---------------------Functions Forward Decleration---------------------*/
void TestSaveStudent();
void PrintStudent(student_t* student);

/*---------------------main---------------------*/
int main()
{
	TestSaveStudent();
	
	return 0;
}

void TestSaveStudent()
{
	humanistic_grades_t h_grades0 = {0, 0, 0};
	real_grades_t r_grades0 = {0, 0, 0};
	grades_t grades0 = {h_grades0, r_grades0, 0};
	student_t student0;
	char first_name0[MAX_NAME_LENGTH] = {0};
	char last_name0[MAX_NAME_LENGTH] = {0};

	humanistic_grades_t h_grades1 = {78.7f, 89.4f, 80.1f};
	real_grades_t r_grades1 = {94.6f, 92.8f, 90.4f};
	grades_t grades1 = {h_grades1, r_grades1, 99.9f};
	student_t student1;
	char* first_name1 = "Tom";
	char* last_name1 = "Golzman";
	const char* file_path = "/home/tom/git/c/serialize_structs/file";
	
	humanistic_grades_t h_grades2 = {18.7f, 19.4f, 30.1f};
	real_grades_t r_grades2 = {34.6f, 22.8f, 10.4f};
	grades_t grades2 = {h_grades2, r_grades2, 39.9f};
	student_t student2;
	char* first_name2 = "Moshe";
	char* last_name2 = "Israel"; 
	
	strcpy(student0.first_name, first_name0);
	strcpy(student0.last_name, last_name0);
	student0.grades = grades0;
	
	strcpy(student1.first_name, first_name1);
	strcpy(student1.last_name, last_name1);
	student1.grades = grades1;
	SaveStudent(&student1, file_path); 
	
	strcpy(student2.first_name, first_name2);
	strcpy(student2.last_name, last_name2);
	student2.grades = grades2;
	SaveStudent(&student2, file_path); 
	
	
	printf("load: %d", LoadStudent(1, file_path, &student0));
	PrintStudent(&student0);
}

void PrintStudent(student_t* student)
{
    printf("-------------------------------------------------\n");
/*    printf("SID: %d\n", student->sid); */
    printf("Name: %s %s\n", student->first_name, student->last_name);
    printf("Grades:\n");
    printf("  Sociology: %.2f\n", student->grades.humanistic.sociology);
    printf("  Psychology: %.2f\n", student->grades.humanistic.psychology);
    printf("  Literature: %.2f\n", student->grades.humanistic.literature);
    printf("  Math: %.2f\n", student->grades.real.math);
    printf("  Chemistry: %.2f\n", student->grades.real.chemistry);
    printf("  Physics: %.2f\n", student->grades.real.physics);
    printf("  Sports: %.2f\n", student->grades.sports);
    printf("-------------------------------------------------\n");
}


