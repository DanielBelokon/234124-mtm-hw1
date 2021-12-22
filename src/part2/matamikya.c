#include "matamikya.h"
#include "amount_set.h"
#include "set.h"

struct Matamikya_t
{
    int id;
    AmountSet products;
    AmountSet product_profits; // by ID
    Set orders;
};

struct Order_t
{
    unsigned int id;
    AmountSet product_ids; // by ID
} * Order;

struct Product_t
{
    unsigned int id;
    char *name;
    MatamikyaAmountType amountType;
    MtmProductData customData;
    MtmCopyData copyData;
    MtmFreeData freeData;
    MtmGetProductPrice prodPrice;
} * Product;

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

// ORDERS

unsigned int mtmCreateNewOrder(Matamikya matamikya)
{
}

MatamikyaResult mtmChangeProductAmountInOrder(Matamikya matamikya, const unsigned int orderId,
                                              const unsigned int productId, const double amount)
{
}

MatamikyaResult mtmChangeProductAmountInOrder(Matamikya matamikya, const unsigned int orderId,
                                              const unsigned int productId, const double amount)
{
}

MatamikyaResult mtmShipOrder(Matamikya matamikya, const unsigned int orderId)
{
}

MatamikyaResult mtmCancelOrder(Matamikya matamikya, const unsigned int orderId)
{
}

MatamikyaResult mtmPrintOrder(Matamikya matamikya, const unsigned int orderId, FILE *output)
{
}

MatamikyaResult mtmPrintBestSelling(Matamikya matamikya, FILE *output)
{
}