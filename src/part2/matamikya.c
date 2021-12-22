#include "matamikya.h"

struct Matamikya_t
{
    int id;
    // Product List
    // Order list
};

Matamikya matamikyaCreate()
{
}

void matamikyaDestroy(Matamikya matamikya)
{
}

MatamikyaResult mtmNewProduct(Matamikya matamikya, const unsigned int id, const char *name,
                              const double amount, const MatamikyaAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice)
{
}

MatamikyaResult mtmChangeProductAmount(Matamikya matamikya, const unsigned int id, const double amount)
{
}

MatamikyaResult mtmClearProduct(Matamikya matamikya, const unsigned int id)
{
}

MatamikyaResult mtmPrintBestSelling(Matamikya matamikya, FILE *output)
{
}