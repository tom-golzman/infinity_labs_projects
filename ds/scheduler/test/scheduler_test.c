#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <unistd.h>

#include "scheduler.h"
#include "test_utils.h"

#define TO_RESCHEDULE (1)
#define DONT_RESCHEDULE (0)

typedef struct {
	int reschedule_counter;
	sched_t* s;
} stop_params_t;

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
	return 0;
}

void TestSchedCreateAndDestroy()
{
	sched_t* s = SchedCreate();
	TITLE("Test: SchedCreate() and SchedDestroy()");
	RUN_TEST("SchedCreate returns non-null", (NULL != s));
	SchedDestroy(s);
}

void TestSchedAddTask()
{
	sched_t* s = SchedCreate();
	time_t now = time(NULL);

	TITLE("Test: SchedAddTask()");
	SchedAddTask(s, now, ActionGetSize, s, GenericCleanup, NULL, 1);
	SchedAddTask(s, now + 1, ActionIsEmpty, s, GenericCleanup, NULL, 1);

	RUN_TEST("SchedSize == 2 after 2 inserts", (2 == SchedSize(s)));
	SchedDestroy(s);
}

void TestSchedRemoveTask()
{
	sched_t* s = SchedCreate();
	time_t now = time(NULL);
	ilrd_uid_t uid1;

	TITLE("Test: SchedRemoveTask()");
	uid1 = SchedAddTask(s, now, ActionGetSize, s, GenericCleanup, NULL, 1);
	SchedRemoveTask(s, uid1);

	RUN_TEST("IsEmpty after RemoveTask", TRUE == SchedIsEmpty(s));

	SchedRemoveTask(s, uid1);
	RUN_TEST("RemoveTask again does not crash", TRUE);

	SchedDestroy(s);
}

void TestSchedIsEmpty()
{
	sched_t* s = SchedCreate();
	time_t now = time(NULL);

	TITLE("Test: SchedIsEmpty()");
	RUN_TEST("IsEmpty after creation", TRUE == SchedIsEmpty(s));

	SchedAddTask(s, now, ActionGetSize, s, GenericCleanup, NULL, 1);
	RUN_TEST("IsEmpty == FALSE after insert", FALSE == SchedIsEmpty(s));

	SchedDestroy(s);
}

void TestSchedSize()
{
	sched_t* s = SchedCreate();
	time_t now = time(NULL);

	TITLE("Test: SchedSize()");
	RUN_TEST("SchedSize == 0 after creation", 0 == SchedSize(s));

	SchedAddTask(s, now, ActionGetSize, s, GenericCleanup, NULL, 1);
	SchedAddTask(s, now, ActionGetSize, s, GenericCleanup, NULL, 1);

	RUN_TEST("SchedSize == 2 after 2 inserts", 2 == SchedSize(s));

	SchedDestroy(s);
}

void TestSchedRunAndClear()
{
	sched_t* s = SchedCreate();
	time_t now = time(NULL);
	status_t result;

	TITLE("Test: SchedRun() and SchedClear()");
	SchedAddTask(s, now + 1, ActionGetSize, s, GenericCleanup, NULL, 1);
	SchedAddTask(s, now + 2, ActionIsEmpty, s, GenericCleanup, NULL, 1);
	SchedAddTask(s, now + 3, ActionPrintSizeBeforeClear, s, GenericCleanup, NULL, 1);
	target_uid = SchedAddTask(s, now + 4, ActionRemoveSelf, s, GenericCleanup, NULL, 1);
	SchedAddTask(s, now + 5, ActionCreateScheduler, NULL, GenericCleanup, NULL, 1);
	SchedAddTask(s, now + 6, ActionClearAllWithPrint, s, GenericCleanup, NULL, 1);

	result = SchedRun(s);
	sleep(5);

	RUN_TEST("Run-Clear scenario passes", STOPPED_DEFAULT == result && cleanup_counter >= 5);

	SchedDestroy(s);
}

void TestSchedRunAndStop()
{
	sched_t* s = SchedCreate();
	time_t now = time(NULL);
	status_t result;
	reschedule_counter = 3;

	TITLE("Test: SchedRun() and SchedStop()");
	SchedAddTask(s, now + 1, ActionGetSize, s, GenericCleanup, NULL, 1);
	SchedAddTask(s, now + 2, ActionIsEmpty, s, GenericCleanup, NULL, 1);
	SchedAddTask(s, now + 3, ActionPrintSizeBeforeClear, s, GenericCleanup, NULL, 1);
	target_uid = SchedAddTask(s, now + 4, ActionRemoveSelf, s, GenericCleanup, NULL, 1);
	SchedAddTask(s, now + 5, ActionCreateScheduler, NULL, GenericCleanup, NULL, 1);
	SchedAddTask(s, now + 6, ActionRescheduleThree, &reschedule_counter, GenericCleanup, NULL, 1);
	SchedAddTask(s, now + 10, ActionStopWithoutReschedule, s, GenericCleanup, NULL, 1);

	result = SchedRun(s);
	sleep(5);

	RUN_TEST("Run-Stop scenario passes", STOPPED_BY_USER == result && cleanup_counter >= 5);

	SchedDestroy(s);
}

void TestStopWithReschedule()
{
	sched_t* s = SchedCreate();
	time_t now = time(NULL);
	status_t result;
	stop_params_t stop_params = {0};

	TITLE("Test: Stop() with reschedule");

	stop_params.reschedule_counter = 1;
	stop_params.s = s;

	SchedAddTask(s, now + 1, ActionGetSize, s, GenericCleanup, NULL, 1);
	SchedAddTask(s, now + 2, ActionIsEmpty, s, GenericCleanup, NULL, 1);
	SchedAddTask(s, now + 3, ActionStopWithReschedule, &stop_params, GenericCleanup, NULL, 3);
	SchedAddTask(s, now + 4, ActionGetSize, s, GenericCleanup, NULL, 1);
	SchedAddTask(s, now + 5, ActionIsEmpty, s, GenericCleanup, NULL, 1);

	result = SchedRun(s);
	sleep(5);
	RUN_TEST("Stop with reschedule - 1st run", STOPPED_BY_USER == result);

	result = SchedRun(s);
	sleep(5);
	RUN_TEST("Stop with reschedule - 2nd run", STOPPED_BY_USER == result);

	SchedDestroy(s);
}

static int ActionGetSize(void* param)
{
	sched_t* s = (sched_t*)param;
	printf("Action: GetSize = %lu\n", SchedSize(s));
	return DONT_RESCHEDULE;
}

static int ActionIsEmpty(void* param)
{
	sched_t* s = (sched_t*)param;
	printf("Action: IsEmpty = %s\n", SchedIsEmpty(s) ? "TRUE" : "FALSE");
	return DONT_RESCHEDULE;
}

static int ActionStopWithoutReschedule(void* param)
{
	SchedStop((sched_t*)param);
	return DONT_RESCHEDULE;
}

static int ActionStopWithReschedule(void* param)
{
	stop_params_t* p = (stop_params_t*)param;
	SchedStop(p->s);

	if (p->reschedule_counter-- > 0)
	{
		return TO_RESCHEDULE;
	}

	return DONT_RESCHEDULE;
}

static int ActionRemoveSelf(void* param)
{
	SchedRemoveTask((sched_t*)param, target_uid);
	return DONT_RESCHEDULE;
}

static int ActionCreateScheduler(void* param)
{
	sched_t* s = SchedCreate();
	SchedDestroy(s);
	return DONT_RESCHEDULE;
	(void)param;
}

static int ActionRescheduleThree(void* param)
{
	int* counter = (int*)param;

	if ((*counter)-- > 0)
	{
		return TO_RESCHEDULE;
	}

	return DONT_RESCHEDULE;
}

static int ActionPrintSizeBeforeClear(void* param)
{
	printf("Before Clear: size = %lu\n", SchedSize((sched_t*)param));
	return DONT_RESCHEDULE;
}

static int ActionClearAllWithPrint(void* param)
{
	sched_t* s = (sched_t*)param;
	SchedClear(s);
	printf("After Clear: size = %lu\n", SchedSize(s));
	return DONT_RESCHEDULE;
}

static void GenericCleanup(void* param)
{
	(void)param;
	cleanup_counter++;
}
