/************************************includes************************************/
#define _POSIX_C_SOURCE 200112L

#include <stdio.h> /* printf */

#include "scheduler.h"

/************************************define************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define TITLE "\033[1;4m"

#define TRUE (1)
#define FALSE (0)
#define TO_RESCHEDULE (1)
#define DONT_RESCHEDULE (0)

typedef struct {
	int reschedule_counter;
	sched_t* s;
} stop_params_t;

/************************************Functions Forward Decleration************************************/
void TestSchedCreateAndDestroy();
void TestSchedAddTask();
void TestSchedRemoveTask();
void TestSchedIsEmpty();
void TestSchedSize();
void TestSchedRunAndClear();
void TestSchedRunAndStop();
void TestStopWithReschedule();

static int ActionGetSize(void* param);
static int ActionIsEmpty(void* param);
static int ActionStopWithoutReschedule(void* param);
static int ActionStopWithReschedule(void* param);
static int ActionRescheduleThree(void* param);
static int ActionRemoveSelf(void* param);
static int ActionCreateScheduler(void* param);
static int ActionPrintSizeBeforeClear(void* param);
static int ActionClearAllWithPrint(void* param);

static void GenericCleanup(void* param);

static size_t cleanup_counter = 0;
static int reschedule_counter = 0;
static ilrd_uid_t target_uid;

/************************************main************************************/
int main(void)
{
	TestSchedCreateAndDestroy();
	TestSchedAddTask();
	TestSchedRemoveTask();
	TestSchedIsEmpty();
	TestSchedSize();
	TestSchedRunAndClear();
	TestSchedRunAndStop();
	TestStopWithReschedule();
	
    printf("\n");
    	
	return (0);
}

/************************************Functions************************************/
void TestSchedCreateAndDestroy()
{
	sched_t* s = SchedCreate();
	
	printf(TITLE "Test: SchedCreate() and SchedDestroy()\n" RESET);
	
	if (NULL != s)
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
	}
	
	SchedDestroy(s);
}

void TestSchedAddTask()
{
	sched_t* s = SchedCreate();
	time_t now = time(NULL);

	printf(TITLE "\nTest: SchedAddTask()\n" RESET);

	SchedAddTask(s, now, ActionGetSize, s, GenericCleanup, NULL, 1);
	SchedAddTask(s, now + 1, ActionIsEmpty, s, GenericCleanup, NULL, 1);

	if (2 == SchedSize(s))
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
	}

	SchedDestroy(s);
}

void TestSchedRemoveTask()
{
	sched_t* s = SchedCreate();
	time_t now = time(NULL);
	ilrd_uid_t uid1;

	printf(TITLE "\nTest: SchedRemoveTask()\n" RESET);

	uid1 = SchedAddTask(s, now, ActionGetSize, s, GenericCleanup, NULL, 1);
	
	SchedRemoveTask(s, uid1);

	if (TRUE == SchedIsEmpty(s))
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
	}
	
	SchedRemoveTask(s, uid1);

	printf(GREEN "Test 2 Passed!\n" RESET);

	SchedDestroy(s);
}

void TestSchedIsEmpty()
{
	sched_t* s = SchedCreate();
	time_t now = time(NULL);

	printf(TITLE "\nTest: SchedIsEmpty()\n" RESET);

	if (TRUE == SchedIsEmpty(s))
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
	}

	SchedAddTask(s, now, ActionGetSize, s, GenericCleanup, NULL, 1);

	if (FALSE == SchedIsEmpty(s))
	{
		printf(GREEN "Test 2 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 2 Failed!\n" RESET);
	}

	SchedDestroy(s);
}

void TestSchedSize()
{
	sched_t* s = SchedCreate();
	time_t now = time(NULL);

	printf(TITLE "\nTest: SchedSize()\n" RESET);

	if (0 == SchedSize(s))
	{
		printf(GREEN "Test 1 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 1 Failed!\n" RESET);
	}

	SchedAddTask(s, now, ActionGetSize, s, GenericCleanup, NULL, 1);
	SchedAddTask(s, now, ActionGetSize, s, GenericCleanup, NULL, 1);

	if (2 == SchedSize(s))
	{
		printf(GREEN "Test 2 Passed!\n" RESET);
	}
	else
	{
		printf(RED "Test 2 Failed!\n" RESET);
	}

	SchedDestroy(s);
}

void TestSchedRunAndClear()
{
    sched_t* s = SchedCreate();
    time_t now = time(NULL);
    status_t result;

	printf(TITLE "\nTest: SchedRun() and SchedClear()\n" RESET);

    SchedAddTask(s, now + 1, ActionGetSize, s, GenericCleanup, NULL, 1);
    SchedAddTask(s, now + 2, ActionIsEmpty, s, GenericCleanup, NULL, 1);
    SchedAddTask(s, now + 3, ActionPrintSizeBeforeClear, s, GenericCleanup, NULL, 1);
    target_uid = SchedAddTask(s, now + 4, ActionRemoveSelf, s, GenericCleanup, NULL, 1);
    SchedAddTask(s, now + 5, ActionCreateScheduler, NULL, GenericCleanup, NULL, 1);
    SchedAddTask(s, now + 6, ActionClearAllWithPrint, s, GenericCleanup, NULL, 1);	

    result = SchedRun(s);
    sleep(5);
        
    if (STOPPED_DEFAULT == result && cleanup_counter >= 5)
    {
        printf(GREEN "Test 1 Passed!\n" RESET);
    }
    else
    {
        printf(RED "Test 1 Failed!\n" RESET);
        printf("reschedule_counter = %d\n", reschedule_counter);
		printf("cleanup_counter = %lu\n", cleanup_counter);        
        printf("result = %d\n", result);
    }
    
    SchedDestroy(s);
}
void TestSchedRunAndStop()
{
    sched_t* s = SchedCreate();
    time_t now = time(NULL);
    status_t result;
    reschedule_counter = 3;

	printf(TITLE "\nTest: SchedRun() and SchedStop()\n" RESET);

    SchedAddTask(s, now + 1, ActionGetSize, s, GenericCleanup, NULL, 1);
    SchedAddTask(s, now + 2, ActionIsEmpty, s, GenericCleanup, NULL, 1);
    SchedAddTask(s, now + 3, ActionPrintSizeBeforeClear, s, GenericCleanup, NULL, 1);
    target_uid = SchedAddTask(s, now + 4, ActionRemoveSelf, s, GenericCleanup, NULL, 1);
    SchedAddTask(s, now + 5, ActionCreateScheduler, NULL, GenericCleanup, NULL, 1);
	SchedAddTask(s, now + 6, ActionRescheduleThree, &reschedule_counter, GenericCleanup, NULL, 1);
    SchedAddTask(s, now + 10, ActionStopWithoutReschedule, s, GenericCleanup, NULL, 1);
    
    result = SchedRun(s);
    sleep(5);
    
    if (STOPPED_BY_USER == result && 0 == reschedule_counter && cleanup_counter >= 5)
    {
        printf(GREEN "Test 1 Passed!\n" RESET);
    }
    else
    {
        printf(RED "Test 1 Failed!\n" RESET);
        printf("reschedule_counter = %d\n", reschedule_counter);
		printf("cleanup_counter = %lu\n", cleanup_counter);        
        printf("result = %d\n", result);
    }
    
    SchedDestroy(s);
}

void TestStopWithReschedule()
{
    sched_t* s = SchedCreate();
    time_t now = time(NULL);
    status_t result;
	stop_params_t stop_params = {1, NULL};
	
	printf(TITLE "\nTest: Stop() with reschedule\n" RESET);

	stop_params.reschedule_counter = 1;
	stop_params.s = s;
	
    SchedAddTask(s, now + 1, ActionGetSize, s, GenericCleanup, NULL, 1);
    SchedAddTask(s, now + 2, ActionIsEmpty, s, GenericCleanup, NULL, 1);
    SchedAddTask(s, now + 3, ActionStopWithReschedule, &stop_params, GenericCleanup, NULL, 3);
        
    SchedAddTask(s, now + 4, ActionGetSize, s, GenericCleanup, NULL, 1);
    SchedAddTask(s, now + 5, ActionIsEmpty, s, GenericCleanup, NULL, 1);

    result = SchedRun(s);
	sleep(5);

    if (STOPPED_BY_USER == result)
    {
        printf(GREEN "Test 1 Passed!\n" RESET);
    }
    else
    {
        printf(RED "Test 1 Failed!\n" RESET);
    }
    
    result = SchedRun(s);
    sleep(5);
    
    if (STOPPED_BY_USER == result)
    {
        printf(GREEN "Test 2 Passed!\n" RESET);
    }
    else
    {
        printf(RED "Test 2 Failed!\n" RESET);
    }
    
    SchedDestroy(s);
}

static int ActionGetSize(void* param)
{
	sched_t* s = (sched_t*)param;
	
	printf("action GetSize: %lu\n", SchedSize(s));
	
	return (DONT_RESCHEDULE);
}

static int ActionIsEmpty(void* param)
{
	sched_t* s = (sched_t*)param;

	if (FALSE == SchedIsEmpty(s))
	{
		printf("action IsEmpty: FALSE\n");
	}
	else
	{
		printf("action IsEmpty: TRUE\n");
	}
	
	return (DONT_RESCHEDULE);
}

static int ActionStopWithoutReschedule(void* param)
{
	sched_t* s = (sched_t*)param;
	
	SchedStop(s);
	
	return (DONT_RESCHEDULE);
}

static int ActionStopWithReschedule(void* param)
{
	stop_params_t* params = (stop_params_t*)param;
	
	int counter = params->reschedule_counter;
	sched_t* s = params->s;
	
	SchedStop(s);
	
	if (counter > 0)
	{
		params->reschedule_counter--;
		
		return (TO_RESCHEDULE);
	}
	
	return (DONT_RESCHEDULE);
}

static int ActionRemoveSelf(void* param)
{
    sched_t* s;
    s = (sched_t*)param;
    
    SchedRemoveTask(s, target_uid);
    
    return (DONT_RESCHEDULE);
}

static int ActionCreateScheduler(void* param)
{
    sched_t* new_s;
    (void)param;
    new_s = SchedCreate();
    
    SchedDestroy(new_s);
    
    return (DONT_RESCHEDULE);
}

static int ActionRescheduleThree(void* param)
{
	int* counter = (int*)param;
	
	if (*counter > 0)
	{
		(*counter)--;
		
		return (TO_RESCHEDULE);
	}
	
	return (DONT_RESCHEDULE);
}

static int ActionPrintSizeBeforeClear(void* param)
{
    sched_t* s;
    s = (sched_t*)param;
    
    printf("Before Clear: size = %lu\n", SchedSize(s));
    
    return (DONT_RESCHEDULE);
}

static int ActionClearAllWithPrint(void* param)
{
    sched_t* s = (sched_t*)param;

    SchedClear(s);
    
    printf("After Clear: size = %lu\n" , SchedSize(s));
    
    return (DONT_RESCHEDULE);
}

static void GenericCleanup(void* param)
{
	(void)param;
	
	cleanup_counter++;
}
