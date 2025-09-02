#ifndef __ILRD_SCHEDULER_H__
#define __ILRD_SCHEDULER_H__

#include <time.h>
#include <stddef.h>
#include <stdlib.h>

#include "uid.h"

typedef struct scheduler sched_t;

enum { NOT_RESCHEDULE = 0, TO_RESCHEDULE = 1 };

typedef enum {
	INTERNAL_ERROR = -1,
	STOPPED_DEFAULT,
	ALREADY_RUNNING,
	STOPPED_BY_USER
} status_t;

/*
	DESCRIPTION: an action function signature that is executed when the task is removed. to be implemented by the user.
	RETURNS: int - reschedule or not
	ARGUMENTS: 
			@void* - parameters to pass to the function
	LIMITATIONS: (none)
	NOTES: (none)
*/
typedef int (*sched_action_t)(void* optinal_param);

/*
	DESCRIPTION: a cleanup function signature that is executed when the task is removed (see NOTES) from the scheduler. to be implemented by the user.
	RETURNS: (none)
	ARGUMENTS: 
			@void* - parameters to pass to the function
	LIMITATIONS: (none)
	NOTES: (none)
*/
typedef void (*sched_cleanup_t)(void* optinal_param);

/*
	DESCRIPTION: Creates a scheduler.
	RETURNS: sched_t* a pointer to a scheduler, or NULL on failure.
	ARGUMENTS: (void)
	LIMITATIONS: (none)
	NOTES: (none)
	COMPLEXITY: 
			Time: O(1)
*/
sched_t* SchedCreate(void);

/*
	DESCRIPTION: Destroys the scheduler
	RETURNS: (void)
	ARGUMENTS:
			@sched_t* - pointer to scheduler object created with SchedCreate().
	LIMITATIONS:
			- undefined behavior when scheduler is NULL.
			- undefined behavior when scheduler is running.
	NOTES: (none)
	COMPLEXITY: 
			Time: O(n)
*/
void SchedDestroy(sched_t* scheduler);

/*
	DESCRIPTION: Add a task to the scheduler tasks list.
	RETURNS: ilrd_uid_t assigned to the newly added task, on error returns an invalid ilrd_uid_t (check with invalid_uid). 
	ARGUMENTS: 
			@sched_t* - pointer to scheduler object created with SchedCreate().
			@time_t - time t with precision of seconds.
			@sched_action_t - pointer to a function the task will run.
			@sched_cleanup_t - pointer to a cleanup function the task will run.
			@void* action_param - the param to pass to the task's action function.
			@void* cleanup_param - the param to pass to the task's cleanup function.
	LIMITATIONS: undefined behaviour in case scheduler/sched_action_t/sched_cleanup_t is NULL.
	NOTES: 
			- When more than 1 task with the same time are added, there is no guarentee which of them will run first.
			- tasks time determines priority and a guarentee it will not start until that time was reached. Which means:
			- a task will not start before the specified time.
			- if the specified time for a task passed and the task was not started yet (either the scheduler is currently in STOPPED state or another task is currently running), it will run when the scheduler is in RUNNING state and when it's the first in the queue.
			- tasks added with passed time will have higher priority and will run as soon as possible.
	COMPLEXITY: 
			Time: O(n)
*/
ilrd_uid_t SchedAddTask(sched_t* scheduler, time_t execution_time, sched_action_t action, void* action_param, sched_cleanup_t cleanup, void* cleanup_param, unsigned long interval);

/*
	DESCRIPTION: Removes a task from a scheduler.
	RETURNS: (void)
	ARGUMENTS: 
			@ilrd_uid_t - task's uid to be removed.
	LIMITATIONS: undefined behaviour when s is NULL.
	NOTES: 
			- Can be used internally from a sched_func_t.
			- The current running task can remove itself.
			- If the task_id doesnt exists in the scheduler, the function does nothing.
	COMPLEXITY: 
			Time: O(n)
*/
void SchedRemoveTask(sched_t* s, ilrd_uid_t task_id);

/*
	DESCRIPTION: returns whether the scheduler is empty.
	RETURNS: 1 on TRUE and 0 on FALSE
	ARGUMENTS: 
			@sched_t* - pointer to a scheduler created with SchedCreate().
	LIMITATIONS: undefined behaviour when s is NULL.
	NOTES: if the last task is running, the scheduler isn't empty.
	COMPLEXITY: 
			Time: O(1)
*/
int SchedIsEmpty(const sched_t* s);

/*
	DESCRIPTION: Get the number of tasks not done executing in the scheduler.
	RETURNS: number of tasks in the scheduler
	ARGUMENTS: 
			@sched_t* - pointer to a scheduler object created with SchedCreate().
	LIMITATIONS: undefined behaviour when s is NULL.
	NOTES: if the scheduler is running - the size includes the currently running task. 
	COMPLEXITY: 
			Time: O(n)
*/
size_t SchedSize(const sched_t* s);

/*
	DESCRIPTION: starts the scheduler execution
	RETURNS: 2 STOPPED_BY_USER, 1 ALREADY_RUNNING, 0 STOPPED_DEFAULT (SUCCESS - empty scheduler), -1 on ERROR (internal error)
	ARGUMENTS: 
			@sched_t* - pointer to a scheduler object created with SchedCreate().
	LIMITATIONS: undefined behaviour when s is NULL.
	NOTES: if the scheduler is running - nothing happens.
	COMPLEXITY: 
			Time: O(n)
*/
status_t SchedRun(sched_t* s);

/*
	DESCRIPTION: stops the scheduler execution.
	RETURNS: (none).
	ARGUMENTS: 
			@sched_t* - pointer to a scheduler object created with SchedCreate().
	LIMITATIONS: undefined behaviour when s is NULL.
	NOTES: 
			- after SchedStop() was called and the scheduler is in STOPPED state, subsequent calls do nothing and will just get the current state.
	COMPLEXITY: 
			Time: O(1)
*/
void SchedStop(sched_t* s);

/*
	DESCRIPTION: clear all tasks in scheduler.
	RETURNS: (none)
	ARGUMENTS: 
			@sched_t* - pointer to a scheduler object created with SchedCreate()
	LIMITATIONS: undefined behaviour when s is NULL.
	NOTES: 
	COMPLEXITY: 
			Time: O(n)
*/
void SchedClear(sched_t* s);

#endif /* __ILRD_SCHEDULER_H__ */
