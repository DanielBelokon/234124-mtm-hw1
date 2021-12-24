#include "matamikya_product.h"
#include "matamikya.h"
#include <string.h>
#include <stdlib.h>

void *productCopy(void *from)
{
    Product new_product = malloc(sizeof(*new_product));
    Product old_product = (Product)from;

    new_product->amount = old_product->amount;
    new_product->amountType = old_product->amountType;
    new_product->copyProdData = old_product->copyProdData;
    new_product->freeProdData = old_product->freeProdData;
    new_product->getProdPrice = old_product->getProdPrice;

    new_product->customData = old_product->copyProdData(old_product->customData);

    new_product->id = old_product->id;
    new_product->name = malloc(strlen(old_product->name) + 1);
    strcpy(new_product->name, old_product->name);

    return new_product;
}

void productDelete(void *product)
{
    if (product == NULL)
        return;
    Product cur_product = (Product)product;
    cur_product->freeProdData(cur_product->customData);
    free(cur_product->name);

    return;
}

Product productCreate(const unsigned int id, const char *name,
                      const double amount, const MatamikyaAmountType amountType,
                      const MtmProductData customData, MtmCopyData copyData,
                      MtmFreeData freeData, MtmGetProductPrice prodPrice)
{
    Product new_product = malloc(sizeof(*new_product));
    if (new_product == NULL)
        return NULL;

    new_product->amount = amount;
    new_product->amountType = amountType;
    new_product->copyProdData = copyData;
    new_product->freeProdData = freeData;
    new_product->getProdPrice = prodPrice;

    new_product->customData = copyData(customData);

    new_product->id = id;
    new_product->name = malloc(strlen(name) + 1);
    if (new_product->name == NULL)
    {
        productDelete(new_product);
        return NULL;
    }

    strcpy(new_product->name, name);

    return new_product;
}
int productCompare(void *prod1, void *prod2)
{
    if (((Product)prod1)->id == ((Product)prod2)->id)
        return 0;

    return strcmp(((Product)prod1)->name, ((Product)prod2)->name);
}
int productGetId(Product product)
{
    if (product == NULL)
        return PRODUCT_NULL_ARG;

    return product->id;
}

int productGetProfit(Product product)
{
    if (product == NULL)
        return PRODUCT_NULL_ARG;

    return product->profit;
}