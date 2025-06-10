/**
	Written By: Tom Golzman
	Date: 24/04/2025
	Reviewed By: Amir Granot
**/

/************************************includes************************************/
#include "priority_queue.h" /* pq functions */
#include "task.h" 			/* task_t */
#include "scheduler.h"

/************************************define************************************/
#define TRUE (1)
#define FALSE (0)
#define SUCCESS (0)
#define FAIL (1)

/************************************typedef************************************/
typedef enum {
	STOPPED = 0,
	RUNNING
} state_t;

struct scheduler{
	state_t state;
	priority_queue_t* tasks;
	int action_was_self_remove;
	task_t* current_task;
};

/************************************Private Functions Forward Decleration************************************/
static int SleepUntilExecTime(task_t* task);
static int TaskComparer(const void* task1, const void* task2, void* param);
static int TaskIsMatch(const void* task, const void* id);
static int SchedRescheduleTask(sched_t* s);

/************************************Functions************************************/
sched_t* SchedCreate(void)
{
	priority_queue_t* pq = NULL;
	
	sched_t* sched = (sched_t*)malloc(sizeof(*sched));
	if (NULL == sched)
	{
		return (NULL);
	}
	
	pq = PQCreate(TaskComparer, NULL);
	if (NULL == pq)
	{
		SchedDestroy(sched);
		sched = NULL;
		
		return (NULL);
	}
	
	sched->state = STOPPED;
	sched->tasks = pq;
	sched->action_was_self_remove = FALSE;
	sched->current_task = NULL;
	
	return (sched);
}

void SchedDestroy(sched_t* scheduler)
{
	assert(RUNNING != scheduler->state);
		
	SchedClear(scheduler);
	PQDestroy(scheduler->tasks);
	
	free(scheduler);
	scheduler = NULL;
}

ilrd_uid_t SchedAddTask(sched_t* scheduler, time_t execution_time, sched_action_t action, void* action_param, sched_cleanup_t cleanup, void* cleanup_param, unsigned long interval)
{
	int enqueue_status = FAIL;
	task_t* task = NULL;
	
	assert(NULL != scheduler);
	assert(NULL != action);
	assert(NULL != cleanup);
	
	task = TaskCreate(execution_time, action, action_param, cleanup, cleanup_param, interval);
	if (NULL == task)
	{
		return (invalid_uid);
	}
	
	enqueue_status = PQEnqueue(scheduler->tasks, task);
	if (FAIL == enqueue_status)
	{
		TaskDestroy(task);
		task = NULL;
		
		return (invalid_uid);
	}
	
	return (TaskGetId(task));
}

void SchedRemoveTask(sched_t* s, ilrd_uid_t task_id)
{
	task_t* task = NULL;
	
	assert(NULL != s);
	
	if (NULL != s->current_task && UIDIsSame(task_id, TaskGetId(s->current_task)))
	{
		s->action_was_self_remove = TRUE;
		
		return;
	}	

	task = PQErase(s->tasks, TaskIsMatch, &task_id);
	if (NULL == task)
	{
		return;
	}

	TaskDestroy(task);
	task = NULL;
}

int SchedIsEmpty(const sched_t* s)
{
	assert(NULL != s);

	if (!PQIsEmpty(s->tasks) || NULL != s->current_task)
	{
		return (FALSE);
	}
	
	return (TRUE);
}

size_t SchedSize(const sched_t* s)
{
	assert(NULL != s);

	if (NULL != s->current_task)
	{
		return (PQSize(s->tasks) + 1);
	}
	
	return (PQSize(s->tasks));
}

status_t SchedRun(sched_t* s)
{
	int to_reschedule = FALSE;

	assert(NULL != s);

	if (RUNNING == s->state)
	{
		return (ALREADY_RUNNING);
	}

	s->state = RUNNING;
	
	while (RUNNING == s->state && !SchedIsEmpty(s))
	{
		s->current_task = PQPeek(s->tasks);

		if (FAIL == SleepUntilExecTime(s->current_task))
		{
			s->current_task = NULL;
			s->state = STOPPED;

			return (INTERNAL_ERROR);
		}
		
		PQDequeue(s->tasks);		
		to_reschedule = TaskExec(s->current_task);
		
		if (TRUE == to_reschedule && FALSE == s->action_was_self_remove)
		{
			if (FAIL == SchedRescheduleTask(s))
			{
				TaskDestroy(s->current_task);
				s->current_task = NULL;
				s->state = STOPPED;
				
				return (INTERNAL_ERROR);
			}
		}
		
		else
		{
			TaskDestroy(s->current_task);
			s->current_task = NULL;
		}
		
		s->action_was_self_remove = FALSE;
	}

	if (RUNNING == s->state)
	{		
		s->state = STOPPED;
		
		return (STOPPED_DEFAULT);
	}
	
	return (STOPPED_BY_USER);

}

void SchedStop(sched_t* s)
{
	assert(NULL != s);
	
	s->state = STOPPED;
}

void SchedClear(sched_t* s)
{
	task_t* task = NULL;
	
	assert(NULL != s);
	
	if (RUNNING == s->state)
	{
		s->action_was_self_remove = TRUE;		
	}
	
	while (!PQIsEmpty(s->tasks))
	{
		task = PQPeek(s->tasks);
		PQDequeue(s->tasks);
		TaskDestroy(task);
		task = NULL;
	}
}

/************************************Private Functions************************************/
static int SleepUntilExecTime(task_t* task)
{
	time_t now = time(NULL);
	time_t exec_time = 0;
	time_t sleep_time = 0;
	
	assert (NULL != task);
	
	if ((time_t)-1 == now)
	{
		return (FAIL);
	}
	
	exec_time = TaskGetTime(task);
	
	if (exec_time > now)
	{
		sleep_time = exec_time - now;
		
		while (sleep_time > 0)
		{
			sleep_time = sleep(sleep_time);
		}
	}
	
	return (SUCCESS);
}

static int TaskComparer(const void* task1, const void* task2, void* param)
{
	(void)param;
	
	return (TaskGetTime((task_t*)task1) - TaskGetTime((task_t*)task2));
}

static int TaskIsMatch(const void* task, const void* id)
{
	return (UIDIsSame(TaskGetId((task_t*)task), *(ilrd_uid_t*)id));
}

static int SchedRescheduleTask(sched_t* s)
{
	if (FAIL == TaskUpdateTimeToRun(s->current_task))
	{
		return (FAIL);
	}
	
	return (PQEnqueue(s->tasks, s->current_task));
}
