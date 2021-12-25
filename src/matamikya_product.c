#include "matamikya_product.h"
#include "matamikya.h"
#include <string.h>
#include <stdlib.h>

#define EPSILON 0.001

bool isAmountValid(const double amount, MatamikyaAmountType type)
{
    bool valid = false;

    if (type == MATAMIKYA_ANY_AMOUNT)
        return true;
    double abs_amount = amount < 0 ? -amount : amount;
    double remainder = abs_amount - ((int)abs_amount);

    int round = remainder > 0.5 ? (int)(abs_amount + 1) : (int)abs_amount;
    double epsilon_half = EPSILON + 0.5;

    double diff = abs_amount - round;

    double absdiff = diff < 0 ? -diff : diff;

    if (type == MATAMIKYA_HALF_INTEGER_AMOUNT)
        valid = (absdiff <= epsilon_half && absdiff >= 1 - epsilon_half);

    return (absdiff <= EPSILON) || valid;
}

bool isNameValid(const char *name)
{
    if (name == NULL)
        return false;

    if ((*name <= 'z' && *name >= 'a') ||
        (*name >= '0' && *name <= '9') ||
        (*name >= 'A' && *name <= 'Z'))
        return true;

    return false;
}

MatamikyaResult productChangeAmount(Product product, const double amount)
{
    if (product->amount + amount < -EPSILON)
        return MATAMIKYA_INSUFFICIENT_AMOUNT;

    if (isAmountValid(amount, product->amountType))
        product->amount += amount;
    else
        return MATAMIKYA_INVALID_AMOUNT;

    return MATAMIKYA_SUCCESS;
}

void *productCopy(void *from)
{
    Product new_product = malloc(sizeof(*new_product));
    Product old_product = (Product)from;

    new_product->amountType = old_product->amountType;
    new_product->amount = 0;
    new_product->profit = old_product->profit;

    if (productChangeAmount(new_product, old_product->amount) != MATAMIKYA_SUCCESS)
    {
        free(new_product);
        return NULL;
    }

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

    free(product);
    return;
}

Product productCreate(const unsigned int id, const char *name,
                      const double amount, const MatamikyaAmountType amountType,
                      const MtmProductData customData, MtmCopyData copyData,
                      MtmFreeData freeData, MtmGetProductPrice prodPrice,
                      MatamikyaResult *result)
{
    *result = MATAMIKYA_OUT_OF_MEMORY;
    if (name == NULL ||
        customData == NULL ||
        copyData == NULL ||
        freeData == NULL ||
        prodPrice == NULL)
    {
        *result = MATAMIKYA_NULL_ARGUMENT;
        return NULL;
    }

    if (!isNameValid(name))
    {
        *result = MATAMIKYA_INVALID_NAME;
        return NULL;
    }

    if (amount < 0)
    {
        *result = MATAMIKYA_INVALID_AMOUNT;
        return NULL;
    }

    Product new_product = malloc(sizeof(*new_product));
    if (new_product == NULL)
        return NULL;

    new_product->amountType = amountType;
    new_product->amount = 0;
    new_product->profit = 0;

    if ((*result = productChangeAmount(new_product, amount)) != MATAMIKYA_SUCCESS)
    {
        free(new_product);
        return NULL;
    }
    new_product->copyProdData = copyData;
    new_product->freeProdData = freeData;
    new_product->getProdPrice = prodPrice;

    new_product->customData = copyData(customData);

    new_product->id = id;
    new_product->name = malloc(strlen(name) + 1);
    if (new_product->name == NULL)
    {
        *result = MATAMIKYA_OUT_OF_MEMORY;
        productDelete(new_product);
        return NULL;
    }

    strcpy(new_product->name, name);

    *result = MATAMIKYA_SUCCESS;
    return new_product;
}
int productCompare(void *prod1, void *prod2)
{
    return ((Product)prod1)->id - ((Product)prod2)->id;
}
int productGetId(Product product)
{
    if (product == NULL)
        return PRODUCT_NULL_ARG;

    return product->id;
}

double productGetProfit(Product product)
{
    if (product == NULL)
        return 0;
    return product->profit;
}