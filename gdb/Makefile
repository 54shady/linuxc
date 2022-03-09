CC=gcc
CFLAGS= -Wall -g -fno-omit-frame-pointer
#CFLAGS= -Wall -gdwarf-2 -fno-omit-frame-pointer
CFLAGS3= -Wall -g -fno-omit-frame-pointer -DBUG_FIX
#CFLAGS= -Wall -g -fno-omit-frame-pointer -DNO_ERROR_MAKER
OBJCOPY=objcopy
STRIP=strip

PROGS =	gout1 gout2 gout3 gout4 gout5 gout6 \
		gout7 gout8 gout9 gouta goutb goutc \
		goutd goute goutf

all:	$(PROGS)

gout1: test1.o
	$(CC) $(CFLAGS) -o $@ $^

gout2: test2.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

# dependece should be a C file instead of an object here
# dependece is C file result as below:
# gcc -Wall -g -fno-omit-frame-pointer -DBUG_FIX -o gout3 test3.c -lpthread
#
# dependece object file result as below(the marco will not take effict):
# gcc -Wall -g -fno-omit-frame-pointer   -c -o test3.o test3.c
# gcc -Wall -g -fno-omit-frame-pointer -DBUG_FIX -o gout3 test3.o -lpthread
gout3: test3.c
	$(CC) $(CFLAGS3) -o $@ $^ -lpthread

gout4 : main.o foo.o bar.o
	$(CC) $(CFLAGS) -o $@ $^

gout5 : set_var_return.o
	$(CC) $(CFLAGS) -o $@ $^

gout6 : convenience.o
	$(CC) $(CFLAGS) -o $@ $^

gout7 : variables.o
	$(CC) $(CFLAGS) -o $@ $^

gout8: astall.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

gout9: multi_thread.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

gouta: signal_test.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

goutb: recursion.o
	$(CC) $(CFLAGS) -o $@ $^

goutc: multi_process.c
	$(CC) $(CFLAGS) -o $@ $^ -lpthread -D_GNU_SOURCE

goutd: hello.o
	$(CC) $(CFLAGS) -o $@ $^
	$(OBJCOPY) --only-keep-debug $@ objcopy_$@.symbols
	$(STRIP) --only-keep-debug $@ -o strip_$@.symbols
	$(STRIP) $@

goute: struct_display.o
	$(CC) $(CFLAGS) -o $@ $^

goutf: call_function.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(PROGS) *.o core*
