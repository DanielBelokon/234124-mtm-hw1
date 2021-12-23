#include "matamikya_product.h"
#include "matamikya.h"

struct Product_t
{
    unsigned int id;
    char *name;
    MatamikyaAmountType amountType;
    MtmProductData customData;

    MtmCopyData copyProdData;
    MtmFreeData freeProdData;
    MtmGetProductPrice getProdPrice;
};