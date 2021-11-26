#include "amount_set.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct AmountSetNode_t
{
    double amount;
    ASElement element;
    AmountSetNode next;
} * AmountSetNode;

struct AmountSet_t
{
    int size;
    AmountSetNode current_node;
    AmountSetNode first;

    CopyASElement copyElement;
    FreeASElement freeElement;
    CompareASElements compareElements;
};

AmountSet asCreate(CopyASElement copyElement,
                   FreeASElement freeElement,
                   CompareASElements compareElements)
{
    if (!copyElement || !freeElement || !compareElements)
        return NULL;

    AmountSet new_set = malloc(new_set);

    if (!new_set)
        return NULL;

    new_set->size = 0;
    new_set->current_node = NULL;
    new_set->first = NULL;

    new_set->copyElement = copyElement;
    new_set->freeElement = freeElement;
    new_set->compareElements = compareElements;

    return new_set;
}

void asDestroy(AmountSet set)
{
    if (!set)
        return;

    asClear(set);

    free(set);
}

AmountSet asCopy(AmountSet set)
{
    if (!set)
        return NULL;

    AmountSet new_set = asCreate(set->copyElement, set->freeElement, set->compareElements);

    AS_FOREACH(ASElement, current_element, set)
    {
        asRegister(new_set, current_element);
    };

    // ASElement next_element = asGetFirst(set);
    // while (next_element)
    // {
    //     asRegister(new_set, set->copyElement());
    //     next_element = asGetNext(set);
    // }

    return new_set;
}

int asGetSize(AmountSet set)
{
    if (!set)
        return 0;

    return set->size;
}

// find an item (sets iterator to it's location)
AmountSetResult asFind(AmountSet set, ASElement element)
{
    if (!set || !element)
        return AS_NULL_ARGUMENT;

    AmountSetResult operation_result = AS_ITEM_DOES_NOT_EXIST;

    AS_FOREACH(ASElement, current_element, set)
    {
        if (set->compareElements(current_element, element))
        {
            operation_result = AS_SUCCESS;
            break;
        }
    }

    return operation_result;
}

bool asContains(AmountSet set, ASElement element)
{
    if (!set || !element)
        return false;

    AmountSetNode last_position = set->current_node;
    bool operation_result = asFind(set, element);
    set->current_node = last_position;

    return (operation_result == AS_SUCCESS);
}

AmountSetResult asGetAmount(AmountSet set, ASElement element, double *outAmount)
{
    AmountSetResult operation_result;

    if ((operation_result = asFind(set, element)) == AS_SUCCESS)
    {
        assert(set->current_node);
        *outAmount = set->current_node->amount;
    }

    return operation_result;
}

AmountSetResult asRegister(AmountSet set, ASElement element)
{
    if (!set || !element)
        return AS_NULL_ARGUMENT;

    int compare_result = 0;
    AmountSetNode previous_pos = NULL;

    AS_FOREACH(ASElement, current_element, set)
    {
        compare_result = set->compareElements(current_element, element);
        if (compare_result == 0)
            return AS_ITEM_ALREADY_EXISTS;
        else if (compare_result > 0)
            break;

        previous_pos = set->current_node;
    }

    AmountSetNode new_node = malloc(new_node);
    if (!new_node)
        return AS_OUT_OF_MEMORY;

    assert(set->current_node);
    new_node->amount = 0;
    new_node->element = element;
    new_node->next = set->current_node;

    previous_pos->next = new_node;

    return AS_SUCCESS;
}

AmountSetResult asChangeAmount(AmountSet set, ASElement element, const double amount)
{
    if (!set || !element)
        return AS_NULL_ARGUMENT;

    AmountSetNode last_position = set->current_node;
    AmountSetResult operation_result;

    if ((operation_result = asFind(set, element)) == AS_SUCCESS)
    {
        if (set->current_node->amount + amount < 0)
            operation_result = AS_INSUFFICIENT_AMOUNT;

        set->current_node->amount += amount;
    }

    set->current_node = last_position;
    return operation_result;
}

AmountSetResult asDelete(AmountSet set, ASElement element)
{
    assert(false && "NOT IMPLEMENTED");
    return AS_NULL_ARGUMENT;
}

AmountSetResult asClear(AmountSet set)
{
    assert(false && "NOT IMPLEMENTED");
    return AS_NULL_ARGUMENT;
}

ASElement asGetFirst(AmountSet set)
{
    assert(false && "NOT IMPLEMENTED");
    return AS_NULL_ARGUMENT;
}

ASElement asGetNext(AmountSet set)
{
    assert(false && "NOT IMPLEMENTED");
    return AS_NULL_ARGUMENT;
}