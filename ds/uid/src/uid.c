/**
	Written By: Tom Golzman
	Date: 24/04/2025
	Reviewed By: Amir Granot
**/

/************************************includes************************************/
#define _POSIX_C_SOURCE 200112L /* for gethostname() warning */
#include <unistd.h> 	/* gethostname() */
#include <netdb.h> 		/* gethostbyname() */
#include <netinet/in.h> /* struct in_addr */

#include "uid.h"

/************************************define************************************/
#define TRUE	(1)
#define FALSE	(0)

#define INVALID_IP	 ((unsigned int)0)
#define INVALID_PID	 ((pid_t)0)
#define INVALID_ID	 ((unsigned long)0)
#define INVALID_TIME ((time_t)-1)

/************************************Global Variables************************************/
static volatile unsigned long counter = 0;

const ilrd_uid_t invalid_uid = {INVALID_IP, INVALID_PID, INVALID_ID, INVALID_TIME};

/************************************Private Functions************************************/
static unsigned int getIp();

/************************************Functions************************************/
ilrd_uid_t UIDCreate()
{
	unsigned int ip = getIp();
	pid_t pid = getpid();
	time_t curr_time = time(NULL);
	ilrd_uid_t uid;

	if (INVALID_IP == ip || INVALID_TIME == curr_time)
	{
		return (invalid_uid);
	}
	
	uid.ip = ip;
	uid.pid = pid;
	uid.id = __atomic_fetch_add(&counter, 1, __ATOMIC_SEQ_CST);
	uid.time = curr_time;
	
	return (uid);
}

int UIDIsSame(const ilrd_uid_t id1, const ilrd_uid_t id2)
{
	if (id1.ip != id2.ip || id1.pid != id2.pid || id1.id != id2.id || id1.time != id2.time)
	{
		return (FALSE);
	}
	
	return (TRUE);
}

static unsigned int getIp()
{
	char hostname[256] = {'0'};
	
	struct hostent* host = NULL;
	if (-1 == gethostname(hostname, sizeof(hostname)))
	{
		return (INVALID_IP);
	}

	host = gethostbyname(hostname);
	if (NULL == host)
	{	
		return (INVALID_IP);
	}

	return (((struct in_addr*)host->h_addr_list[0])->s_addr);
}
