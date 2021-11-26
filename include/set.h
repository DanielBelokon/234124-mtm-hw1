#ifndef SET_H_
#define SET_H_

#include <stdbool.h>

/**
* Generic Set Container
*
* Implements a set container type.
* The set has an internal iterator for external use. For all functions
* where the state of the iterator after calling that function is not stated,
* it is undefined. That is you cannot assume anything about it.
*
* The following functions are available:
*   setCreate		- Creates a new empty set
*   setCopy		- Copies an existing set
*   setDestroy		- Deletes an existing set and frees all resources
*   setGetSize		- Returns the size of a given set
*   setIsIn		- returns weather or not an item exists inside the set.
*   				  This resets the internal iterator.
*   setGetFirst	- Sets the internal iterator to the first element in the
*   				  set, and returns it.
*   setGetNext		- Advances the internal iterator to the next element and
*   				  returns it.
*   setAdd			- Adds a new element to the set.
*   setRemove		- Removes an element which matches a given element (by the
*   				  compare function). Resets the internal iterator.
*	 setClear		- Clears the contents of the set. Frees all the elements of
*	 				  the set using the free function.
* 	 SET_FOREACH	- A macro for iterating over the set's elements.
*/

/** Type for defining the set */
typedef struct Set_t *Set;

/** Type used for returning error codes from set functions */
typedef enum SetResult_t {
	SET_SUCCESS,
	SET_OUT_OF_MEMORY,
	SET_NULL_ARGUMENT,
	SET_ITEM_ALREADY_EXISTS,
	SET_ITEM_DOES_NOT_EXIST
} SetResult;

/** Element data type for set container */
typedef void* SetElement;

/** Type of function for copying an element of the set */
typedef SetElement(*copySetElements)(SetElement);

/** Type of function for deallocating an element of the set */
typedef void(*freeSetElements)(SetElement);

/**
* Type of function used by the set to identify equal elements.
* This function will be used to deciding the iteration order of the set.
* This function should return:
* 		A positive integer if the first element is greater;
* 		0 if they're equal;
*		A negative integer if the second element is greater.
*/
typedef int(*compareSetElements)(SetElement, SetElement);

/**
* setCreate: Allocates a new empty set.
*
* @param copyElement - Function pointer to be used for copying elements into
*  	the set or when copying the set.
* @param freeElement - Function pointer to be used for removing elements from
* 		the set
* @param compareElements - Function pointer to be used for comparing elements
* 		inside the set. Used to check if new elements already exist in the set.
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new Set in case of success.
*/
Set setCreate(copySetElements copyElement, freeSetElements freeElement, compareSetElements compareElements);

/**
* setCopy: Creates a copy of target set.
*
* @param set - Target set.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Set containing the same elements as set otherwise.
*/
Set setCopy(Set set);

/**
* setDestroy: Deallocates an existing set. Clears all elements by using the
* stored free function.
*
* @param set - Target set to be deallocated. If set is NULL nothing will be
* 		done
*/
void setDestroy(Set set);

/**
* setGetSize: Returns the number of elements in a set
* @param set - The set which size is requested
* @return
* 	-1 if a NULL pointer was sent.
* 	Otherwise the number of elements in the set.
*/
int setGetSize(Set set);

/**
* setIsIn: Checks if an element exists in the set. The element will be
* considered in the set if one of the elements in the set it determined equal
* using the comparison function used to initialize the set.
*
* @param set - The set to search in
* @param element - The element to look for. Will be compared using the
* 		comparison function.
* @return
* 	false - if the input set is null, or if the element was not found.
* 	true - if the element was found in the set.
*/
bool setIsIn(Set set, SetElement element);

/**
*	setGetFirst: Sets the internal iterator (also called current element) to
*	the first element in the set. The "first" element is the one having the
*	lowest value as determined by the comparison function used to initialize the
*	set.
*	Use this to start iteraing over the set.
*	Use (To continue iteration use setGetNext)
*
* @param set - The set for which to set the iterator and return the first
* 		element.
* @return
* 	NULL if a NULL pointer was sent or the set is empty.
* 	The first element of the set otherwise
*/
SetElement setGetFirst(Set set);

/**
*	setGetNext: Advances the set iterator to the next element and returns it
*	The next element is determined by the comparison function induced order.
* @param set - The set for which to advance the iterator
* @return
* 	NULL if reached the end of the set, or the iterator is at an invalid state
* 	or a NULL sent as argument
* 	The next element on the set in case of success
*/
SetElement setGetNext(Set set);


/**
*	setAdd: Adds a new element to the set.
*  Iterator's value is undefined after this operation.
*
* @param set - The set for which to add an element
* @param element - The element to insert. A copy of the element will be
* 		inserted as supplied by the copying function which is given at
* 		initialization.
* @return
* 	SET_NULL_ARGUMENT if a NULL was sent as set
* 	SET_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* 	an element failed)
*  SET_ITEM_ALREADY_EXISTS if an equal item already exists in the set
* 	SET_SUCCESS the element has been inserted successfully
*/
SetResult setAdd(Set set, SetElement element);

/**
* 	setRemove: Removes an element from the set. The element is found using the
* 	comparison function given at initialization. Once found, the element is
* 	removed and deallocated using the free function supplied at initialzation.
*  Iterator's value is undefined after this operation.
*
* @param set -
* 	The set to remove the element from.
* @param element
* 	The element to remove from the set. The element will be freed using the
* 	free function given at initialization.
* @return
* 	SET_NULL_ARGUMENT if a NULL was sent as set
* 	SET_ITEM_DOES_NOT_EXIST if the element doesn't exist in the set
* 	SET_SUCCESS if the element was successfully removed.
*/
SetResult setRemove(Set set, SetElement element);

/**
* setClear: Removes all elements from target set.
* The elements are deallocated using the stored free function
* @param set
* 	Target set to remove all element from
* @return
* 	SET_NULL_ARGUMENT - if a NULL pointer was sent.
* 	SET_SUCCESS - Otherwise.
*/
SetResult setClear(Set);

/*!
* Macro for iterating over a set.
* Declares a new iterator for the loop.
*/
#define SET_FOREACH(type,iterator,set) \
	for(type iterator = setGetFirst(set) ; \
		iterator ;\
		iterator = setGetNext(set))

#endif /* SET_H_ */
