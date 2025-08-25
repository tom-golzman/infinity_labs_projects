#ifndef __TEST_UTILS_HPP__
#define __TEST_UTILS_HPP__

/************************************ Colors ************************************/
#define GREEN "\033[1;32m"
#define RED   "\033[1;31m"
#define RESET "\033[0m"

/************************************ Titles ************************************/
#define BOLD       "\033[1m"
#define BOLD_TITLE "\033[1;4m"
#define TITLE      "\033[4m"

/************************************ Status ************************************/
namespace test
{
    enum Status
    {
        SUCCESS = 0,
        FAIL    = 1
    };
}

#endif /* __TEST_UTILS_HPP__ */
