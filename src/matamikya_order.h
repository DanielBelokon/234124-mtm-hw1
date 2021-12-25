#ifndef MATAMIKYA_ORDER_H_
#define MATAMIKYA_ORDER_H_
#include "matamikya.h"
#include "amount_set.h"

#define ORDER_NULL_ARG -1;

typedef struct Order_t *Order;
struct Order_t
{
    unsigned int id;
    AmountSet products;
};

void *orderCopy(void *from);
void orderDelete(void *order);
Order orderCreate(unsigned int id);
int orderCompare(void *, void *);
int orderGetId(Order order);
int orderAddItem(Order order, unsigned int id);
int orderRemoveItem(Order order, unsigned int id);
AmountSetResult orderChangeItemAmount(Order order, unsigned int id, double amount);

#endif