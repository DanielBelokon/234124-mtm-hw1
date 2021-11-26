#include "matamikya_print.h"

void mtmPrintProductDetails(const char* name, const unsigned int id, const double amount, const double price, FILE* output){
    fprintf(output,"name: %s, id: %d, amount: %.3f, price: %.3f\n",name, id, amount, price);
}

void mtmPrintOrderHeading(const unsigned int orderId, FILE* output){
    fprintf(output,"Order %d Details:\n", orderId);
}

void mtmPrintOrderSummary(const double totalOrderPrice, FILE* output){
    fprintf(output,"----------\nTotal Price: %.3f\n", totalOrderPrice);
}

void mtmPrintIncomeLine(const char* name, const unsigned int id, const double totalIncome, FILE* output){
    fprintf(output, "name: %s, id: %d, total income: %.3f\n", name, id, totalIncome);
}
