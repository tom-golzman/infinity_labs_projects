/**
	Written By: Tom Golzman
	Date: 24/04/2025
	Reviewed By: Amir Granot
**/

/************************************includes************************************/
#include <unistd.h> 	/* gethostname */
#include <netdb.h> 		/* gethostbyname */
#include <netinet/in.h> /* struct in_addr */

#include "uid.h"

/************************************define************************************/
#define TRUE (1)
#define FALSE (0)

#define INVALID_IP ((unsigned int)0)
#define INVALID_PID ((pid_t)0)
#define INVALID_ID ((unsigned long)0)
#define INVALID_TIME ((time_t)-1)

/************************************Global Variables************************************/
static unsigned long counter = 0;

const ilrd_uid_t invalid_uid = {INVALID_IP, INVALID_PID, INVALID_ID, INVALID_TIME};

/************************************Private Functions************************************/
static unsigned int getIp();

/************************************Functions************************************/
ilrd_uid_t UIDCreate()
{
	unsigned int ip = getIp();
	pid_t pid = getpid();
	unsigned long id = counter + 1;
	time_t curr_time = time(NULL);
	ilrd_uid_t uid;

	if (INVALID_IP == ip || INVALID_TIME == curr_time)
	{
		return (invalid_uid);
	}
	
	uid.ip = ip;
	uid.pid = pid;
	uid.id = id;
	uid.time = curr_time;
	
	counter++;
	if (0 == counter)
	{
		counter++;
	}
	
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
		return (0);
	}

	host = gethostbyname(hostname);
	if (NULL == host)
	{	
		return (0);
	}

	return (((struct in_addr*)host->h_addr_list[0])->s_addr);
}
