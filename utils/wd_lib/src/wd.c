/**
	Written By: Tom Golzman
	Date: 09/07/2025
	Reviewed By: 
**/

/************************************ includes ************************************/
#define _POSIX_C_SOURCE 200112L
#include <signal.h>		/* sigaction(), sig_atomic_t */
#include <unistd.h>		/* getpid(), execv(), kill() */
#include <assert.h>		/* assert */
#include <time.h>		/* time() */
#include <string.h>		/* memset() */
#include <semaphore.h>	/* sem_t, sem_init() */
#include <pthread.h>	/* pthread_t, pthread_create() */
#include <errno.h>		/* errno */
#include <sys/wait.h>	/* waitpid() */
#include "utils.h"		/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

#include "scheduler.h"
#include "wd.h"

/************************************ defines************************************/
enum { WD_PATH_IDX = 0, MAX_MISSES_IDX = 1, INTERVAL_IDX = 2, PID_IDX = 3, NUM_ADDED_FIELDS = 4, ARGV_BUFF_SIZE = 2000 };

typedef struct watchdog
{
	int argc;
	char** new_argv;
	int max_misses;
	unsigned long interval;
	sched_t* scheduler;
	const char* wd_exec_path;
	pid_t other_process_pid; /* wd pid */
	volatile int* is_first_signal_received;
	sem_t* sem;
} wd_t;

/* struct to pass arguments to thread function */
typedef struct data
{
	int argc;
	char** original_argv;
	int max_misses;
	unsigned long interval;
	const char* wd_exec_path;
	volatile int* is_first_signal_received;
	sem_t* sem;
} mmi_data_t;

static volatile sig_atomic_t g_counter = 2;
static volatile int g_is_dnr_received = FALSE;
static pthread_t g_thread;
/******************************** Private Functions ********************************/
static void* ThreadFunc(void* arg);
static char** ReCreateArgv(int argc, char* original_argv[], int max_misses, unsigned long interval, const char* wd_exec_path);
static int SemWaitWithTimeout(sem_t* sem, int timeout_sec);

static int InitMMIDataStruct(mmi_data_t* mmi_data, int argc, char* argv[], int max_misses, unsigned long interval, const char* wd_exec_path, volatile int* is_first_signal_received, sem_t* sem);
static int InitWDStruct(wd_t* wd, int argc, char* argv[], int max_misses, unsigned long interval, volatile int* is_first_signal_received, sem_t* sem);

static int AddInitialTasks(wd_t* wd);
static int AddMainstreamTasks(wd_t* wd);
static int AddRevivalTasks(wd_t* wd);

static int InitSchedulerClient(wd_t* wd);
static int CreateWDProcess(char* new_argv[], const char* wd_exec_path);
static int ReviveWD(wd_t* wd);

static void DestroyArgv(char* argv[]);
static void DestroySemaphore(sem_t* sem);
static void DestroyMMI(mmi_data_t* mmi_data);
static void CleanUpThread(sched_t* scheduler, char* new_argv[]);
static void CleanupAndUpdateFlag(char* argv[], volatile int* is_first_signal_received, int status);

static int WaitFirstSignalTask(void* arg);
static int WaitForConnectionTask(void* arg);
static int SendSignalClientTask(void* arg);
static int CheckCounterClientTask(void* arg);
static int CheckDNRTask(void* arg);

static int SetSignalHandlers();
static int MaskSignals();
static int UnMaskSignals();
static void HandleSIGUSR1(int sig);

/************************************ Functions ************************************/
int MakeMeImmortal(int argc, char* argv[], int max_misses, unsigned long interval, const char* wd_exec_path)
{
	mmi_data_t* mmi_data = NULL;
	int status = 0;
	volatile int is_first_signal_received = FALSE;
	int sem_timeout_sec = max_misses * interval;
	sem_t sem = {0};
	
	/* assert */
	assert(max_misses > 0);
	assert(interval > 0);
	assert(NULL != wd_exec_path);
	
	/* mask signals */
	status = MaskSignals();
	RET_IF_BAD(FAIL != status, FAIL, "wd.c-> MakeMeImmortal(): MaskSignals() FAILED!\n");
	
	/* allocate memory for mmi_data struct */
	mmi_data = (mmi_data_t*)calloc(1, sizeof(mmi_data_t));
	RET_IF_BAD(NULL != mmi_data, FAIL, "wd.c-> MakeMeImmortal(): calloc(mmi_data) FAILED!\n");
	
	/* init semaphore */
	status = sem_init(&sem, 0, 0);
	RET_IF_BAD_SC(status, FAIL, "wd.c-> MakeMeImmortal(): sem_init() FAILED!\n");
	
	/* initialize struct to pass arguments to the thread function */
	status = InitMMIDataStruct(mmi_data, argc, argv, max_misses, interval, wd_exec_path, &is_first_signal_received, &sem);
	RET_IF_BAD_CLEAN(FAIL != status, FAIL, "wd.c-> MakeMeImmortal(): InitMMIDataStruct() FAILED!\n", DestroyMMI(mmi_data));
	
	/* create thread */
	status = pthread_create(&g_thread, NULL, ThreadFunc, mmi_data);
	RET_IF_BAD_SC_CLEAN(status, FAIL, "wd.c-> MakeMeImmortal(): pthread_create() FAILED!\n", DestroyMMI(mmi_data));
	
	/* wait for semaphore */
	status = SemWaitWithTimeout(&sem, sem_timeout_sec);
	RET_IF_BAD_CLEAN(FAIL != status, FAIL, "wd.c-> MakeMeImmortal(): sem_wait() FAILED!\n", pthread_join(g_thread, NULL););
	
	/* destroy semaphore */
	DestroySemaphore(&sem);
	
	/* return thread status */
	if (is_first_signal_received)
	{
		/* return SUCCESS */
		return SUCCESS;
	}
	
	/* return FAIL */
	return FAIL;
}

int DNR()
{
	/* change global flag of DNR to TRUE */
	__atomic_store_n(&g_is_dnr_received, TRUE, __ATOMIC_SEQ_CST);
	
	Log("wd.c-> DNR(): DNR received\n");
	
	RET_IF_BAD_SC(pthread_join(g_thread, NULL), FAIL, "wd.c-> DNR(): pthread_join() FAILED!\n");
	
	return SUCCESS;
} 

static void* ThreadFunc(void* arg)
{
	mmi_data_t* mmi_data = (assert(NULL != arg), (mmi_data_t*)arg);
	char** new_argv = NULL;
	int new_argc = 0;
	int status = 0;
	wd_t wd;
	
	/* recreate argv */
	new_argv = ReCreateArgv(mmi_data->argc, mmi_data->original_argv, mmi_data->max_misses, mmi_data->interval, mmi_data->wd_exec_path);
	RET_IF_BAD_CLEAN(NULL != new_argv, NULL, "wd.c-> ThreadFunc(): ReCreateArgv() FAILED!\n", DestroyArgv(new_argv));
	
	/* count new argc */
	while (NULL != new_argv[new_argc])
	{
		++new_argc;
	}
	
	/* initialize wd struct fields */
	status = InitWDStruct(&wd, new_argc, new_argv, mmi_data->max_misses, mmi_data->interval, mmi_data->is_first_signal_received, mmi_data->sem);
	RET_IF_BAD_CLEAN(FAIL != status, NULL, "wd.c-> ThreadFunc(): InitWDStruct() FAILED!\n", CleanupAndUpdateFlag(new_argv, mmi_data->is_first_signal_received, FALSE));

	/* set signal handlers */
	status = SetSignalHandlers();
	RET_IF_BAD_CLEAN(FAIL != status, NULL, "wd.c-> ThreadFunc(): SetSignalHandlers() FAILED!\n", CleanupAndUpdateFlag(new_argv, mmi_data->is_first_signal_received, FALSE));

	/* unmask signals */
	status = UnMaskSignals();
	RET_IF_BAD_CLEAN(FAIL != status, NULL, "wd.c-> ThreadFunc(): UnMaskSignals() FAILED!\n", CleanupAndUpdateFlag(new_argv, mmi_data->is_first_signal_received, FALSE));

	/* create wd process & assign wd pid in the wd struct*/
	wd.other_process_pid = CreateWDProcess(new_argv, new_argv[0]);
	RET_IF_BAD_CLEAN(FAIL != wd.other_process_pid, NULL, "wd.c-> ThreadFunc(): CreateWDProcess() FAILED!\n", CleanupAndUpdateFlag(new_argv, mmi_data->is_first_signal_received, FALSE));
	
	/* init client scheduler */
	status = InitSchedulerClient(&wd);
	RET_IF_BAD_CLEAN(FAIL != status, NULL, "wd.c-> ThreadFunc(): InitSchedulerClient() FAILED!\n", CleanupAndUpdateFlag(new_argv, mmi_data->is_first_signal_received, FALSE));
	
	/* run scheduler */
	SchedRun(wd.scheduler);
	
	/* clean up */
	CleanUpThread(wd.scheduler, new_argv);
	
	return NULL;
}

static int InitMMIDataStruct(mmi_data_t* mmi_data, int argc, char* argv[], int max_misses, unsigned long interval, const char* wd_exec_path, volatile int* is_first_signal_received, sem_t* sem)
{
	assert(NULL != mmi_data);
	assert(NULL != argv);
	assert(NULL != wd_exec_path);
	assert(max_misses > 0);
	assert(interval > 0);
	
	mmi_data->argc = argc;
	mmi_data->original_argv = argv;
	mmi_data->max_misses = max_misses;
	mmi_data->interval = interval;
	mmi_data->wd_exec_path = wd_exec_path;
	mmi_data->is_first_signal_received = is_first_signal_received;
	mmi_data->sem = sem;
	
	return SUCCESS;
}

static char** ReCreateArgv(int argc, char* original_argv[], int max_misses, unsigned long interval, const char* wd_exec_path)
{
	char** new_argv = NULL;
	char interval_str[ARGV_BUFF_SIZE];
	char misses_str[ARGV_BUFF_SIZE];
	char curr_pid_str[ARGV_BUFF_SIZE];
	int sprintf_status = -1;
	
	/* assert */
	assert(NULL != original_argv);
	assert(NULL != wd_exec_path);
	
	/* convert max_misses to string */
	sprintf_status = snprintf(misses_str, sizeof(misses_str), "%d", max_misses);	
	RET_IF_BAD(sprintf_status >= 0, NULL, "wd.c-> ReCreateArgv(): snprintf(misses_str) FAILED!\n");
	
	/* convert interval to string */
	sprintf_status = snprintf(interval_str, sizeof(interval_str), "%lu", interval);
	RET_IF_BAD(sprintf_status >= 0, NULL, "wd.c-> ReCreateArgv(): snprintf(interval_str) FAILED!\n");	
	
	/* convert pid to string */
	sprintf_status = snprintf(curr_pid_str, sizeof(curr_pid_str), "%d", getpid());
	RET_IF_BAD(sprintf_status >= 0, NULL, "wd.c-> ReCreateArgv(): snprintf(curr_pid_str) FAILED!\n");
	
	/* allocate memory for new_argv */
	new_argv = (char**)calloc(1, (argc + NUM_ADDED_FIELDS + 1) * sizeof(char*)); /* +1 for the NULL at the end */
	RET_IF_BAD(NULL != new_argv, NULL, "wd.c-> ReCreateArgv(): calloc() FAILED!\n");
	
	/* initialize in new argv wd_exec_path, max_misses, interval and pid */
	new_argv[WD_PATH_IDX] = StrDup(wd_exec_path);
	RET_IF_BAD(NULL != new_argv[WD_PATH_IDX], NULL, "wd.c-> ReCreateArgv(): StrDup(wd_exec_path) FAILED!\n");

	new_argv[MAX_MISSES_IDX] = StrDup(misses_str);
	RET_IF_BAD(NULL != new_argv[MAX_MISSES_IDX], NULL, "wd.c-> ReCreateArgv(): StrDup(misses_str) FAILED!\n");

	new_argv[INTERVAL_IDX] = StrDup(interval_str);
	RET_IF_BAD(NULL != new_argv[INTERVAL_IDX], NULL, "wd.c-> ReCreateArgv(): StrDup(interval_str) FAILED!\n");

	new_argv[PID_IDX] = StrDup(curr_pid_str);
	RET_IF_BAD(NULL != new_argv[PID_IDX], NULL, "wd.c-> ReCreateArgv(): StrDup(curr_pid_str) FAILED!\n");

	/* copy the original argv to new argv */
	memcpy(&new_argv[NUM_ADDED_FIELDS], original_argv, argc * sizeof(char*));

	/* add NULL at the end of the new argv */
	new_argv[argc + NUM_ADDED_FIELDS] = NULL;
	
	/* return new argv */
	return new_argv;
}

static int InitWDStruct(wd_t* wd, int argc, char* argv[], int max_misses, unsigned long interval, volatile int* is_first_signal_received, sem_t* sem)
{
	/* assert */
	assert(NULL != wd);
	
	/* initialize struct fields */
	wd->argc = argc;
	wd->new_argv = argv;
	wd->max_misses = max_misses;
	wd->interval = interval;
	wd->wd_exec_path = argv[WD_PATH_IDX];
	wd->other_process_pid = getpid();
	wd->is_first_signal_received = is_first_signal_received;
	wd->sem = sem;

	wd->scheduler = SchedCreate();
	RET_IF_BAD(NULL != wd->scheduler, FAIL, "wd.c-> InitWDStruct(): SchedCreate() FAILED!\n");
	
	/* return SUCCESS */
	return SUCCESS;
}

/******************************** phase 1 (init) functions ********************************/
static int AddInitialTasks(wd_t* wd)
{
	time_t now = -1;
	ilrd_uid_t task_uid = invalid_uid;
	
	/* assert */
	assert(NULL != wd);
	assert(NULL != wd->scheduler);

	/* take current time */
	now = time(NULL);
	RET_IF_BAD(-1 != now, FAIL, "wd.c-> AddInitTask(): time(NULL) FAILED!\n");

	/* add task - increment counter */
	task_uid = SchedAddTask(wd->scheduler, now, WaitFirstSignalTask, wd, NULL, NULL, wd->interval);
	RET_IF_BAD(!UIDIsSame(task_uid, invalid_uid), FAIL, "wd.c-> AddInitTask(): AddTask(WaitFirstSignalTask) FAILED!\n");
	
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
	RET_IF_BAD(-1 != now, FAIL, "wd.c-> AddMainstreamTasks(): time(NULL) FAILED!\n");

	/* add task - check DNR */
	task_uid = SchedAddTask(wd->scheduler, now, CheckDNRTask, wd, NULL, NULL, wd->interval);
	RET_IF_BAD(!UIDIsSame(task_uid, invalid_uid), FAIL, "wd.c-> AddMainstreamTasks(): AddTask(CheckDNRTask) FAILED!\n");
	
	/* add task - send signal */
	task_uid = SchedAddTask(wd->scheduler, now, SendSignalClientTask, wd, NULL, NULL, wd->interval);
	RET_IF_BAD(!UIDIsSame(task_uid, invalid_uid), FAIL, "wd.c-> AddMainstreamTasks(): AddTask(SendSignalClient) FAILED!\n");

	/* add task - check counter client */
	task_uid = SchedAddTask(wd->scheduler, now, CheckCounterClientTask, wd, NULL, NULL, wd->interval);
	RET_IF_BAD(!UIDIsSame(task_uid, invalid_uid), FAIL, "wd.c-> AddMainstreamTasks(): AddTask(SendSignalClient) FAILED!\n");

	/* return SUCCESS */
	return SUCCESS;
}

/******************************** phase 3 (revival) functions ********************************/
static int AddRevivalTasks(wd_t* wd)
{
	time_t now = -1;
	ilrd_uid_t task_uid = invalid_uid;
	
	/* assert */
	assert(NULL != wd);
	
	/* take current time */
	now = time(NULL);
	RET_IF_BAD(-1 != now, FAIL, "wd.c-> AddRevivalTasks(): time(NULL) FAILED!\n");
	
	/* add task - send signal */
	task_uid = SchedAddTask(wd->scheduler, now, WaitForConnectionTask, wd, NULL, NULL, wd->interval);
	RET_IF_BAD(!UIDIsSame(task_uid, invalid_uid), FAIL, "wd.c-> AddRevivalTasks(): AddTask(WaitForConnectionTask) FAILED!\n");

	/* return SUCCESS */
	return SUCCESS;
}

/******************************** functions ********************************/
static int InitSchedulerClient(wd_t* wd)
{
	/* assert */
	assert(NULL != wd);
	assert(NULL != wd->scheduler);
	
	return AddInitialTasks(wd);
}

static int CreateWDProcess(char* new_argv[], const char* wd_exec_path)
{
	pid_t pid = -1;
	
	/* assert */
	assert(NULL != new_argv);
	assert(NULL != wd_exec_path);
	
	/* fork & handle failure */
	pid = fork();
	RET_IF_BAD(-1 != pid, FAIL, "wd.c-> CreateWDProcess(): fork() FAILED!\n");
	
	/* if parent */
	if (pid > 0)
	{
		/* return wd pid */
		return pid;
	}
	
	/* if child */
	if (0 == pid)
	{
		/* execv() & handle failure */
		execv(wd_exec_path, new_argv);	
		_exit(1);
	}

	/* return FAIL */
	return FAIL;
}

static int ReviveWD(wd_t* wd)
{
	int status = 0;
	
	/* assert */
	assert(NULL != wd);
	
	/* kill wd process */
	status = kill(wd->other_process_pid, SIGTERM);
	RET_IF_BAD_SC(status, FAIL, "wd.c-> ReviveWD(): kill() FAILED!\n");
	
	/* reset g_counter */
	g_counter = 2;

	status = waitpid(wd->other_process_pid, NULL, WNOHANG);
	
	/* if waitpid() returns ECHILD - its success */
	if (-1 == status)
	{
		RET_IF_BAD(ECHILD != errno, FAIL, "wd.c-> ReviveWD(): waitpid() FAILED!\n");
	}
	
	/* create new wd process & assign wd pid in the wd struct */
	wd->other_process_pid = CreateWDProcess(wd->new_argv, wd->wd_exec_path);
	RET_IF_BAD(FAIL != wd->other_process_pid, FAIL, "wd.c-> ReviveWD(): CreateWDProcess() FAILED!\n");
		
	/* clear scheduler */
	SchedClear(wd->scheduler);
	
	/* go to revival phase */
	status = AddRevivalTasks(wd);
	RET_IF_BAD(FAIL != status, FAIL, "wd.c-> ReviveWD(): AddRevivalTasks() FAILED!\n");
	
	/* return SUCCESS */
	return SUCCESS;
}

static int SemWaitWithTimeout(sem_t* sem, int timeout_sec)
{
	struct timespec ts = { 0 };
	int status = 0;
	
	/* assign current time into ts struct */
	clock_gettime(CLOCK_REALTIME, &ts);
	
	/* add to wait time the timeout */
	ts.tv_sec += timeout_sec;
	
	/* wait the timeout */
	status = sem_timedwait(sem, &ts);
	
	/* handle failure */
	if (-1 == status)
	{
		/* if reached timeout */
		RET_IF_BAD_CLEAN(ETIMEDOUT == errno, FAIL, "wd.c-> SemWaitWithTimeout(): reached timeout\n", DestroySemaphore(sem));
		
		/* failed for other reason */
		Log("wd.c-> SemWaitWithTimeout(): sem_timedwait() FAILED!\n");
		
		/* destroy semaphore */
		DestroySemaphore(sem);
		
		/* return FAIL */
		return FAIL;
	}
	
	/* return SUCCESS */
	return SUCCESS;
}

/******************************** Cleanup Functions ********************************/
static void CleanupAndUpdateFlag(char* argv[], volatile int* is_first_signal_received, int status)
{
	DestroyArgv(argv);
	
	assert(NULL != is_first_signal_received);
	
	*is_first_signal_received = status;
}

static void CleanUpThread(sched_t* scheduler, char* new_argv[])
{
	/* destroy scheduler */
	SchedDestroy(scheduler);
	
	/* destroy new argv */
	DestroyArgv(new_argv);
}

static void DestroyArgv(char* argv[])
{
	char** curr = NULL;	
	char** end = NULL;
	
	if (NULL == argv)
	{
		return;
	}
	
	end = argv + NUM_ADDED_FIELDS;
	
	for (curr = argv; curr != end; ++curr)
	{
		free(*curr);
		*curr = NULL;
	}
	
	free(argv);
	argv = NULL;
}

static void DestroySemaphore(sem_t* sem)
{
	if (NULL == sem)
	{
		return;
	}
	
	sem_destroy(sem);
}

static void DestroyMMI(mmi_data_t* mmi_data)
{
	if (NULL == mmi_data)
	{
		return;
	}

	DestroySemaphore(mmi_data->sem);
	
	DEBUG_ONLY(
		mmi_data->argc = -1;
		mmi_data->original_argv = BAD_MEM(char**);
		mmi_data->max_misses = -1;
		mmi_data->interval = 0;
		mmi_data->is_first_signal_received = BAD_MEM(int*);
		mmi_data->sem = BAD_MEM(sem_t*);
	);
	
	free(mmi_data);
	mmi_data = NULL;
}

/******************************** tasks ********************************/
static int WaitFirstSignalTask(void* arg)
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);
	int status = 0;
	
	/* if g_counter is zero */
	if (0 == g_counter || 1 == g_counter)
	{
		/* update flag that was received first siganl */
		*(wd->is_first_signal_received) = TRUE;
				
		/* post the semaphore to indicate MMI() that first signal was received */
		status = sem_post(wd->sem);
		RET_IF_BAD_SC(status, FAIL, "wd.c-> WaitFirstSignalTask(): sem_post() FAILED!\n");
		
		/* move to mainstream phase: */
		/* add mainstream tasks */
		status = AddMainstreamTasks(wd);
		
		/* if AddMainstreamTasks() failed */
		if (FAIL == status)
		{
			/* print to log */
			Log("wd.c-> WaitFirstSignalTask(): AddMainstreamTasks() FAILED!\n");

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
		/* write to log */
		Log("wd.c-> WaitFirstSignalTask(): WD didn't send first signal - exiting\n");
		
		/* stop scheduler */
		SchedStop(wd->scheduler);
		
		/* return NOT_RESCHEDULE */
		return NOT_RESCHEDULE;
	}
	
	/* return TO_RESCHEDULE */
	return TO_RESCHEDULE;	
}

static int WaitForConnectionTask(void* arg)
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);
	int status = 0;
	
	/* if g_counter is zero */
	if (0 == g_counter || 1 == g_counter)
	{
		/* move to mainstream phase: */
		/* add mainstream tasks */
		status = AddMainstreamTasks(wd);
		
		/* if AddMainstreamTasks() failed */
		if (FAIL == status)
		{
			/* print to log */
			Log("wd.c-> WaitForConnectionTask(): AddMainstreamTasks() FAILED!\n");

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
		/* write to log */
		Log("wd.c-> WaitForConnectionTask(): WD didn't send first signal - exiting\n");
		
		/* stop scheduler */
		SchedStop(wd->scheduler);
		
		/* return NOT_RESCHEDULE */
		return NOT_RESCHEDULE;
	}
	
	/* return TO_RESCHEDULE */
	return TO_RESCHEDULE;	
}

static int SendSignalClientTask(void* arg)
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);
	int status = FAIL;

	/* send signal to client */
	status = kill(wd->other_process_pid, SIGUSR1);
	LogIfBad(0 == status, "wd.c-> SendSignalClientTask(): kill() FAILED!\n");
	
	Log("Client sent signal to %d (pid)\n", wd->other_process_pid);
	
	/* return TO_RESCHEDULE */
	return TO_RESCHEDULE;
}

static int CheckCounterClientTask(void* arg)
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);

	/* increse g_counter */
	++g_counter;

	/* if g_counter is above max_misses */
	if (g_counter > wd->max_misses)
	{
		/* write to log */
		Log("wd.c-> CheckCounterClientTask(): WD didn't respond - exiting\n");

		/* stop scheduler */
		SchedStop(wd->scheduler);
					
		/* call revive client */
		ReviveWD(wd);
		
		/* return NOT_RESCHEDULE */
		return NOT_RESCHEDULE;
	}
	
	/* return TO_RESCHEDULE */
	return TO_RESCHEDULE;
}

static int CheckDNRTask(void* arg)
{
	wd_t* wd = (assert(NULL != arg), (wd_t*)arg);
	int status = -1;
	
	/* if received dnr */
	if (__atomic_load_n(&g_is_dnr_received, __ATOMIC_SEQ_CST))
	{
		/* send signal to client */
		status = kill(wd->other_process_pid, SIGKILL);
		LogIfBad(0 == status, "wd.c-> CheckDNRTask(): kill() FAILED!\n");
		
		status = waitpid(wd->other_process_pid, NULL, WNOHANG);
		
		/* if waitpid() returns ECHILD - its success */
		if (-1 == status)
		{
			RET_IF_BAD(ECHILD != errno, FAIL, "wd.c-> CheckDNRTask(): waitpid() FAILED!\n");
		}
		
		/* stop scheduler */
		SchedStop(wd->scheduler);
		
		/* print to log */
		Log("wd.c-> CheckDNRTask(): wd process dead and scheduler stopped\n");
		
		/* return NOT_RESCHEDULE */
		return NOT_RESCHEDULE;
	}
	
	/* return TO_RESCHEDULE */
	return TO_RESCHEDULE;
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
	
	RET_IF_BAD_SC(status, FAIL, "wd.c-> SetSignalHandlers(): sigaction() FAILED!\n");

	return SUCCESS;
}

static int MaskSignals()
{
	int status = -1;
	sigset_t mask;
	
	status = sigemptyset(&mask);
	RET_IF_BAD(-1 != status, FAIL, "wd.c-> MaskSignals(): sigemptyset() FAILED!\n");
	
	status = sigaddset(&mask, SIGUSR1);
	RET_IF_BAD(-1 != status, FAIL, "wd.c-> MaskSignals(): sigaddset() FAILED!\n");
	
	status = sigprocmask(SIG_BLOCK, &mask, NULL);
	RET_IF_BAD(-1 != status, FAIL, "wd.c-> MaskSignals(): sigprocmask() FAILED!\n");
	
	return SUCCESS;
}

static int UnMaskSignals()
{
	int status = -1;
	sigset_t mask;
	
	status = sigemptyset(&mask);
	RET_IF_BAD(-1 != status, FAIL, "wd.c-> UnMaskSignals(): sigemptyset() FAILED!\n");
	
	status = sigaddset(&mask, SIGUSR1);
	RET_IF_BAD(-1 != status, FAIL, "wd.c-> MaskSignals(): sigaddset() FAILED!\n");
	
	status = sigprocmask(SIG_UNBLOCK, &mask, NULL);
	RET_IF_BAD(-1 != status, FAIL, "wd.c-> MaskSignals(): sigprocmask() FAILED!\n");

	return SUCCESS;
}

static void HandleSIGUSR1(int sig)
{
	/* assert */
	assert(sig == SIGUSR1);
	
	/* reset g_counter */
	g_counter = 0;
}
