#include <stdlib.h>
#include "matamikya.h"
#include "matamikya_order.h"
#include "matamikya_product.h"
#include "matamikya_print.h"
#include "amount_set.h"
#include "set.h"

struct Matamikya_t
{
    Set products;
    Set orders;
    int order_index;
};

Product getProductById(Matamikya matamikya, int id)
{
    SET_FOREACH(Product, product, matamikya->products)
    {
        if (productGetId(product) == id)
            return product;
    }

    return NULL;
}

Order getOrderById(Matamikya matamikya, int id)
{
    SET_FOREACH(Order, order, matamikya->orders)
    {
        if (orderGetId(order) == id)
            return order;
    }

    return NULL;
}

Matamikya matamikyaCreate()
{
    Matamikya new_matamikya = malloc(sizeof(*new_matamikya));
    if (new_matamikya == NULL)
        return NULL;

    Set orders = setCreate(orderCopy, orderDelete, orderCompare);
    if (orders == NULL)
        return NULL;

    Set products = setCreate(productCopy, productDelete, productCompare);
    if (products == NULL)
    {
        setDestroy(orders);
        return NULL;
    }

    new_matamikya->orders = orders;
    new_matamikya->products = products;

    return new_matamikya;
}

void matamikyaDestroy(Matamikya matamikya)
{
    if (matamikya == NULL)
        return;

    setDestroy(matamikya->products);
    setDestroy(matamikya->orders);

    return;
}

MatamikyaResult mtmNewProduct(Matamikya matamikya, const unsigned int id, const char *name,
                              const double amount, const MatamikyaAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice)
{
    if (matamikya == NULL)
        return MATAMIKYA_NULL_ARGUMENT;

    if (getProductById(matamikya, id))
        return MATAMIKYA_PRODUCT_ALREADY_EXIST;

    MatamikyaResult result;
    Product new_product = productCreate(id,
                                        name,
                                        amount,
                                        amountType,
                                        customData, copyData, freeData,
                                        prodPrice, &result);
    if (new_product == NULL)
        return result;
    setAdd(matamikya->products, new_product);

    // inefficient but w/e, I'll deal with it later
    productDelete(new_product);

    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmChangeProductAmount(Matamikya matamikya, const unsigned int id, const double amount)
{
    if (matamikya == NULL)
        return MATAMIKYA_NULL_ARGUMENT;
    Product product;

    if ((product = getProductById(matamikya, id)) == NULL)
        return MATAMIKYA_PRODUCT_NOT_EXIST;

    return productChangeAmount(product, amount);
}

MatamikyaResult mtmClearProduct(Matamikya matamikya, const unsigned int id)
{
    if (matamikya == NULL)
        return MATAMIKYA_NULL_ARGUMENT;
    Product product;

    if ((product = getProductById(matamikya, id)) == NULL)
        return MATAMIKYA_PRODUCT_NOT_EXIST;

    SET_FOREACH(Order, order, matamikya->orders)
    {
        orderRemoveItem(order, id);
    }

    if (setRemove(matamikya->products, product))
        return -1;

    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmPrintBestSelling(Matamikya matamikya, FILE *output)
{
    if (matamikya == NULL)
        return MATAMIKYA_NULL_ARGUMENT;

    int max_profits = 0;
    Product profitable_product = NULL;

    SET_FOREACH(Product, product, matamikya->products)
    {
        int cur_product_profits = 0;
        if (max_profits < (cur_product_profits = productGetProfit(product)))
        {
            max_profits = cur_product_profits;
            profitable_product = product;
        }
    }
    if (profitable_product == NULL)
        return MATAMIKYA_OUT_OF_MEMORY;
    mtmPrintProductDetails(profitable_product->name, profitable_product->id, profitable_product->amount, 0, output);

    return MATAMIKYA_SUCCESS;
}

// ORDERS

unsigned int mtmCreateNewOrder(Matamikya matamikya)
{
    if (matamikya == NULL)
        return 0;

    int index = matamikya->order_index++;
    Order order = orderCreate(index);
    SetResult result = setAdd(matamikya->orders, order);
    if (result == SET_SUCCESS)
        return index;

    return 0;
    // orderDelete(order);
}

MatamikyaResult mtmChangeProductAmountInOrder(Matamikya matamikya, const unsigned int orderId,
                                              const unsigned int productId, const double amount)
{
    if (matamikya == NULL)
        return MATAMIKYA_NULL_ARGUMENT;

    Order order = getOrderById(matamikya, orderId);

    if (order == NULL)
        return MATAMIKYA_ORDER_NOT_EXIST;

    Product product = getProductById(matamikya, productId);
    if (product == NULL)
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    if (!isAmountValid(amount, product->amountType))
        return MATAMIKYA_INVALID_AMOUNT;

    AmountSetResult result = orderChangeItemAmount(order, productId, amount);

    if (result == AS_INSUFFICIENT_AMOUNT)
        return MATAMIKYA_INSUFFICIENT_AMOUNT;

    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmShipOrder(Matamikya matamikya, const unsigned int orderId)
{
    if (matamikya == NULL)
        return MATAMIKYA_NULL_ARGUMENT;

    Order order = getOrderById(matamikya, orderId);

    if (order == NULL)
        return MATAMIKYA_ORDER_NOT_EXIST;

    // TODO: Keep warehouse unchanged if operation fails
    AS_FOREACH(int *, product_id, order->products)
    {
        double amount_out = 0;
        asGetAmount(order->products, product_id, &amount_out);
        mtmChangeProductAmount(matamikya, *product_id, amount_out);
    }

    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmCancelOrder(Matamikya matamikya, const unsigned int orderId)
{
    if (matamikya == NULL)
        return MATAMIKYA_NULL_ARGUMENT;

    Order order = getOrderById(matamikya, orderId);

    if (order == NULL)
        return MATAMIKYA_ORDER_NOT_EXIST;

    setRemove(matamikya->orders, order);

    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmPrintOrder(Matamikya matamikya, const unsigned int orderId, FILE *output)
{
    return MATAMIKYA_NULL_ARGUMENT;
}

MatamikyaResult mtmPrintInventory(Matamikya matamikya, FILE *output)
{
    return MATAMIKYA_NULL_ARGUMENT;
}