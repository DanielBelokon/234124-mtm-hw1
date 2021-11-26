#ifndef MATAMIKYA_PRINT_H_
#define MATAMIKYA_PRINT_H_

#include <stdio.h>

/**
 * mtmPrintProductDetails: print the details of a single product, as required
 * from mtmPrintInventory and mtmPrintOrder.
 */
void mtmPrintProductDetails(const char* name, const unsigned int id, const double amount, const double price, FILE* output);

/**
 * mtmPrintOrderHeading: print the heading line of an order, as required from
 * mtmPrintOrder.
 */
void mtmPrintOrderHeading(const unsigned int orderId, FILE* output);

/**
 * mtmPrintOrderSummary: print the sumamry line of an order, as required from
 * mtmPrintOrder.
 */
void mtmPrintOrderSummary(const double totalOrderPrice, FILE* output);

/**
 * mtmPrintIncomeLine: print the income line of a product, as required from
 * mtmPrintBestSelling.
 */
void mtmPrintIncomeLine(const char* name, const unsigned int id, const double totalIncome, FILE* output);

#endif /* MATAMIKYA_PRINT_H_ */
