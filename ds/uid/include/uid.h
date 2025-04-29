#ifndef __ILRD_UID_H__
#define __ILRD_UID_H__

/************************************includes************************************/
#include <time.h>		/* time() */
#include <sys/types.h> 	/* pid_t */

/************************************typedef************************************/
typedef struct uid {
	unsigned int ip;
	pid_t pid;
	unsigned long id;
	time_t time;
} ilrd_uid_t;

/************************************Global variables************************************/
extern const ilrd_uid_t invalid_uid;

/************************************Functions************************************/
/*
	DESCRIPTION: create a unique id.
	RETURNS: ilrd_uid_t id, on error returns invalid_uid.
	ARGUMENTS: (none)
	LIMITATIONS: (none)
	NOTES: (none)
	COMPLEXITY: 
			Time: O(1)
*/
ilrd_uid_t UIDCreate();

/*
	DESCRIPTION: compares 2 uids.
	RETURNS: 1 if TRUE and 0 if FALSE.
	ARGUMENTS: 
		@const ilrd_uid_t - an id .
		@const ilrd_uid_t - an id to compare against.
	LIMITATIONS: (none)
	NOTES: (none)
	COMPLEXITY: 
			Time: O(1)
*/
int UIDIsSame(const ilrd_uid_t id1, const ilrd_uid_t id2);

#endif /* __ILRD_UID_H__ */
