#ifndef AMOUNT_SET_STR_TESTS_H_
#define AMOUNT_SET_STR_TESTS_H_

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

#endif //AMOUNT_SET_STR_TESTS_H_