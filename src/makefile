CC = gcc
AS_STR_OBJS = amount_set_str.o amount_set_str_tests.o amount_set_str_main.o
MTMIKYA_OBJS = matamikya.o  matamikya_product.o matamikya_order.o matamikya_print.o tests/matamikya_main.o tests/matamikya_tests.o
MTM_EXE = matamikya
AS_EXE = amount_set_str
LIB_FLAG = -L. -las -lmtm
DEBUG_FLAG = -g
COMP_FLAG = -std=c99 -Wall -Werror -pedantic-errors

# Generic rule

%.o: %.c
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $< -o $@

# MATAMIKYA 

$(MTM_EXE): $(MTMIKYA_OBJS)
	$(CC) $(DEBUG_FLAG) $(MTMIKYA_OBJS) $(LIB_FLAG) -no-pie -o $@

matamikya.o: matamikya.c matamikya.h
matamikya_order.o: matamikya_order.c matamikya_order.h
matamikya_print.o: matamikya_print.c matamikya_print.h
matamikya_product.o: matamikya_product.c matamikya_product.h

tests/%.o: tests/%.c
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $< -o $@

tests/matamikya_tests.o: tests/matamikya_tests.c tests/matamikya_tests.h
tests/matamikya_main.o: tests/matamikya_main.c

# AMOUNT SET STR

$(AS_EXE) : $(AS_STR_OBJS)
	$(CC) $(DEBUG_FLAG) $(AS_STR_OBJS) -o $@

amount_set_str.o: amount_set_str.c amount_set_str.h
amount_set_str_main.o: amount_set_str_main.c amount_set_str_tests.h
amount_set_str_tests.o: amount_set_str_tests.c amount_set_str.h

clean:
	rm -f $(OBJS) $(AS_STR_OBJS) $(MTMIKYA_OBJS)
