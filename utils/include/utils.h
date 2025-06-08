#ifndef __UTILS_H__
#define __UTILS_H__

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
  	#define DEBUG_ONLY(statements)  do { \
		statements \
} while (0)
#endif

/* status */
enum { SUCCESS = 0, FAIL = 1 };

/* boolean */
enum { FALSE = 0, TRUE = 1 };

#endif /* __UTILS_H__ */

