#ifndef __TEST_UTILS_HPP__
#define __TEST_UTILS_HPP__

/************************************ Colors ************************************/
#define GREEN "\033[1;32m"
#define RED   "\033[1;31m"
#define RESET "\033[0m"

/************************************ Titles ************************************/
#define BOLD       "\033[1m"
#define BOLD_TITLE "\033[1;4m"

/************************************ Status ************************************/
namespace test
{
    enum Status
    {
        SUCCESS = 0,
        FAIL    = 1
    };
}

#define RUN_TEST(name, condition)                                          \
    do {                                                                   \
        if (condition)                                                     \
        {                                                                  \
            std::cout << GREEN << "[PASS] " << RESET << name << std::endl; \
        }                                                                  \
        else                                                               \
        {                                                                  \
            std::cout << RED << "[FAIL] " << RESET << name << std::endl;   \
        }                                                                  \
    } while (0)

#define TITLE(title)                                                       \
    do {                                                                   \
        std::cout << std::endl                                             \
                  << BOLD_TITLE << title << RESET                          \
                  << std::endl;                                            \
    } while (0)

#endif /* __TEST_UTILS_HPP__ */
