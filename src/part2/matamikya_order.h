#ifndef MATAMIKYA_ORDER_H_
#define MATAMIKYA_ORDER_H_

typedef struct Order_t *Order;
Order orderCopy(Order from, int new_id);
Order orderDelete(Order order);
Order orderCreate(int id);
Order orderCompare(Order, Order);
int orderGetId(Order order);
int orderAddItem(Order order, int id);
int orderRemoveItem(Order order, int id);

#endif