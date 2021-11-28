#include <stdio.h>
#include "amount_set_str.h"
#include <stdbool.h>
#include "string.h"

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

AmountSet CreateDummy(int items);

bool testCreate()
{
    AmountSet set = asCreate();

    bool passed = true;
    if (set == NULL)
        passed = false;

    if (asGetFirst(set) != NULL)
    {
        printf("First not set to NULL\n");
        passed = false;
    }

    if (asGetNext(set) != NULL)
    {
        printf("Next not set to NULL\n");
        passed = false;
    }

    if (asGetSize(set) != 0)
    {
        printf("Size not set to 0\n");
        passed = false;
    }

    asDestroy(set);
    return passed;
}

bool testDestroy()
{
    AmountSet set = CreateDummy(4);

    asDestroy(set);
    // not much to verify from inside the prog...
    return true;
}

bool testCopy()
{
    AmountSet source_set = CreateDummy(5);
    AmountSet dest_set = asCopy(source_set);
    bool passed = true;

    if (asGetSize(dest_set) != asGetSize(source_set))
    {
        printf("Size mismatch.\n");
        passed = false;
    }

    AS_FOREACH(char *, item, source_set)
    {
        if (!asContains(dest_set, item))
        {
            printf("Contents mismatch.\n");
            passed = false;
        }

        double source_amount, dest_amount;
        asGetAmount(dest_set, item, &dest_amount);
        asGetAmount(source_set, item, &source_amount);

        if (source_amount != dest_amount)
        {
            printf("Amount mismatch.\n");
            passed = false;
        }
    }

    asDestroy(source_set);

    asDestroy(dest_set);
    return passed;
}

bool testGetSize()
{
    bool passed = true;
    AmountSet set = asCreate();

    if (asGetSize(set) != 0)
    {
        printf("Initiale size not 0.\n");
        passed = false;
    }

    asRegister(set, "Item 1");
    asRegister(set, "Item 2");

    if (asGetSize(set) != 2)
    {
        printf("Post-register size incorrect.\n");
        passed = false;
    }

    asDelete(set, "Item 1");

    if (asGetSize(set) != 1)
    {
        printf("Post-delete size incorrect.\n");
        passed = false;
    }

    asClear(set);

    if (asGetSize(set) != 0)
    {
        printf("Post-clear size incorrect.\n");
        passed = false;
    }

    if (asDestroy)

        return passed;
}

bool testContains()
{
    bool passed = true;
    AmountSet set = CreateDummy(3);

    if (!asContains(set, "Item 1"))
    {
        passed = false;
        printf("Incorrect result in contains.\n");
    }

    asDelete(set, "Item 1");

    if (asContains(set, "Item 1"))
    {
        passed = false;
        printf("Incorrect result in contains post delete.\n");
    }

    return passed;
}

bool testGetAmount()
{
    bool passed = true;
    double amount = -1;

    AmountSet set = CreateDummy(3);

    AmountSetResult operation_result;

    if ((operation_result = asGetAmount(set, "Item 1", &amount)) != AS_SUCCESS)
    {
        printf("asGetAmount failed on execute with error: %s\n", operation_result);
        passed = false;
    }

    if (amount != 0)
    {
        printf("Incorrect initial amount.\n");
        passed = false;
    }

    amount = -1;

    asChangeAmount(set, "Item 1", 3);

    if ((operation_result = asGetAmount(set, "Item 1", &amount)) != AS_SUCCESS)
    {
        printf("asGetAmount failed on execute with error: %s\n", operation_result);
        passed = false;
    }

    if (amount != 3)
    {
        printf("Incorrect post-update amount.\n");
        passed = false;
    }

    asChangeAmount(set, "Item 1", -2);

    if ((operation_result = asGetAmount(set, "Item 1", &amount)) != AS_SUCCESS)
    {
        printf("asGetAmount failed on execute with error: %s\n", operation_result);
        passed = false;
    }

    if (amount != 1)
    {
        printf("Incorrect post-update negative amount.\n");
        passed = false;
    }

    asDestroy(set);
    return passed;
}

bool testRegister()
{
    bool passed = true;

    return passed;
}

bool testChangeAmount()
{
    bool passed = true;
    double amount = -1;

    AmountSet set = CreateDummy(3);

    asChangeAmount(set, "Item 1", 3);
    asGetAmount(set, "Item 1", &amount);
    if (amount != 3)
    {
        printf("Incorrect post-update amount, now %lf instead of 3.\n", amount);
        passed = false;
    }

    asChangeAmount(set, "Item 1", -2);
    asGetAmount(set, "Item 1", &amount);
    if (amount != 1)
    {
        printf("Incorrect post-update negative amount, now %lf instead of 1.\n", amount);
        passed = false;
    }

    if (asChangeAmount(set, "Item 1", -2) != AS_INSUFFICIENT_AMOUNT)
    {
        printf("Incorrect or no error on insufficient amount.\n");
        passed = false;
    }

    if (amount != 1)
    {
        printf("Amount changed after insufficient amount, now %lf instead of 1.\n", amount);
        passed = false;
    }

    asDestroy(set);
    return passed;
}

bool testDelete()
{
    bool passed = true;
    AmountSet set = CreateDummy(3);
    AmountSetResult operation_result;

    if ((operation_result = asDelete(set, "Item 2")) != AS_SUCCESS)
    {
        printf("asDelete failed on execute with error: %s\n", operation_result);
        passed = false;
    }

    if (asContains(set, "Item 2"))
    {
        printf("Set still contains deleted item.\n");
        passed = false;
    }

    if ((operation_result = asDelete(set, "Item 2")) != AS_ITEM_DOES_NOT_EXIST)
    {
        printf("asDelete returned incorrect error: %s, instead of AS_ITEM_DOES_NOT_EXIST\n", operation_result);
        passed = false;
    }

    asDestroy(set);
    return passed;
}

bool testClear()
{
    bool passed = true;
    AmountSet set = CreateDummy(5);
    AmountSetResult operation_result;
    if ((operation_result = asClear(set)) != AS_SUCCESS)
    {
        printf("asClear failed on execute with error: %s\n", operation_result);
        passed = false;
    }

    if (asGetSize(set) != 0)
    {
        printf("Set size is not zero.\n");
        passed = false;
    }

    if (asGetFirst(set) != NULL)
    {
        printf("First item is not NULL.\n");
        passed = false;
    }

    asDestroy(set);
    return passed;
}

bool testGetFirst()
{
    bool passed = true;

    return passed;
}

bool testGetNext()
{
    bool passed = true;

    return passed;
}

bool testOrdered()
{
    bool passed = true;

    return passed;
}

AmountSet CreateDummy(int items)
{
    AmountSet set = asCreate();
    for (int i = 1; i <= items; i++)
    {
        char item[10];
        sprintf(item, "Item %d", i);
        asRegister(set, item);
    }

    return set;
}

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