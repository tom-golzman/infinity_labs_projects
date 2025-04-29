/************************************includes************************************/
#include <stdio.h> 	/* printf */
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <unistd.h> /* sleep */

#include "task.h"

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define TITLE "\033[1;4m"

/************************************Functions Forward Decleration************************************/
void TestTaskCreate();
void TestTaskGetId();
void TestTaskGetTime();
void TestTaskExec();
void TestTaskUpdateTimeToRun();

int DummyAction(void* param);
void DummyCleanup(void* param);

/************************************main************************************/
int main()
{
	TestTaskCreate();
	TestTaskGetId();
	TestTaskGetTime();
	TestTaskExec();
	TestTaskUpdateTimeToRun();
	
	printf("\n");
	
	return (0);
}

/************************************Functions************************************/
void TestTaskCreate()
{
	task_t* task = TaskCreate(time(NULL), DummyAction, NULL, DummyCleanup, NULL, 5);

	printf(TITLE "Test: TaskCreate and TaskDestroy()\n" RESET);

	if (NULL != task)
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
	}
	
	TaskDestroy(task);
}

void TestTaskGetId()
{
	task_t* task = TaskCreate(time(NULL), DummyAction, NULL, DummyCleanup, NULL, 5);

	printf(TITLE "\nTest: TaskGetId()\n" RESET);

	if (!UIDIsSame(TaskGetId(task), invalid_uid))
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
	}

	TaskDestroy(task);
}

void TestTaskGetTime()
{
	time_t curr_time = time(NULL);
	task_t* task = TaskCreate(curr_time, DummyAction, NULL, DummyCleanup, NULL, 5);

	printf(TITLE "\nTest: TaskGetTime()\n" RESET);

	if (TaskGetTime(task) == curr_time)
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
	}

	TaskDestroy(task);
}

void TestTaskExec()
{
	task_t* task = TaskCreate(time(NULL), DummyAction, NULL, DummyCleanup, NULL, 5);

	printf(TITLE "\nTest: TaskExec()\n" RESET);

	if (TaskExec(task) == 1)
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
	}

	TaskDestroy(task);
}

void TestTaskUpdateTimeToRun()
{
	task_t* task = TaskCreate(time(NULL), DummyAction, NULL, DummyCleanup, NULL, 2);
	time_t before = TaskGetTime(task);
	time_t after = -1;
	
	printf(TITLE "\nTest: TaskUpdateTimeToRun()\n" RESET);

	sleep(1); /* to have bigger curr_time */
	TaskUpdateTimeToRun(task);
	after = TaskGetTime(task);

	if (after > before)
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
	}

	TaskDestroy(task);
}

/************************************Helper Functions************************************/
int DummyAction(void* param)
{
	(void)param;
	
	printf("dummy action\n");
	
	return (1); /* to reschedule one time */
}

void DummyCleanup(void* param)
{
	(void)param;
}

