#ifndef __ILRD_TASK__
#define __ILRD_TASK__

/************************************includes************************************/
#include <time.h> /* time_t */

#include "uid.h" /* ILRD_uid_t */

/************************************typedef************************************/
typedef struct task task_t;

/*
	DESCRIPTION: function signature to be implemented by the user that optionally accepts a param.
	RETURNS: enum - reschedule or not / int 
	ARGUMENTS: 
			@void* - param to pass to the function
	LIMITATIONS: (none)
	NOTES: (none)
*/
typedef int (*task_action_t)(void*);

/*
	DESCRIPTION: function signature to be implemented by the user that optionally accepts a param.
	RETURNS: (none)
	ARGUMENTS: 
			@void* - param to pass to the function
	LIMITATIONS: (none)
	NOTES: (none)
*/
typedef void (*task_cleanup_t)(void*);

/************************************Functions************************************/
/*
	DESCRIPTION: creates a task to be used in other task functions.
	RETURNS: task_t* - pointer to a task or NULL on error.
	ARGUMENTS: 
		@task_t - a time for the task to run with precision in seconds.
		@task_action_t - pointer to a function to execute.
		@void* - optional parameter to be passed to task function.
		@task_cleanup_t - pointer to a function to execute.
		@void* - optional parameter to be passed to cleanup function.
		@unsigned long - optional interval for when to re-execute the task.
	LIMITATIONS: (none)
	NOTES: (none)
	COMPLEXITY: 
			Time: O(1)
*/
task_t* TaskCreate(time_t execution_time, task_action_t action, void* action_param, task_cleanup_t cleanup, void* cleanup_param, unsigned long interval);

/*
	DESCRIPTION: Destroy a task created by TaskCreate, execute cleanup function.
	RETURNS: (void)
	ARGUMENTS: 
			@task_t* - pointer to a task.
	LIMITATIONS: undefined behavior when task is NULL.
	NOTES: (none)
	COMPLEXITY: 
			Time: O(1)
*/
void TaskDestroy(task_t* task);

/*
	DESCRIPTION: Get the task unique Id
	RETURNS: uid_t - task id
	ARGUMENTS: 
			@task_t* - pointer to a task.
	LIMITATIONS: (none)
	NOTES: (none)
	COMPLEXITY: 
			Time: O(1)
*/
ilrd_uid_t TaskGetId(task_t* task);

/*
	DESCRIPTION: Getting the time of the task
	RETURNS: time_t - the time desired the task to execute
	ARGUMENTS: 
			@task_t* - pointer to a task created by TaskCreate.
	LIMITATIONS: None
	NOTES: (none)
	COMPLEXITY: 
			Time: O(1)
*/
time_t TaskGetTime(task_t* task);

/*
	DESCRIPTION: Execute a task
	RETURNS: 1 to reschedule, 0 to not reschedule
	ARGUMENTS: 
			@task_t* - pointer to the task we want to execute.
	LIMITATIONS: (none)
	NOTES: (none)
	COMPLEXITY: 
			Time: O(f) where f is the complexity of the task's function which was specified with TaskCreate.
*/
int TaskExec(task_t* task);

/*
	DESCRIPTION: Update execution_time for the task
	RETURNS: 0 on SUCCESS, 1 on FAILURE
	ARGUMENTS: 
			@task_t* - pointer to the task we want to update its time.
	LIMITATIONS: (none)
	NOTES: (none)
	COMPLEXITY: 
			Time: O(1)
*/
int TaskUpdateTimeToRun(task_t* task);

#endif /* __ILRD_TASK__ */
