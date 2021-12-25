#include "amount_set_str_tests.h"
#include <stdio.h>
#include <stdbool.h>

int main()
{
    RUN_TEST(testCreate);
    RUN_TEST(testDestroy);
    RUN_TEST(testCopy);
    RUN_TEST(testGetSize);
    RUN_TEST(testContains);
    RUN_TEST(testGetAmount);
    RUN_TEST(testRegister);
    RUN_TEST(testChangeAmount);
    RUN_TEST(testDelete);
    RUN_TEST(testClear);
    RUN_TEST(testGetFirst);
    RUN_TEST(testGetNext);
    RUN_TEST(testOrdered);
    return 0;
}