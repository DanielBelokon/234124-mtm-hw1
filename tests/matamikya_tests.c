#include "matamikya_tests.h"
#include "../matamikya.h"
#include "test_utilities.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#define INVENTORY_OUT_FILE "tests/printed_inventory.txt"
#define INVENTORY_TEST_FILE "tests/expected_inventory.txt"
#define ORDER_OUT_FILE "tests/printed_order.txt"
#define ORDER_TEST_FILE "tests/expected_order.txt"
#define BEST_SELLING_OUT_FILE "tests/printed_best_selling.txt"
#define BEST_SELLING_TEST_FILE "tests/expected_best_selling.txt"
#define NO_SELLING_OUT_FILE "tests/printed_no_selling.txt"
#define NO_SELLING_TEST_FILE "tests/expected_no_selling.txt"

#define ASSERT_OR_DESTROY(expr) ASSERT_TEST_WITH_FREE((expr), matamikyaDestroy(mtm))

bool testCreate() {
    Matamikya mtm = matamikyaCreate();
    ASSERT_TEST(mtm != NULL);
    matamikyaDestroy(mtm);
    return true;
}

bool testDestroy() {
    Matamikya mtm = matamikyaCreate();
    matamikyaDestroy(mtm);
    /* try to elicit undefined behavior in case matamikyaDestroy has bugs */
    Matamikya mtm2 = matamikyaCreate();
    ASSERT_TEST(mtm2 != NULL);
    matamikyaDestroy(mtm2);
    return true;
}

static MtmProductData copyDouble(MtmProductData number) {
    double *copy = malloc(sizeof(*copy));
    if (copy) {
        *copy = *(double*)number;
    }
    return copy;
}

static void freeDouble(MtmProductData number) {
    free(number);
}

static double simplePrice(MtmProductData basePrice, const double amount) {
    return (*(double*)basePrice) * amount;
}

static double buy10Get10ForFree(MtmProductData basePrice, const double amount) {
    double realAmount = amount;
    if (amount >= 20) {
        realAmount -= 10;
    } else if (amount >= 10) {
        realAmount = 10;
    }
    return simplePrice(basePrice, realAmount);
}

bool testModifyProducts() {
    Matamikya mtm = matamikyaCreate();
    double basePrice = 8.9;
    ASSERT_OR_DESTROY(MATAMIKYA_SUCCESS ==
                      mtmNewProduct(mtm, 4, "Tomato", 2019.11, MATAMIKYA_ANY_AMOUNT,
                                    &basePrice, copyDouble, freeDouble, simplePrice));
    basePrice = 5.8;
    ASSERT_OR_DESTROY(MATAMIKYA_SUCCESS ==
                      mtmNewProduct(mtm, 6, "Onion", 1789.75, MATAMIKYA_ANY_AMOUNT,
                                    &basePrice, copyDouble, freeDouble, buy10Get10ForFree));
    basePrice = 2000;
    ASSERT_OR_DESTROY(MATAMIKYA_SUCCESS ==
                      mtmNewProduct(mtm, 10, "Television", 15, MATAMIKYA_INTEGER_AMOUNT,
                                    &basePrice, copyDouble, freeDouble, simplePrice));
    basePrice = 5000;
    ASSERT_OR_DESTROY(MATAMIKYA_SUCCESS ==
                      mtmNewProduct(mtm, 11, "Television", 4, MATAMIKYA_INTEGER_AMOUNT,
                                    &basePrice, copyDouble, freeDouble, simplePrice));

    ASSERT_OR_DESTROY(MATAMIKYA_SUCCESS == mtmClearProduct(mtm, 6));
    ASSERT_OR_DESTROY(MATAMIKYA_SUCCESS == mtmChangeProductAmount(mtm, 4, 30.5));
    ASSERT_OR_DESTROY(MATAMIKYA_SUCCESS == mtmChangeProductAmount(mtm, 4, -12.85));
    ASSERT_OR_DESTROY(MATAMIKYA_INVALID_AMOUNT == mtmChangeProductAmount(mtm, 10, 2.25));
    ASSERT_OR_DESTROY(MATAMIKYA_INSUFFICIENT_AMOUNT == mtmChangeProductAmount(mtm, 10, -1000));

    basePrice = 18.5;
    ASSERT_OR_DESTROY(MATAMIKYA_INVALID_AMOUNT ==
                      mtmNewProduct(mtm, 7, "Watermelon", 24.54,
                                    MATAMIKYA_HALF_INTEGER_AMOUNT, &basePrice, copyDouble,
                                    freeDouble, simplePrice));
    ASSERT_OR_DESTROY(MATAMIKYA_SUCCESS ==
                      mtmNewProduct(mtm, 7, "Watermelon", 24.5,
                                    MATAMIKYA_HALF_INTEGER_AMOUNT,
                                    &basePrice, copyDouble,
                                    freeDouble, simplePrice));

    matamikyaDestroy(mtm);
    return true;
}

static void makeInventory(Matamikya mtm) {
    double basePrice = 8.9;
    mtmNewProduct(mtm, 4, "Tomato", 2019.11, MATAMIKYA_ANY_AMOUNT, &basePrice, copyDouble,
                  freeDouble, simplePrice);
    basePrice = 5.8;
    mtmNewProduct(mtm, 6, "Onion", 1789.75, MATAMIKYA_ANY_AMOUNT, &basePrice, copyDouble,
                  freeDouble, buy10Get10ForFree);
    basePrice = 2000;
    mtmNewProduct(mtm, 10, "Television", 15, MATAMIKYA_INTEGER_AMOUNT, &basePrice,
                  copyDouble, freeDouble, simplePrice);
    basePrice = 5000;
    mtmNewProduct(mtm, 11, "Smart TV", 4, MATAMIKYA_INTEGER_AMOUNT, &basePrice,
                  copyDouble, freeDouble, simplePrice);
    basePrice = 18.5;
    mtmNewProduct(mtm, 7, "Watermelon", 24.5, MATAMIKYA_HALF_INTEGER_AMOUNT, &basePrice,
                  copyDouble, freeDouble, simplePrice);
}

bool testModifyOrders() {
    Matamikya mtm = matamikyaCreate();
    makeInventory(mtm);

    unsigned int order1 = mtmCreateNewOrder(mtm);
    ASSERT_OR_DESTROY(order1 > 0);
    ASSERT_OR_DESTROY(MATAMIKYA_SUCCESS ==
                      mtmChangeProductAmountInOrder(mtm, order1, 11, 1.0));
    ASSERT_OR_DESTROY(MATAMIKYA_PRODUCT_NOT_EXIST ==
                      mtmChangeProductAmountInOrder(mtm, order1, 15, 1.0));
    ASSERT_OR_DESTROY(MATAMIKYA_INVALID_AMOUNT ==
                      mtmChangeProductAmountInOrder(mtm, order1, 11, 1.2));

    unsigned int order2 = mtmCreateNewOrder(mtm);
    ASSERT_OR_DESTROY(order2 > 0);
    ASSERT_OR_DESTROY(MATAMIKYA_SUCCESS ==
                      mtmChangeProductAmountInOrder(mtm, order2, 6, 10.25));
    ASSERT_OR_DESTROY(MATAMIKYA_SUCCESS ==
                      mtmChangeProductAmountInOrder(mtm, order2, 7, 1.5));
    ASSERT_OR_DESTROY(MATAMIKYA_SUCCESS == mtmCancelOrder(mtm, order1));
    ASSERT_OR_DESTROY(MATAMIKYA_SUCCESS ==
                      mtmChangeProductAmountInOrder(mtm, order2, 10, 2.0));
    ASSERT_OR_DESTROY(MATAMIKYA_SUCCESS == mtmShipOrder(mtm, order2));

    matamikyaDestroy(mtm);
    return true;
}

static bool fileEqual(FILE *file1, FILE *file2) {
    int c1, c2;
    do {
        c1 = fgetc(file1);
        c2 = fgetc(file2);
    } while (c1 != EOF && c2 != EOF && c1 == c2);
    return (c1 == EOF && c2 == EOF);
}

static bool wholeFileEqual(const char *filename1, const char *filename2) {
    FILE *file1 = fopen(filename1, "r");
    FILE *file2 = fopen(filename2, "r");
    assert(file1);
    assert(file2);
    bool result = fileEqual(file1, file2);
    fclose(file1);
    fclose(file2);
    return result;
}

bool testPrintInventory() {
    Matamikya mtm = matamikyaCreate();
    makeInventory(mtm);
    FILE *outputFile = fopen(INVENTORY_OUT_FILE, "w");
    assert(outputFile);
    ASSERT_OR_DESTROY(mtmPrintInventory(mtm, outputFile) == MATAMIKYA_SUCCESS);
    fclose(outputFile);
    ASSERT_OR_DESTROY(wholeFileEqual(INVENTORY_TEST_FILE, INVENTORY_OUT_FILE));
    matamikyaDestroy(mtm);
    return true;
}

static unsigned int makeOrder(Matamikya mtm) {
    makeInventory(mtm);
    unsigned int id = mtmCreateNewOrder(mtm);
    mtmChangeProductAmountInOrder(mtm, id, 6, 10.25);
    mtmChangeProductAmountInOrder(mtm, id, 10, 2);
    mtmChangeProductAmountInOrder(mtm, id, 7, 1.5);
    return id;
}

bool testPrintOrder() {
    Matamikya mtm = matamikyaCreate();
    unsigned int orderId = makeOrder(mtm);
    FILE *outputFile = fopen(ORDER_OUT_FILE, "w");
    assert(outputFile);
    ASSERT_OR_DESTROY(mtmPrintOrder(mtm, orderId, outputFile) == MATAMIKYA_SUCCESS);
    fclose(outputFile);
    FILE *expected = fopen(ORDER_TEST_FILE, "r");
    FILE *printed = fopen(ORDER_OUT_FILE, "r");
    assert(expected);
    assert(printed);

    /* test that prefix and printed orderId is correct */
    const char *expectedPrefix = "Order ";
    int size = strlen(expectedPrefix);
    for (int i = 0; i < size; ++i) {
        ASSERT_OR_DESTROY(fgetc(printed) == expectedPrefix[i]);
    }
    unsigned int printedId;
    fscanf(printed, "%u", &printedId);
    ASSERT_OR_DESTROY(orderId == printedId);

    /* skip the rest of the header line */
    while (fgetc(printed) != '\n');
    while (fgetc(expected) != '\n');

    ASSERT_OR_DESTROY(fileEqual(expected, printed));
    fclose(expected);
    fclose(printed);
    matamikyaDestroy(mtm);
    return true;
}

bool testPrintBestSelling() {
    Matamikya mtm = matamikyaCreate();
    makeInventory(mtm);

    FILE *outputFile = fopen(NO_SELLING_OUT_FILE, "w");
    assert(outputFile);
    ASSERT_OR_DESTROY(mtmPrintBestSelling(mtm, outputFile) == MATAMIKYA_SUCCESS);
    fclose(outputFile);
    ASSERT_OR_DESTROY(wholeFileEqual(NO_SELLING_TEST_FILE, NO_SELLING_OUT_FILE));

    unsigned int order = mtmCreateNewOrder(mtm);
    mtmChangeProductAmountInOrder(mtm, order, 10, 3.0);
    mtmShipOrder(mtm, order);

    order = mtmCreateNewOrder(mtm);
    mtmChangeProductAmountInOrder(mtm, order, 6, 10.25);
    mtmChangeProductAmountInOrder(mtm, order, 7, 1.5);
    mtmChangeProductAmountInOrder(mtm, order, 11, 1.0);
    mtmShipOrder(mtm, order);

    outputFile = fopen(BEST_SELLING_OUT_FILE, "w");
    assert(outputFile);
    ASSERT_OR_DESTROY(mtmPrintBestSelling(mtm, outputFile) == MATAMIKYA_SUCCESS);
    fclose(outputFile);
    ASSERT_OR_DESTROY(wholeFileEqual(BEST_SELLING_TEST_FILE, BEST_SELLING_OUT_FILE));

    matamikyaDestroy(mtm);
    return true;
}
