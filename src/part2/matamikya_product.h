#ifndef MATAMIKYA_PRODUCT_H_
#define MATAMIKYA_PRODUCT_H_

typedef struct Product_t *Product;

Product productCopy(Product from, int new_id);
Product productDelete(Product product);
Product productCreate(int id);
Product productCompare(Product, Product);
int productGetId(Product product);
int productGetProfit(Product product);
int productAddItem(Product product, int id);
int productRemoveItem(Product product, int id);

#endif