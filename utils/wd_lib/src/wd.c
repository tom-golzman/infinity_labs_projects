/**
	Written By: Tom Golzman
	Date: 09/07/2025
	Reviewed By: 
**/

/************************************ includes ************************************/
#include <assert.h> /* assert() */

#include "utils.h"	/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

#include "scheduler.h"
#include "wd.h"

/************************************ defines************************************/
enum { MAX_MISSES_IDX = 1, INTERVAL_IDX = 2, WD_PATH_IDX = 0 };

typedef struct watchdog
{
	int argc;
	const char** argv;
	int max_misses;
	unsigned long interval;
	int revive_counter;
	sched_t* scheduler;
	const char* wd_exec_path;
	pid_t other_process_pid; /* wd pid */
} wd_t;

/* struct to pass arguments to thread function */
typedef struct data
{
	int argc;
	const char** argv;
	int max_misses;
	unsigned long interval;
	const char* wd_exec_path;
} mmi_data_t;

static volatile sig_atomic_t g_counter = 1;
static volatile int g_is_dnr_received = FALSE;
static volatile int g_thread_status = FAIL;
static sem_t g_sem;

/******************************** Private Functions ********************************/
static void* ThreadFunc(void* arg);
static void InitMMIDataStruct(mmi_data_t* mmi_data, int argc, const char* argv[], int max_misses, unsigned long interval, const char* wd_exec_path);
static const char** ReCreateArgv(mmi_data_t* mmi_data);
static void InitWDStruct(wd_t* wd, int argc, const char* argv[], int max_misses, unsigned long interval);
static void CleanUp(wd_t* wd);
static void DestroyArgv(int argc, const char* argv[]);

static int SetSignalHandlers();
static void MaskSignals();
static void UnMaskSignals();
static void HandleSIGUSR1(int sig);

static void ChangeThreadStatusToSuccess();
static void ChangeThreadStatusToFail();

/************************************ Functions ************************************/
int MakeMeImmortal(int argc, const char* argv[], int max_misses, unsigned long interval, const char* wd_exec_path)
{
	pthread_t thread_id;
	mmi_data_t mmi_data;
	
	/* assert */
	assert(max_misses > 0);
	assert(interval > 0);
	assert(NULL != wd_exec_path);
	
	/* mask signals */
	MaskSignals();
	
	/* initialize struct to pass arguments to the thread function */
	InitMMIDataStruct(&mmi_data);
	
	/* init semaphore */
	sem_init(*g_sem, 0, 0);
	
	/* create thread */
	pthread_create(&thread_id, NULL, ThreadFunc, &mmi_data);
	
	/* wait for an indication all is good or all is lost */
	
	
	/* return thread_status */
	return thread_status;
}

static void* ThreadFunc(void* arg)
{
	mmi_data_t* mmi_data = (assert(NULL != arg), (mmi_data_t*)arg);
	const char** new_argv = NULL;
	int new_argc = 0;
	int status = 0;
	wd_t wd;
		
	/* recreate argv */
	new_argv = ReCreateArgv(mmi_data);
	
	/* count new argc */
	while (NULL != new_argv[new_argc])
	{
		++new_argc;
	}
	
	/* initialize wd struct fields */
	status = InitWDStruct(&wd, new_argc, new_argv, mmi_data->max_misses, mmi_data->interval);
	RET_IF_BAD_CLEAN(FAIL != status, NULL, "ThreadFunc(): InitWDStruct() FAILED!\n", ChangeThreadStatusToFail());
	
	/* set signal handlers */
	status = SetSignalHandlers();
	RET_IF_BAD_CLEAN(FAIL != status, NULL, "ThreadFunc(): SetSignalHandlers() FAILED!\n", ChangeThreadStatusToFail());
	
	/* unmask signals */
	UnMaskSignals();
	
	/* create wd process */
	status = CreateWDProcess(wd);
	RET_IF_BAD_CLEAN(FAIL != status, NULL, "ThreadFunc(): CreateWDProcess() FAILED!\n", ChangeThreadStatusToFail());
	
	/* init client scheduler */
	status = InitSchedulerClient(&wd);
	RET_IF_BAD_CLEAN(FAIL != status, NULL, "ThreadFunc(): InitSchedulerClient() FAILED!\n", ChangeThreadStatusToFail());
	
	/* run scheduler */
	SchedRun(wd.scheduler);
	
	/* clean up */
	CleanUp(&wd);
	
	return NULL;
}

static void InitMMIDataStruct(mmi_data_t* mmi_data, int argc, const char* argv[], int max_misses, unsigned long interval, const char* wd_exec_path)
{
	assert(NULL != mmi_data);
	assert(NULL != argv);
	assert(NULL != wd_exec_path);
	assert(argc >= 5);
	assert(max_misses > 0);
	assert(interval > 0);
	
	mmi_data->argc = argc;
	mmi_data->argv = argv;
	mmi_data->max_misses = max_misses;
	mmi_data->interval = interval;
	mmi_data->wd_exec_path = wd_exec_path;
}

static const char** ReCreateArgv(mmi_data_t* mmi_data)
{
	char** new_argv = NULL;
	int i = 0;
	char interval_str[20];
	char misses_str[20];
	
	/* assert */
	assert(NULL != mmi_data);

	/* convert max_misses to string */
	snprintf(misses_str, sizeof(misses_str), "%d", mmi_data->max_misses);	

	/* convert interval to string */
	snprintf(interval_str, sizeof(interval_str), "%lu", mmi_data->interval);
	
	/* allocate memory for new_argv */
	new_argv = (char**)calloc(1, (mmi_data->argc + 4) * sizeof(char*));
	ExitIfBad(NULL != new_argv, FAIL, "ReCreateArgv(): calloc() FAILED!\n");
	
	/* initialize in new argv wd_exec_path, max_misses, interval */
	new_argv[WD_PATH_IDX] = strdup(mmi_data->wd_exec_path);
	new_argv[MAX_MISSES_IDX] = strdup(misses_str);
	new_argv[INTERVAL_IDX] = strdup(interval_str);
	new_argv[3] = strdup(mmi_data->argv[0]);
	
	/* add to the new argv the user's argv */
	for (i = 0; i < mmi_data->argc; ++i)
	{
		new_argv[i + 4] = strdup(mmi_data->argv[i]);
	}
	
	/* add NULL at the end of the new argv */
	new_argv[mmi_data->argc + 4] = NULL;
	
	/* return new argv */
	return (const char**)new_argv;
}

static int InitWDStruct(wd_t* wd, int argc, const char* argv[], int max_misses, unsigned long interval)
{
	/* assert */
	assert(NULL != wd);
	
	/* initialize struct fields */
	wd->argc = argc - 4;
	wd->argv = &argv[4];
	wd->max_misses = max_misses;
	wd->interval = interval;
	wd->revive_counter = 0;
	wd->wd_exec_path = argv[WD_PATH_IDX];
	wd->other_process_pid = getpid();
	wd->status = NULL;
	
	wd->scheduler = SchedCreate();
	RET_IF_BAD(NULL != wd->scheduler, FAIL, "InitWDStruct(): SchedCreate() FAILED!\n");
	
	/* return SUCCESS */
	return SUCCESS;
}

static void DestroyArgv(int argc, const char* argv[])
{
	char** curr = NULL;	
	char** end = NULL;
	
	if (NULL == argv)
	{
		return;
	}
	
	end = argv + argc;
	
	for (curr = argv; curr != end; ++curr)
	{
		free(*curr);
	}
	
	free(argv);
	argv = NULL;
}

/******************************** phase 1 (init) functions ********************************/
static int AddInitTasks(wd_t* wd)
{
	time_t now = -1;
	ilrd_uid_t task_uid = invalid_uid;
	
	/* assert */
	assert(NULL != wd);
	assert(NULL != wd->scheduler);
	
	/* take current time */
	now = time(NULL);
	RET_IF_BAD(-1 != now, FAIL, "AddInitTask(): time(NULL) FAILED!\n");
	
	/* add task - increment counter */
	task_uid = SchedAddTask(wd->scheduler, now + wd->interval, WaitForFirstSignalTask, wd, NULL, NULL, wd->interval);
	RET_IF_BAD(!UIDIsSame(task_uid, invalid_uid), FAIL, "AddInitTask(): AddTask(WaitForFirstSignal) FAILED!\n");
	
	/* return SUCCESS */
	return SUCCESS;
}

/******************************** phase 2 (mainstream) functions ********************************/
static int AddMainstreamTasks(wd_t* wd)
{
	time_t now = -1;
	ilrd_uid_t task_uid = invalid_uid;
	
	/* assert */
	assert(NULL != wd);
	
	/* take current time */
	now = time(NULL);
	RET_IF_BAD(-1 != now, FAIL, "AddMainstreamTasks(): time(NULL) FAILED!\n");
	
	/* add task - send signal */
	task_uid = SchedAddTask(wd->scheduler, now + wd->interval, SendSignalClientTask, wd, NULL, NULL, wd->interval);
	RET_IF_BAD(!UIDIsSame(task_uid, invalid_uid), FAIL, "AddMainstreamTasks(): AddTask(SendSignalClient) FAILED!\n");
	
	/* add task - check counter client */
	task_uid = SchedAddTask(wd->scheduler, now + wd->interval, CheckCounterClientTask, wd, NULL, NULL, wd->interval);
	RET_IF_BAD(!UIDIsSame(task_uid, invalid_uid), FAIL, "AddMainstreamTasks(): AddTask(SendSignalClient) FAILED!\n");
	
	/* add task - check DNR */
	task_uid = SchedAddTask(wd->scheduler, now + wd->interval, CheckDNRTask, wd, NULL, NULL, wd->interval);
	RET_IF_BAD(!UIDIsSame(task_uid, invalid_uid), FAIL, "AddMainstreamTasks(): AddTask(CheckDNRTask) FAILED!\n");

	/* return SUCCESS */
	return SUCCESS;
}

/******************************** functions ********************************/
static int InitSchedulerClient(wd_t* wd)
{
	/* assert */
	assert(NULL != wd);
	assert(NULL != wd->scheduler);
	
	return AddInitTask();
}

static int CreateWDProcess(wd_t* wd)
{
	pid_t pid = -1;
	int status = 0;
	
	/* assert */
	assert(NULL != wd);
	assert(NULL != wd->wd_exec_path);
	assert(NULL != wd->argv);
	
	/* fork & handle failure */
	pid = fork();
	RET_IF_BAD(-1 != pid, FAIL, "CreateWDProcess(): fork() FAILED!\n");
	
	/* if parent */
	if (pid > 0)
	{
		/* save wd pid in the struct */
		wd->other_process_pid = pid;
	
		/* return SUCCESS */
		return SUCCESS;
	}
	
	/* if child: */
	if (0 == pid)
	{
		/* execv() & handle failure */
		execv(wd->wd_exec_path, wd->argv);	
	}
	
	/* if execv returns - failed */
	/* log */
	Log("CreateWDProcess(): execv() FAILED!\n");
	
	/* return FAIL */
	return FAIL;
}

static int ReviveWD(wd_t* wd) /* wd_t* */
{
	int status = 0;
	
	/* assert */
	assert(NULL != wd);
	
	/* kill wd process */
	status = kill(wd->other_process_pid, SIGTERM);
	RET_IF_BAD(0 == status, FAIL, "ReviveWD(): kill() FAILED!\n");
	
	/* create new wd process */
	status = CreateWDProcess(wd);
	RET_IF_BAD(0 == status, FAIL, "ReviveWD(): CreateWDProcess() FAILED!\n");
	
	/* reset g_counter */
	g_counter = 0;
	
	/* reset revive counter */
	wd->revive_counter = 0;

	/* clear scheduler */
	SchedClear(wd->scheduler);
	
	/* go back to init phase */
	status = AddInitTasks(wd);
	RET_IF_BAD(FAIL != status, FAIL, "ReviveWD(): AddInitTasks() FAILED!\n");
	
	/* return SUCCESS */
	return SUCCESS;
}

void DNR()
{
	/* change global flag of DNR to TRUE */
}

static void CleanUp(wd_t* wd)
{
	/* assert */
	assert(NULL != wd);
	
	/* destroy scheduler */
	SchedDestroy(wd->scheduler);
	
	/* destroy new argv */
	DestroyArgv();
}

static void ChangeThreadStatusToSuccess()
{
	g_thread_status = SUCCESS;
}

static void ChangeThreadStatusToFail()
{
	g_thread_status = FAIL;
}

/******************************** tasks ********************************/
static int WaitFirstSignalTask(void* arg)
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);
	int status = 0;
	
	/* if g_counter is zero */
	if (0 == g_counter)
	{
		/* change thread status to SUCCESS */
		ChangeThreadStatusToSuccess();
		
		/* post the semaphore to indicate MMI() that first signal was received */
		sem_post(&g_sem);
		
		/* reset the revive counter in the struct */
		wd->revive_counter = 0;
		
		/* move to mainstream phase: */
		/* add mainstream tasks */
		status = AddMainstreamTasks(wd);
		
		/* if AddMainstreamTasks() failed */
		if (FAIL == status)
		{
			/* print to log */
			Log("WaitFirstSignalTask(): AddMainstreamTasks() FAILED!\n");

			/* stop scheduler */
			SchedStop(wd->scheduler);		
		}
		
		/* return NOT_RESCHEDULE - phase 1 finished */
		return NOT_RESCHEDULE;
	}
	
	/* increase counter */
	++g_counter;
	
	/* if g_counter is above max_misses */
	if (g_counter > wd->max_misses)
	{
		/* print to log */
		Log("WaitFirstSignal(): WD didn't send first signal - exiting\n");
		
		/* stop scheduler */
		SchedStop(wd->scheduler);
		
		/* return NOT_RESCHEDULE */
		return NOT_RESCHEDULE;
	}
	
	/* return TO_RESCHEDULE */
	return TO_RESCHEDULE;	
}

static int SendSignalClientTask(wd_t* wd)
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);
	int status = FAIL;
	
	/* send signal to client */
	status = kill(wd->other_process_pid, SIGUSR1);
	LOG_IF_BAD(0 == status, "SendSignalClientTask(): kill() FAILED!\n");
	
	/* return TO_RESCHEDULE */
	return TO_RESCHEDULE;
}

static int CheckCounterClientTask(void* arg) /* wd_t* */
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);
	
	/* if g_counter is zero */
	if (0 == g_counter)
	{
		/* reset revive counter */
		wd->revive_counter = 0;
		
		/* return TO_RESCHEDULE */
		return TO_RESCHEDULE;
	}
	
	/* increse g_counter */
	++g_counter;
	
	/* if g_counter is above max_misses */
	if (g_couner > wd->max_misses)
	{
		/* print to log */
		Log("CheckCounterClient(): WD did not respond - exiting\n");
		
		/* stop scheduler */
		SchedStop(wd->scheduler);
		
		/* return NOT_RESCHEDULE */
		return NOT_RESCHEDULE;
	}
	
	/* return TO_RESCHEDULE */
	return TO_RESCHEDULE;
}

static int CheckDNRTask(void* arg)
{
	/* if received dnr */
		
		/* stop scheduler and cleanup */
		
		/* log */
		
		/* return NOT_RESCHEDULE */
		
	/* return TO_RESCHEDULE */
}

/************************************ Signal Handler ************************************/
static int SetSignalHandlers()
{
	struct sigaction sa;
	int status = 0;
	
	memset(&sa, 0, sizeof(sa));

	/* set handlers with sigaction */
	sa.sa_handler = HandleSIGUSR1;
	
	status = sigaction(SIGUSR1, &sa, NULL);
	
	RET_IF_BAD(0 == status, FAIL, "SetSignalHandlers(): sigaction() FAILED!\n");

	return SUCCESS;
}

static void MaskSignals()
{
	sigset_t mask;
	
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);
	sigprocmask(SIG_BLOCK, &mask, NULL);
}

static void UnMaskSignals()
{
	sigset_t mask;
	
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);
}

static void HandleSIGUSR1(int sig)
{
	/* reset g_counter */
	g_counter = 0;
}
