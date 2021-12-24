#include <stdlib.h>
#include "matamikya_order.h"
#include "amount_set.h"
#include "matamikya_product.h"

int idCompare(void *a, void *b)
{
    return *(int *)a == *(int *)b;
}

void *idCopy(void *id)
{
    int *new_id = malloc(sizeof(int));
    *new_id = *(int *)id;
    return new_id;
}

void *orderCopy(void *from)
{
    if (from == NULL)
        return NULL;

    Order order = malloc(sizeof(*order));

    order->id = ((Order)from)->id;
    order->products = asCopy(((Order)from)->products);

    return order;
}

void orderDelete(void *order)
{
    if (order == NULL)
        return;
    asDestroy(((Order)order)->products);
    free(order);
}

int orderCompare(void *order1, void *order2)
{
    return (((Order)order1)->id - ((Order)order2)->id);
}

Order orderCreate(int id)
{
    Order order = malloc(sizeof(*order));

    order->id = id;
    order->products = asCreate(idCopy, free, idCompare);

    return order;
}

int orderAddItem(Order order, int id)
{
    if (order == NULL)
        return ORDER_NULL_ARG;

    asRegister(order->products, &id);
    return 0;
}

int orderRemoveItem(Order order, int id)
{
    if (order == NULL)
        return ORDER_NULL_ARG;

    asDelete(order->products, &id);
    return 0;
}

int orderGetId(Order order)
{
    if (order == NULL)
        return ORDER_NULL_ARG;

    return order->id;
}

int orderChangeItemAmount(Order order, int id, double amount)
{
    if (order == NULL)
        return ORDER_NULL_ARG;
    if (!asContains(order->products, &id))
        asRegister(order->products, &id);

    return asChangeAmount(order->products, &(order->id), amount);
}