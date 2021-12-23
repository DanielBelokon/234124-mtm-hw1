#include <stdlib.h>
#include "matamikya_order.h"
#include "amount_set.h"

struct Order_t
{
    unsigned int id;
    AmountSet product_ids; // by ID
};

Order orderCopy(Order from, int new_id)
{
}

Order orderDelete(Order order)
{
}

Order orderCreate(int id)
{
}

int orderAddItem(Order order, int id)
{
}

int orderRemoveItem(Order order, int id)
{
}

int orderGetId(Order order)
{
}