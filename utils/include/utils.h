#ifndef __UTILS_H__
#define __UTILS_H__

/************************************Includes************************************/
#include <stdio.h> /* perror */

/************************************Define************************************/
/* trash memory before free */
#define BAD_MEM32(T)  	((T)0xDEADBEEF)
#define BAD_MEM64(T)  	((T)0xBADC0FFEE0DDF00D)

#if defined(__x86_64__) || defined(N)
    	#define BAD_MEM(T)  	(BAD_MEM64(T))
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
    	#define BAD_MEM(T)  	(BAD_MEM32(T))
#endif

/* debug statements */
#ifdef NDEBUG
  	#define DEBUG_ONLY(statements)
#else
  	#define DEBUG_ONLY(statements) \
	  	do { \
			statements \
		} while (0)
#endif

/************************************enum************************************/
/* status */
enum { SUCCESS = 0, FAIL = 1 };

/* boolean */
enum { FALSE = 0, TRUE = 1 };

/************************************Functions************************************/
/* systemcalls handling */
void ExitIfBad(int is_good, int exit_stat, const char* msg);

void Log(const char* message);

/* return if bad macro */
#define RET_IF_BAD(is_good, return_status, msg) do{ \
    if(!(is_good)) \
    { \
		perror(msg); \
		return return_status; \
    } \
} while(0)

/* return if bad with clean function macro */
#define RET_IF_BAD_CLEAN(is_good, return_status, msg, cleanup_call)	do{ \
	if(!(is_good)) \
	{ \
		perror(msg); \
		cleanup_call; \
		return return_status; \
	} \
}while(0)

/* macro for using RET_IF_BAD in void functions */
#define NOTHING

#endif /* __UTILS_H__ */
