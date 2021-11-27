#include <stdio.h>
#include "amount_set_str.h"
#include <stdbool.h>
#include "string.h"

#define RUN_TEST(test)                                                \
    do                                                                \
    {                                                                 \
        printf("Running %-20s", #test "... ");                        \
        if (test())                                                   \
        {                                                             \
            printf("[OK]");                                           \
        }                                                             \
        else                                                          \
        {                                                             \
            printf("\033[1m\033[31m" #test "%-16s", "[FAIL]\033[0m"); \
        }                                                             \
        printf("\n");                                                 \
    } while (0)

AmountSet CreateDummy(int items);

bool testCreate()
{
    AmountSet set = asCreate();

    bool result = true;
    if (set == NULL)
        result = false;

    if (asGetFirst(set) != NULL)
    {
        printf("\nFirst not set to NULL\n");
        result = false;
    }

    if (asGetNext(set) != NULL)
    {
        printf("\nNext not set to NULL\n");
        result = false;
    }

    if (asGetSize(set) != 0)
    {
        printf("\nSize not set to 0\n");
        result = false;
    }

    asDestroy(set);
    return result;
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
    bool result = true;

    if (asGetSize(dest_set) != asGetSize(source_set))
    {
        printf("Size mismatch.\n");
        result = false;
    }

    AS_FOREACH(char *, item, source_set)
    {
        if (!asContains(dest_set, item))
        {
            printf("Contents mismatch.\n");
            result = false;
        }

        double source_amount, dest_amount;
        asGetAmount(dest_set, item, &dest_amount);
        asGetAmount(source_set, item, &source_amount);

        if (source_amount != dest_amount)
        {
            printf("Amount mismatch.\n");
            result = false;
        }
    }

    asDestroy(source_set);

    asDestroy(dest_set);
    return result;
}

bool testGetSize()
{
    return true;
}

bool testContains()
{
    return true;
}

bool testGetAmount()
{
    return true;
}

bool testRegister()
{
    return true;
}

bool testChangeAmount()
{
    return true;
}

bool testDelete()
{
    return true;
}

bool testClear()
{
    return true;
}

bool testGetFirst()
{
    return true;
}

bool testGetNext()
{
    return true;
}

bool testOrdered()
{
    return true;
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