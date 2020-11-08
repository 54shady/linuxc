# CROSS_COMPILE=aarch64-linux-gnu-
CROSS_COMPILE :=
CC=$(CROSS_COMPILE)gcc
COMPILE.c=$(CC) $(CFLAGS) $(CPPFLAGS) -c
LINK.c=$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
CFLAGS= -Wall -lpthread

PROGS = accountest detach ignore_sig sig2main\
		pj1 pj2 race locktype rwlock_feature \
		conditionvar conditionvar2 interactive_rw \
		thread_args_pitfall toncpu tls

all:	$(PROGS)

tls:	tls.c
		$(CC) -o $@ $^ $(CFLAGS)

toncpu:	thread_oncpu.c
		$(CC) -o $@ $^ $(CFLAGS) -D_GNU_SOURCE

accountest:	account.o account_test.o
		$(CC) -o $@ $^ $(CFLAGS)

detach:	detach.o
		$(CC) -o $@ $^ $(CFLAGS)

ignore_sig: main_ignore_sub_sig.o
		$(CC) -o $@ $^ $(CFLAGS)

sig2main:	sub_sig2_main.o
		$(CC) -o $@ $^ $(CFLAGS)

pj2:	pjoin2.o
		$(CC) -o $@ $^ $(CFLAGS)

pj1:	pjoin.o
		$(CC) -o $@ $^ $(CFLAGS)

race:	race.o
		$(CC) -o $@ $^ $(CFLAGS)

locktype:	lock_type.o
		$(CC) -o $@ $^ $(CFLAGS)

rwlock_feature:	rwlock_feature.o
		$(CC) -o $@ $^ $(CFLAGS)

conditionvar:	condition.o
		$(CC) -o $@ $^ $(CFLAGS)

conditionvar2:	condition2.o
		$(CC) -o $@ $^ $(CFLAGS)

interactive_rw:	interactive_rw.o
		$(CC) -o $@ $^ $(CFLAGS)

thread_args_pitfall: thread_args_pitfall.o
		$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(PROGS) *.o a.out
