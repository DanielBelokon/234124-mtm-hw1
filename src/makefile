CC = gcc
AS_STR_OBJS = amount_set_str.o amount_set_str_tests.o amount_set_str_main.o
MTMIKYA_OBJS = matamikya.o matamikya_tests.o matamikya_product.o matamikya_order.o matamikya_print.o matamikya_main.o
LIB_FLAG = -L. -las -lmtm
DEBUG_FLAG = -g
COMP_FLAG = -std=c99 -Wall -Werror -pedantic-errors

# MATAMIKYA 

matamikya: $(MTMIKYA_OBJS)
	$(CC) $(DEBUG_FLAG) $(MTMIKYA_OBJS) $(LIB_FLAG) -no-pie -o $@

matamikya.o: matamikya.c matamikya.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

matamikya_product.o: matamikya_product.c matamikya_product.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

matamikya_order.o: matamikya_order.c matamikya_order.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

matamikya_print.o: matamikya_print.c matamikya_print.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

matamikya_tests.o: tests/matamikya_tests.c tests/matamikya_tests.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) tests/$*.c

matamikya_main.o: tests/matamikya_main.c
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) tests/$*.c

# AMOUNT SET STR

amount_set_str : $(AS_STR_OBJS)
	$(CC) $(DEBUG_FLAG) $(AS_STR_OBJS) -o $@

amount_set_str.o: amount_set_str.c amount_set_str.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

amount_set_str_main.o: amount_set_str_main.c amount_set_str_tests.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

amount_set_str_tests.o: amount_set_str_tests.c amount_set_str.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

clean:
	rm -f $(OBJS) $(AS_STR_OBJS) $(MTMIKYA_OBJS) $(EXEC)
