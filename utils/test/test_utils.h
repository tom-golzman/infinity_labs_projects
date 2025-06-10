#ifndef __TEST_UTILS_H__
#define __TEST_UTILS_H__

/************************************colors************************************/
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"

/************************************titles************************************/
#define BOLD "\033[1m"
#define BOLD_TITLE "\033[1;4m"
#define TITLE "\033[4m"

/************************************status************************************/
enum { SUCCESS = 0, FAIL = 1 };

/************************************boolean************************************/
enum { FALSE = 0, TRUE = 1 };

#endif /* __TEST_UTILS_H__ */
