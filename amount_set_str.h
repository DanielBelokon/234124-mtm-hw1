#ifndef AMOUNT_SET_STR_H_
#define AMOUNT_SET_STR_H_

#include <stdio.h>
#include <stdbool.h>

/**
 * Amount Set Container
 *
 * Implements a sorted amount set container type for char*.
 * The set has an internal iterator for external use. For all functions
 * where the state of the iterator after calling that function is not stated,
 * it is undefined. That is you cannot assume anything about it.
 * The set is sorted in ascending order - iterating over the set is done in the
 * same order.
 *
 * The following functions are available:
 *   asCreate           - Creates a new empty set
 *   asDestroy          - Deletes an existing set and frees all resources
 *   asCopy             - Copies an existing set
 *   asGetSize          - Returns the size of the set
 *   asContains         - Checks if an element exists in the set
 *   asGetAmount        - Returns the amount of an element in the set
 *   asRegister         - Add a new element into the set
 *   asChangeAmount     - Increase or decrease the amount of an element in the set
 *   asDelete           - Delete an element completely from the set
 *   asClear            - Deletes all elements from target set
 *   asGetFirst         - Sets the internal iterator to the first element
 *                        in the set, and returns it.
 *   asGetNext          - Advances the internal iterator to the next element
 *                        and returns it.
 *   AS_FOREACH         - A macro for iterating over the set's elements
 */

/** Type for defining the set */
typedef struct AmountSet_t *AmountSet;

/** Type used for returning error codes from amount set functions */
typedef enum AmountSetResult_t {
    AS_SUCCESS = 0,
    AS_OUT_OF_MEMORY,
    AS_NULL_ARGUMENT,
    AS_ITEM_ALREADY_EXISTS,
    AS_ITEM_DOES_NOT_EXIST,
    AS_INSUFFICIENT_AMOUNT
} AmountSetResult;

/**
 * asCreate: Allocates a new empty amount set.
 *
 * @return
 *     NULL - if one of the parameters is NULL or allocations failed.
 *     A new amount set in case of success.
 */
AmountSet asCreate();

/**
 * asDestroy: Deallocates an existing amount set. Clears all elements.
 *
 * @param set - Target set to be deallocated. If set is NULL nothing will be done.
 */
void asDestroy(AmountSet set);

/**
 * asCopy: Creates a copy of target set.
 *
 * Iterator values for both sets are undefined after this operation.
 *
 * @param set - Target set.
 * @return
 *     NULL if a NULL was sent or a memory allocation failed.
 *     An amount set containing the same elements (and amounts) as set, otherwise.
 */
AmountSet asCopy(AmountSet set);

/**
 * asGetSize: Returns the number of elements in a set.
 *
 * Iterator's state is unchanged after this operation.
 *
 * @param set - The set which size is requested.
 * @return
 *     -1 if a NULL pointer was sent.
 *     Otherwise the number of elements in the set.
 */
int asGetSize(AmountSet set);

/**
 * asContains: Checks if an element exists in the set.
 *
 * The element will be considered in the set if one of the elements in the set
 * is determined equal (equality between strings).
 * Iterator's state is unchanged after this operation.
 *
 * @param set - The set to search in.
 * @param element - The element to look for.
 * @return
 *     false - if the input set is null, or if the element was not found.
 *     true - if the element was found in the set.
 */
bool asContains(AmountSet set, const char* element);

/**
 * asGetAmount: Returns the amount of an element in the set.
 *
 * The function returns an error code indicating wether the operation succeeded,
 * and in case of success also returns the element's amount via the outAmount
 * pointer.
 * Iterator's state is unchanged after this operation.
 *
 * @param set - The set which contains the element.
 * @param element - The element whose amount is requested.
 * @param outAmount - Pointer to the location where the amount is returned, in case
 *     of success. In case of failure, the contents of outAmount are unchanged.
 * @return
 *     AS_NULL_ARGUMENT - if a NULL argument was passed.
 *     AS_ITEM_DOES_NOT_EXIST - if the element doesn't exist in the set.
 *     AS_SUCCESS - if the amount was returned successfully.
 *
 * For example, to print the amount of the first item in the set:
 * @code
 * double firstAmount;
 * char* first = asGetFirst(set);
 * if (asGetAmount(set, first, &firstAmount) == AS_SUCCESS) {
 *     printf("First amount: %f\n", firstAmount);
 * }
 * @endcode
 */
AmountSetResult asGetAmount(AmountSet set, const char* element, double* outAmount);

/**
 * asRegister: Add a new element into the set.
 *
 * The element is added with an initial amount of 0.
 * Iterator's value is undefined after this operation.
 *
 * @param set - The target set to which the element is added.
 * @param element - The element to add.
 * @return
 *     AS_NULL_ARGUMENT - if a NULL argument was passed.
 *     AS_ITEM_ALREADY_EXISTS - if an equal element already exists in the set.
 *     AS_SUCCESS - if the element was added successfully.
 */
AmountSetResult asRegister(AmountSet set, const char* element);

/**
 * asChangeAmount: Increase or decrease the amount of an element in the set.
 *
 * Iterator's state is unchanged after this operation.
 *
 * @param set - The target set containing the element.
 * @param element - The element whose amount is increased.
 * @param amount - How much to change the element's amount. A positive value
 *     increases element's amount, while a negative value decreases it. A value
 *     of 0 means don't change it.
 * @return
 *     AS_NULL_ARGUMENT - if a NULL argument was passed.
 *     AS_ITEM_DOES_NOT_EXIST - if the element doesn't exist in the set.
 *     AS_INSUFFICIENT_AMOUNT - if amount is negative and the element's amount
 *         in the set is less than the amount that needs to be decreased (i.e.,
 *         if the change will result in a negative amount for the element in the
 *         set.)
 *     AS_SUCCESS - if the element's amount was changed successfully.
 *
 * @note parameter amount doesn't affect the return value. Even if amount is 0,
 *     still AS_ITEM_DOES_NOT_EXIST is returned if element doesn't exist in set,
 *     and AS_SUCCESS is returned if element exists in set (assuming set is not
 *     NULL).
 */
AmountSetResult asChangeAmount(AmountSet set, const char* element, double amount);

/**
 * asDelete: Delete an element completely from the set.
 *
 * This is the opposite of asRegister. The element is deallocated.
 * Iterator's value is undefined after this operation.
 *
 * @param set - The target set from which the element is deleted.
 * @param element - The element to delete.
 * @return
 *     AS_NULL_ARGUMENT - if a NULL argument was passed.
 *     AS_ITEM_DOES_NOT_EXIST - if the element doesn't exist in the set.
 *     AS_SUCCESS - if the element was deleted successfully.
 */
AmountSetResult asDelete(AmountSet set, const char* element);

/**
 * asClear: Deletes all elements from target set.
 *
 * The elements are deallocated.
 * Iterator's value is undefined after this operation.
 *
 * @param set - Target set to delete all elements from.
 * @return
 *     AS_NULL_ARGUMENT - if a NULL pointer was sent.
 *     AS_SUCCESS - Otherwise.
 */
AmountSetResult asClear(AmountSet set);

/**
 * asGetFirst: Sets the internal iterator (also called current element) to
 * the first  element in the set. The first element is the smallest element
 * of the set (alphabetically).
 * Use this to start iterating over the set.
 * To continue iteration use asGetNext.
 *
 * @param set - The set for which to set the iterator and return the first
 *     element itself, and not a copy of it.
 * @return
 *     NULL if a NULL pointer was sent or the set is empty.
 *     The first element of the set otherwise
 */
char* asGetFirst(AmountSet set);

/**
 * asGetNext: Advances the set iterator to the next element and returns it.
 * The iteration is in ascending order on the set's elements.
 *
 * @param set - The set for which to advance the iterator
 * @return
 *     NULL if reached the end of the set, or the iterator is at an invalid state
 *     or a NULL sent as argument
 *     The next element on the set in case of success
 */
char* asGetNext(AmountSet set);

/**
 * Macro for iterating over a set.
 * Declares a new iterator for the loop.
 */
#define AS_FOREACH(type, iterator, set)          \
    for(type iterator = (type) asGetFirst(set) ; \
        iterator ;                               \
        iterator = asGetNext(set))

#endif /* AMOUNT_SET_STR_H_ */
