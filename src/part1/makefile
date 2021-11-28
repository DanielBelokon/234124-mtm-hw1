CC = gcc
OBJS = amount_set_str.o amount_set_str_tests.o
EXEC = AmountSet
DEBUG_FLAG = -g
COMP_FLAG = -std=c99 -Wall -Werror

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@
amount_set_str.o: amount_set_str.c amount_set_str.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
amount_set_str_tests.o: amount_set_str_tests.c amount_set_str.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

clean:
	rm -f $(OBJS) $(EXEC)
