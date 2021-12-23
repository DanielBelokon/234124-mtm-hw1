CC = gcc
AS_STR_OBJS = amount_set_str.o amount_set_str_tests.o amount_set_str_main.o
MTMIKYA_OBJS = matamikya.o
DEBUG_FLAG = -g
COMP_FLAG = -std=c99 -Wall -Werror -pedantic-errors

# MATAMIKYA 

matamikya: $(MTMIKYA_OBJS)
	$(CC) $(DEBUG_FLAG) $(MTMIKYA_OBJS) -o $@

matamikya.o: matamikya.c matamikya.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

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
	rm -f $(OBJS) $(AS_STR_OBJS) $(EXEC)
