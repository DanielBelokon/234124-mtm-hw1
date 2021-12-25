#ifndef AMOUNT_SET_STR_TESTS_H_
#define AMOUNT_SET_STR_TESTS_H_

#include <stdbool.h>

#define RUN_TEST(test)                             \
    do                                             \
    {                                              \
        printf("Running %-20s", #test "... ");     \
        printf("\033[1m\033[31m");                 \
        if (test())                                \
        {                                          \
            printf("\033[0m[OK]");                 \
        }                                          \
        else                                       \
        {                                          \
            printf("\033[1m\033[31m%-26s", #test); \
            printf("[FAIL]\033[0m");               \
        }                                          \
        printf("\033[0m\n");                       \
    } while (0)

bool testCreate();
bool testDestroy();
bool testCopy();
bool testGetSize();
bool testContains();
bool testGetAmount();
bool testRegister();
bool testChangeAmount();
bool testDelete();
bool testClear();
bool testGetFirst();
bool testGetNext();
bool testOrdered();

#endif //AMOUNT_SET_STR_TESTS_H_