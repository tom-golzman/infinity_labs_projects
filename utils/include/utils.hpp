#ifndef __UTILS_HPP__
#define __UTILS_HPP__

/************************************ Define ************************************/
/* trash memory before free */
#define BAD_MEM32(T)   ((T)0xDEADBEEF)
#define BAD_MEM64(T)   ((T)0xBADC0FFEE0DDF00D)

#if defined(__x86_64__) || defined(_M_X64)
    #define BAD_MEM(T)   (BAD_MEM64(T))
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
    #define BAD_MEM(T)   (BAD_MEM32(T))
#else
    #define BAD_MEM(T)   (BAD_MEM32(T)) /* fallback */
#endif

#if __cplusplus<201104
#define NOEXCEPT throw()
#define OVERRIDE
#else
#define NOEXCEPT noexcept
#define OVERRIDE override
#endif //#if __cplusplus<201104

/************************************ Debug ************************************/
#ifdef NDEBUG
    #define DEBUG_ONLY(statements)
#else
    #define DEBUG_ONLY(statements) \
        do {                       \
            statements             \
        } while (0)
#endif

/************************************ Enum ************************************/
/* status */
enum { SUCCESS = 0, FAIL = 1 };

#endif /* __UTILS_HPP__ */
