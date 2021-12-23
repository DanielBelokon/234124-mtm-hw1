#ifndef MATAMIKYA_ORDER_H_
#define MATAMIKYA_ORDER_H_

#define ORDER_NULL_ARG -1;

typedef struct Order_t *Order;
struct Order_t
{
    unsigned int id;
    AmountSet products;
};

void *orderCopy(void *from);
void orderDelete(void *order);
Order orderCreate(int id);
int orderCompare(void *, void *);
int orderGetId(Order order);
int orderAddItem(Order order, int id);
int orderRemoveItem(Order order, int id);
int orderChangeItemAmount(Order order, int id, double amount);

#endif