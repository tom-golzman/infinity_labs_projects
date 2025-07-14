/**
	Written By: Tom Golzman
	Date: 24/04/2025
	Reviewed By: Amir Granot
**/

/************************************includes************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "task.h"

/************************************define************************************/
#define SUCCESS (0)
#define FAIL (1)

/************************************typedef************************************/
struct task {
	ilrd_uid_t id;
	time_t execution_time;
	unsigned long every_interval;
	void* action_params;
	void* cleanup_params;
	task_action_t action;
	task_cleanup_t cleanup;
};

/************************************Functions************************************/
task_t* TaskCreate(time_t execution_time, task_action_t action, void* action_param, task_cleanup_t cleanup, void* cleanup_param, unsigned long interval)
{
	ilrd_uid_t uid;
	task_t* task = NULL;
	
	assert(NULL != action);
	
	uid = UIDCreate();
	if (UIDIsSame(uid, invalid_uid))
	{
		return (NULL);
	}
	
	task = (task_t*)malloc(sizeof(task_t));
	if (NULL == task)
	{
		return (NULL);
	}
	
	task->id = uid;
	task->execution_time = execution_time;
	task->every_interval = interval;
	task->action_params = action_param;
	task->cleanup_params = cleanup_param;
	task->action = action;
	task->cleanup = cleanup;
	
	return (task);
}

void TaskDestroy(task_t* task)
{
	if (NULL == task)
	{
		return;
	}
	
	task->cleanup(task->cleanup_params);
	
	free(task);
	task = NULL;
}

ilrd_uid_t TaskGetId(task_t* task)
{
	assert(NULL != task);
	
	return (task->id);
}

time_t TaskGetTime(task_t* task)
{
	assert(NULL != task);
	
	return (task->execution_time);
}

int TaskExec(task_t* task)
{
	assert(NULL != task);
	
	return (task->action(task->action_params));
}

int TaskUpdateTimeToRun(task_t* task)
{
	time_t curr_time = time(NULL);
	
	assert(NULL != task);
	
	if ((time_t)-1 == curr_time)
	{
		return (FAIL);
	}
	
	task->execution_time = curr_time + task->every_interval;

	return (SUCCESS);
}
