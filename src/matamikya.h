#ifndef MATAMIKYA_H_
#define MATAMIKYA_H_

#include <stdio.h>
#include <stdbool.h>

typedef enum MatamikyaResult_t {
    MATAMIKYA_SUCCESS = 0,
    MATAMIKYA_NULL_ARGUMENT,
    MATAMIKYA_OUT_OF_MEMORY,
    MATAMIKYA_INVALID_NAME,
    MATAMIKYA_INVALID_AMOUNT,
    MATAMIKYA_PRODUCT_ALREADY_EXIST,
    MATAMIKYA_PRODUCT_NOT_EXIST,
    MATAMIKYA_ORDER_NOT_EXIST,
    MATAMIKYA_INSUFFICIENT_AMOUNT,
} MatamikyaResult;

/** Type for specifying what is a valid amount for a product.
 * For a MATAMIKYA_INTEGER_AMOUNT product, a valid amount is an amount which is
 * within 0.001 of an integer. For example, 8.001 or 7.999 are considered a valid amount 
 * for MATAMIKYA_INTEGER_AMOUNT, but 8.0011 or 7.9989 are not.
 * 
 * For a MATAMIKYA_HALF_INTEGER_AMOUNT product, a valid amount is an amount which is
 * within 0.001 of a half-integer. For example, 8.001 is valid and 8.0011 is not valid,
 * but also 8.501 is valid and 8.5011 is not valid.
 * 
 * For MATAMIKYA_ANY_AMOUNT, any amount is valid. For example, this is suitable for
 * products which are measured by weight.
 */
typedef enum MatamikyaAmountType_t {
    MATAMIKYA_INTEGER_AMOUNT,
    MATAMIKYA_HALF_INTEGER_AMOUNT,
    MATAMIKYA_ANY_AMOUNT,
} MatamikyaAmountType;

/** Type for representing a Matamikya warehouse */
typedef struct Matamikya_t *Matamikya;

/** Type for additional custom data of a product */
typedef void *MtmProductData;

/**
 * Type of function for copying a product's custom data.
 *
 * Such a function receives a MtmProductData, creates a new MtmProductData
 * object, and returns a pointer to the new object. If the function fails for
 * any reason, it returns NULL.
 */
typedef MtmProductData (*MtmCopyData)(MtmProductData);

/**
 * Type of function for freeing a product's custom data.
 *
 * Such a function receives a MtmProductData object and frees it. The
 * MtmProductData can be NULL, in which case the function does nothing.
 */
typedef void (*MtmFreeData)(MtmProductData);

/**
 * Type of function for calculating the price of a product.
 *
 * Such a function receives the product's custom data (a MtmProductData) and an
 * amount (a double), and returns the price (a double) of purchasing the given
 * quantity of the given product.
 *
 * For example, a function that treats MtmProductData as the price of a single
 * item, with no discounts or other special considerations:
 * @code
 * double basicGetPrice(MtmProductData basePrice, double amount) {
 *     return (*(double*)basePrice) * amount;
 * }
 * @endcode
 */
typedef double (*MtmGetProductPrice)(MtmProductData, const double amount);

/**
 * matamikyaCreate: create an empty Matamikya warehouse.
 *
 * @return A new Matamikya warehouse in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
Matamikya matamikyaCreate();

/**
 * matamikyaDestroy: free a Matamikya warehouse, and all its contents, from
 * memory.
 *
 * @param matamikya - the warehouse to free from memory. A NULL value is
 *     allowed, and in that case the function does nothing.
 */
void matamikyaDestroy(Matamikya matamikya);

/**
 * mtmNewProduct: add a new product to a Matamikya warehouse.
 *
 * @param matamikya - warehouse to add the product to. Must be non-NULL.
 * @param id - new product id. Must be non-negative, and unique.
 * @param name - name of the product, e.g. "apple". Must be non-empty.
 * @param amount - the initial amount of the product when added to the warehouse.
 * @param amountType - defines what are valid amounts for this product.
 * @param customData - pointer to product's additional data
 * @param copyData - function pointer to be used for copying product's additional 
 *      data.
 * @param freeData - function pointer to be used for free product data.
 * @param prodPrice - function pointer to be used for getting the price of some 
 *      product.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if matamikya/name/customData/copyData/freeData
 *      /prodPrice are NULL.
 *     MATAMIKYA_INVALID_NAME - if name is empty, or doesn't start with a
 *         letter (a -z, A -Z) or a digit (0 -9).
 *     MATAMIKYA_INVALID_AMOUNT - if amount < 0, or is not consistent with amountType
 *         (@see MatamikyaAmountType documentation above)
 *     MATAMIKYA_PRODUCT_ALREADY_EXIST - if a product with the given id already exist.
 *     MATAMIKYA_SUCCESS - if product was added successfully.
 */
MatamikyaResult mtmNewProduct(Matamikya matamikya, const unsigned int id, const char *name,
                              const double amount, const MatamikyaAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice);
/**
 * mtmChangeProductAmount: increase or decrease the amount of an *existing* product in a Matamikya warehouse.
 * if 'amount' < 0 then this amount should be decreased from the matamikya warehouse.
 * if 'amount' > 0 then this amount should be added to the matamikya warehouse.
 * if 'amount' = 0 then nothing should be done.
 * please note:
 * If the amount to decrease is larger than the product's amount in the
 * warehouse, then the product's amount is not changed, and a proper error-code
 * is returned.
 * If the amount is equal to the product's amount in the
 * warehouse,then the product will remain inside the warehouse 
 * with amount of zero.
 *
 * @param matamikya - warehouse to add the product to. Must be non-NULL.
 * @param id - existing product id. Must exist in the warehouse.
 * @param amount - the amount of the product to increase/decrease to the warehouse.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_PRODUCT_NOT_EXIST - if matamikya does not contain a product with
 *         the given id.
 *     MATAMIKYA_INVALID_AMOUNT - if amount is not consistent with product's amount type
 *         (@see parameter amountType in mtmNewProduct).
 *     MATAMIKYA_INSUFFICIENT_AMOUNT - if 'amount' < 0 and the amount to be decreased
 *         is bigger than product's amount in the warehouse.
 *     MATAMIKYA_SUCCESS - if product amount was increased/decreased successfully.
 * @note Even if amount is 0 (thus the function will change nothing), still a proper
 *    error code is returned if one of the parameters is invalid, and MATAMIKYA_SUCCESS
 *    is returned if all the parameters are valid.
 */
MatamikyaResult mtmChangeProductAmount(Matamikya matamikya, const unsigned int id, const double amount);

/**
 * mtmClearProduct: clear a product from a Matamikya warehouse.
 *
 * The entire amount of the product is removed, and the product is erased
 * completely from the warehouse, from all existing orders and from the
 * 'income' mechanism(holding the profits for each existing product).
 * For example, after clearing a product with
 * mtmClearProduct, calling mtmChangeProductAmount on that product will fail.
 *
 * @param matamikya - warehouse to remove the product from.
 * @param id - id of product to be removed.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_PRODUCT_NOT_EXIST - if matamikya does not contain a product with
 *         the given id.
 *     MATAMIKYA_SUCCESS - if product was cleared successfully.
 */
MatamikyaResult mtmClearProduct(Matamikya matamikya, const unsigned int id);

/**
 * mtmCreateNewOrder: create a new empty order in a Matamikya warehouse, and
 * return the order's id.
 *
 * @param matamikya - a Matamikya warehouse
 * @return
 *     Positive id of the new order, if successful.
 *     0 in case of failure.
 */
unsigned int mtmCreateNewOrder(Matamikya matamikya);

/**
 * mtmChangeProductAmountInOrder: add/increase/remove/decrease products to an existing order.
 * Only products that exist inside the matamikya can be added to an order.
 * if 'amount' < 0 then this amount should be decreased from the product in the order.
 * if 'amount' > 0 then this amount should be added to the product in the order.
 * if 'amount' = 0 then nothing should be done.
 * please note:
 *  1) If the amount to decrease('amount' < 0) is *larger or equal* then the amount of the product in the
 *     order, then the product is removed entirely from the order.
 *  2) If 'amount' > 0 and the product doesn't exist inside the order then you should add it to the order
 *     with the amount given as argument.
 *
 * @param matamikya - warehouse containing the order and the product.
 * @param orderId - id of the order being modified.
 * @param productId - id of product to add to the order.
 * @param amount - amount of product to add to the order.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_ORDER_NOT_EXIST - if matamikya does not contain an order with
 *         the given orderId.
 *     MATAMIKYA_PRODUCT_NOT_EXIST - if matamikya does not contain a product with
 *         the given productId.
 *     MATAMIKYA_INVALID_AMOUNT - if amount is not consistent with product's amount type
 *         (@see parameter amountType in mtmNewProduct).
 *     MATAMIKYA_SUCCESS - if product was added/removed/increased/decreased to the order successfully.
 * @note Even if amount is 0 (thus the function will change nothing), still a proper
 *    error code is returned if one of the parameters is invalid, and MATAMIKYA_SUCCESS
 *    is returned if all the parameters are valid.
 */
MatamikyaResult mtmChangeProductAmountInOrder(Matamikya, const unsigned int orderId,
                                     const unsigned int productId, const double amount);

/**
 * mtmShipOrder: ship an order and remove it from a Matamikya warehouse.
 *
 * All products in the order are removed from the warehouse, and the order is
 * deleted. The amount of each product in the order is the amount of the product
 * that is removed from the warehouse. additionally once order is shipped 
 * the profit from the products shipped needs to be updated
 *
 * If the order cannot be shipped for any reason, e.g. some product's amount in
 * the order is larger than its amount in the warehouse, then the entire
 * operation is canceled - the order remains in the warehouse, and the
 * warehouse contents are not modified.
 *
 * @param matamikya - warehouse containing the order and all the products.
 * @param orderId - id of the order being shipped.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_ORDER_NOT_EXIST - if matamikya does not contain an order with
 *         the given orderId.
 *     MATAMIKYA_INSUFFICIENT_AMOUNT - if the order contains a product with an amount
 *         that is larger than its amount in matamikya.
 *     MATAMIKYA_SUCCESS - if the order was shipped successfully.
 */
MatamikyaResult mtmShipOrder(Matamikya matamikya, const unsigned int orderId);

/**
 * mtmCancelOrder: cancel an order and remove it from a Matamikya warehouse.
 *
 * The order is deleted from the warehouse. The products and their amounts in
 * the warehouse is not changed.
 *
 * @param matamikya - warehouse containing the order.
 * @param orderId - id of the order being canceled.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_ORDER_NOT_EXIST - if matamikya does not contain an order with
 *         the given orderId.
 *     MATAMIKYA_SUCCESS - if the order was shipped successfully.
 */
MatamikyaResult mtmCancelOrder(Matamikya matamikya, const unsigned int orderId);

/**
 * mtmPrintInventory: print a Matamikya warehouse and its contents as
 * explained in the *.pdf
 *
 * @param matamikya - a Matamikya warehouse to print.
 * @param output - an open, writable output stream, to which the contents are printed.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_SUCCESS - if printed successfully.
 */
MatamikyaResult mtmPrintInventory(Matamikya matamikya, FILE *output);

/**
 * matamikyaPrintOrder: print a summary of an order from a Matamikya warehouse,
 * as explained in the *.pdf
 *
 * The printout includes the total price of the order.
 *
 * @param matamikya - the Matamikya warehouse containing the order.
 * @param orderId - id of the order in matamikya.
 * @param output - an open, writable output stream, to which the order is printed.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_ORDER_NOT_EXIST - if matamikya does not contain an order with
 *         the given orderId.
 *     MATAMIKYA_SUCCESS - if printed successfully.
 */
MatamikyaResult mtmPrintOrder(Matamikya matamikya, const unsigned int orderId, FILE *output);

/**
 * mtmPrintBestSelling: print the best selling products of a Matamikya
 * warehouse, as explained in the *.pdf.
 *
 * @param matamikya - a Matamikya warehouse.
 * @param output - an open, writable output stream, to which the order is printed.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_SUCCESS - if printed successfully.
 */
MatamikyaResult mtmPrintBestSelling(Matamikya matamikya, FILE *output);

#endif /* MATAMIKYA_H_ */
