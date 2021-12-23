#ifndef MATAMIKYA_PRODUCT_H_
#define MATAMIKYA_PRODUCT_H_

typedef struct Product_t *Product;
struct Product_t
{
    unsigned int id;
    char *name;
    MatamikyaAmountType amountType;
    MtmProductData customData;
    int amount;

    MtmCopyData copyProdData;
    MtmFreeData freeProdData;
    MtmGetProductPrice getProdPrice;
};

void *productCopy(void *from);
void productDelete(void *product);
Product productCreate(const unsigned int id, const char *name,
                      const double amount, const MatamikyaAmountType amountType,
                      const MtmProductData customData, MtmCopyData copyData,
                      MtmFreeData freeData, MtmGetProductPrice prodPrice);
int productCompare(void *, void *);
int productGetId(Product product);
int productGetProfit(Product product);
int productAddItem(Product product, int id);
int productRemoveItem(Product product, int id);

#endif