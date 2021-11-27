#include "amount_set_str.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

typedef struct AmountSetNode_t *AmountSetNode;
struct AmountSetNode_t
{
    double amount;
    char *element;
    AmountSetNode next;
};

struct AmountSet_t
{
    int size;
    AmountSetNode current_node;
    AmountSetNode first;
};

/**
 * asFreeNode: Frees a specific node's resources.
 * 
 * The node's and it's element's memory will be released, 
 * Adjacent nodes and the set's pointer will be untouched. 
 * 
 * @param node - The AmountSetNode to be released.
 * 
 * 
 * **/
void asFreeNode(AmountSetNode node);

/**
 * asFindNode: Set the iterator to the node containing a specific element.
 * 
 * Iterator value is set to the node containing the element if found,
 * The iterator is undefined if the element is not found.
 * 
 * @param set The set to search in.
 * @param element The element to match.
 * @return
 *      AS_NULL_ARGUMENT - if a NULL argument was passed,
 *      AS_ITEM_DOES_NOT_EXIST - if the element doesn't exist in the set.
 *      AS_SUCCESS - if the element was found and the node set.
 * **/
AmountSetResult asFindNode(AmountSet set, const char *element);

/**
 * asFindPrecedingNode: Set the iterator to the node containing a specific element, and find the preceding node.
 * 
 * Iterator value is set to the node containing the element if found,
 * The iterator is undefined if the element is not found.
 * The preceding node pointer is set to the node preceding the node that contains the element, if found.
 * 
 * @param set The set to search in.
 * @param element The element to match.
 * @param preceding_node The variable to return the result in.
 * @return
 *      AS_NULL_ARGUMENT - if a NULL argument was passed,
 *      AS_ITEM_DOES_NOT_EXIST - if the element doesn't exist in the set.
 *      AS_SUCCESS - if the element was found and the node set.
 * **/
AmountSetResult asFindPrecedingNode(AmountSet set, const char *element, AmountSetNode *preceding_node);

AmountSet asCreate()
{
    AmountSet new_set = malloc(sizeof(*new_set));

    if (new_set == NULL)
        return NULL;

    new_set->size = 0;
    new_set->current_node = NULL;
    new_set->first = NULL;

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

    AmountSet new_set = asCreate();
    AS_FOREACH(char *, current_element, set)
    {
        if (asRegister(new_set, current_element))
        {
            asDestroy(new_set);
            return NULL;
        }
    };

    return new_set;
}

int asGetSize(AmountSet set)
{
    if (!set)
        return 0;

    return set->size;
}

bool asContains(AmountSet set, const char *element)
{
    if (!set || !element)
        return false;

    AmountSetNode last_position = set->current_node;
    bool operation_result = asFindNode(set, element);
    set->current_node = last_position;

    return (operation_result == AS_SUCCESS);
}

AmountSetResult asGetAmount(AmountSet set, const char *element, double *outAmount)
{
    AmountSetResult operation_result;

    if ((operation_result = asFindNode(set, element)) == AS_SUCCESS)
    {
        assert(set->current_node);
        *outAmount = set->current_node->amount;
    }

    return operation_result;
}

AmountSetResult asRegister(AmountSet set, const char *element)
{
    if (!set || !element)
        return AS_NULL_ARGUMENT;

    AmountSetNode preceding_node = NULL;
    if (asFindPrecedingNode(set, element, &preceding_node) == AS_SUCCESS)
        return AS_ITEM_ALREADY_EXISTS;

    AmountSetNode new_node = malloc(sizeof(*new_node));
    if (!new_node)
        return AS_OUT_OF_MEMORY;

    char *new_element = malloc((strlen(element) + 1) * sizeof(char));
    if (!new_element)
    {
        free(new_node);
        return AS_OUT_OF_MEMORY;
    }
    strcpy(new_element, element);
    new_node->amount = 0;
    new_node->element = new_element;

    if (preceding_node != NULL)
    {
        new_node->next = preceding_node->next;
        preceding_node->next = new_node;
    }
    else
    {
        new_node->next = set->first;
        set->first = new_node;
    }

    set->size++;
    return AS_SUCCESS;
}

AmountSetResult asChangeAmount(AmountSet set, const char *element, const double amount)
{
    if (!set || !element)
        return AS_NULL_ARGUMENT;

    AmountSetNode last_position = set->current_node;
    AmountSetResult operation_result;

    if ((operation_result = asFindNode(set, element)) == AS_SUCCESS)
    {
        if (set->current_node->amount + amount < 0)
            operation_result = AS_INSUFFICIENT_AMOUNT;

        set->current_node->amount += amount;
    }

    set->current_node = last_position;
    return operation_result;
}

AmountSetResult asDelete(AmountSet set, const char *element)
{
    if (!set || !element)
        return AS_NULL_ARGUMENT;

    AmountSetNode preceding_node = NULL;

    if (asFindPrecedingNode(set, element, &preceding_node) == AS_ITEM_DOES_NOT_EXIST)
        return AS_ITEM_DOES_NOT_EXIST;

    if (preceding_node != NULL)
        preceding_node->next = set->current_node->next;
    else
        set->first = set->current_node->next;

    asFreeNode(set->current_node);
    set->current_node = set->first;
    set->size--;
    return AS_SUCCESS;
}

AmountSetResult asClear(AmountSet set)
{
    if (set == NULL)
        return AS_NULL_ARGUMENT;

    AmountSetNode next_node, current_node = set->first;
    while (current_node != NULL)
    {
        next_node = current_node->next;
        asFreeNode(current_node);
        current_node = next_node;
    }
    set->size = 0;
    return AS_SUCCESS;
}

char *asGetFirst(AmountSet set)
{
    if (!set || !(set->first))
        return NULL;

    set->current_node = set->first;
    return set->first->element;
}

char *asGetNext(AmountSet set)
{
    if (!set || set->current_node == NULL)
        return NULL;

    if (set->current_node->next != NULL)
        set->current_node = set->current_node->next;
    else
        return NULL;

    return set->current_node->element;
}

void asFreeNode(AmountSetNode node)
{
    if (node == NULL)
        return;

    free(node->element);
    free(node);

    return;
}

AmountSetResult asFindNode(AmountSet set, const char *element)
{
    AmountSetNode preceding_node;
    return asFindPrecedingNode(set, element, &preceding_node);
}

AmountSetResult asFindPrecedingNode(AmountSet set, const char *element, AmountSetNode *preceding_node)
{
    if (!set || !element)
        return AS_NULL_ARGUMENT;

    AmountSetResult operation_result = AS_ITEM_DOES_NOT_EXIST;
    int compare_result = 1;
    AS_FOREACH(char *, current_element, set)
    {
        if ((compare_result = strcmp(current_element, element)) == 0)
        {
            operation_result = AS_SUCCESS;
            break;
        }
        else if (compare_result > 0) // Passed the element
            break;
        if (set->current_node != NULL)
            *preceding_node = set->current_node;
    }

    return operation_result;
}