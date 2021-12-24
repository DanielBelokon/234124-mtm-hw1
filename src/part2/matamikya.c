#include <stdlib.h>
#include "matamikya.h"
#include "matamikya_order.h"
#include "matamikya_product.h"
#include "stdio.h"
#include "matamikya_print.h"
#include "amount_set.h"
#include "set.h"
#include "list.h"

struct Matamikya_t
{
    List products;
    List orders;
    int order_index;
};

Product getProductById(Matamikya matamikya, int id)
{
    LIST_FOREACH(Product, product, matamikya->products)
    {
        if (productGetId(product) == id)
            return product;
    }

    return NULL;
}

Order getOrderById(Matamikya matamikya, int id)
{
    LIST_FOREACH(Order, order, matamikya->orders)
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

    List orders = listCreate(orderCopy, orderDelete);
    if (orders == NULL)
    {
        free(new_matamikya);
        return NULL;
    }

    List products = listCreate(productCopy, productDelete);
    if (products == NULL)
    {
        free(new_matamikya);
        listDestroy(orders);
        return NULL;
    }

    new_matamikya->orders = orders;
    new_matamikya->products = products;
    new_matamikya->order_index = 1;

    return new_matamikya;
}

void matamikyaDestroy(Matamikya matamikya)
{
    if (matamikya == NULL)
        return;

    listClear(matamikya->products);
    listClear(matamikya->orders);
    listDestroy(matamikya->products);
    listDestroy(matamikya->orders);

    free(matamikya);
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
    listInsertLast(matamikya->products, new_product);

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

    LIST_FOREACH(Order, order, matamikya->orders)
    {
        orderRemoveItem(order, id);
    }

    if (listRemoveCurrent(matamikya->products) != LIST_SUCCESS)
        return -1;

    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmPrintBestSelling(Matamikya matamikya, FILE *output)
{
    if (matamikya == NULL)
        return MATAMIKYA_NULL_ARGUMENT;
    fprintf(output, "Best Selling Product:\n");
    int max_profits = 0;
    Product profitable_product = NULL;

    LIST_FOREACH(Product, product, matamikya->products)
    {
        int cur_product_profits = 0;
        if (max_profits < (cur_product_profits = productGetProfit(product)))
        {
            max_profits = cur_product_profits;
            profitable_product = product;
        }
    }
    if (profitable_product == NULL)
        fprintf(output, "none\n");
    else
        mtmPrintIncomeLine(profitable_product->name, profitable_product->id, max_profits, output);

    return MATAMIKYA_SUCCESS;
}

// ORDERS

unsigned int mtmCreateNewOrder(Matamikya matamikya)
{
    if (matamikya == NULL)
        return 0;

    int index = matamikya->order_index++;
    Order order = orderCreate(index);
    if (order == NULL)
        return 0;

    ListResult result = listInsertLast(matamikya->orders, order);
    if (result != LIST_SUCCESS)
        return 0;

    orderDelete(order);
    return index;
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
    if (result != AS_SUCCESS)
        return MATAMIKYA_OUT_OF_MEMORY;
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
    AS_FOREACH(unsigned int *, product_id, order->products)
    {
        Product product = getProductById(matamikya, *product_id);
        double amount_out = 0;
        if (product == NULL)
        {
            AS_FOREACH(unsigned int *, product_restore, order->products)
            {
                if (product_restore == product_id)
                    break;
                asGetAmount(order->products, product_id, &amount_out);
                mtmChangeProductAmount(matamikya, *product_id, -amount_out);

                product->profit -= product->getProdPrice(product->customData, amount_out);
            }
        }

        asGetAmount(order->products, product_id, &amount_out);
        mtmChangeProductAmount(matamikya, *product_id, amount_out);

        product->profit += product->getProdPrice(product->customData, amount_out);
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

    listRemoveCurrent(matamikya->orders);

    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmPrintOrder(Matamikya matamikya, const unsigned int orderId, FILE *output)
{
    if (matamikya == NULL)
        return MATAMIKYA_NULL_ARGUMENT;

    Order order = getOrderById(matamikya, orderId);

    if (order == NULL)
        return MATAMIKYA_ORDER_NOT_EXIST;

    mtmPrintOrderHeading(order->id, output);

    double totalPrice = 0;
    AS_FOREACH(unsigned int *, productId, order->products)
    {
        double amount = 2;
        asGetAmount(order->products, productId, &amount);
        Product cur_product = getProductById(matamikya, *productId);

        double product_price = cur_product->getProdPrice(cur_product->customData, amount);
        totalPrice += product_price;
        mtmPrintProductDetails(cur_product->name, cur_product->id, amount, product_price, output);
    }

    mtmPrintOrderSummary(totalPrice, output);
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmPrintInventory(Matamikya matamikya, FILE *output)
{
    listSort(matamikya->products, productCompare);
    fprintf(output, "Inventory Status:\n");
    LIST_FOREACH(Product, product, matamikya->products)
    {
        double product_price = product->getProdPrice(product->customData, 1);
        double amount = product->amount;
        mtmPrintProductDetails(product->name, product->id, amount, product_price, output);
    }

    return MATAMIKYA_SUCCESS;
}