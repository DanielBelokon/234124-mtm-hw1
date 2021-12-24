#ifndef MATAMIKYA_PRODUCT_H_
#define MATAMIKYA_PRODUCT_H_

#include "matamikya.h"
#define PRODUCT_NULL_ARG -1;

typedef struct Product_t *Product;
struct Product_t
{
    unsigned int id;
    char *name;
    MatamikyaAmountType amountType;
    MtmProductData customData;
    int amount;
    int profit;

    MtmCopyData copyProdData;
    MtmFreeData freeProdData;
    MtmGetProductPrice getProdPrice;
};

void *productCopy(void *from);
void productDelete(void *product);
Product productCreate(const unsigned int id, const char *name,
                      const double amount, const MatamikyaAmountType amountType,
                      const MtmProductData customData, MtmCopyData copyData,
                      MtmFreeData freeData, MtmGetProductPrice prodPrice,
                      MatamikyaResult *result);
int productCompare(void *, void *);
int productGetId(Product product);
int productGetProfit(Product product);
MatamikyaResult productChangeAmount(Product product, const double amount);

bool isAmountValid(const double amount, MatamikyaAmountType type);

#endif