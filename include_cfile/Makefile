CC=gcc
COMPILE.c=$(CC) $(CFLAGS) $(CPPFLAGS) -c
LINK.c=$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
CFLAGS= -Wall

PROGS =	incl unincl

all:	$(PROGS)

imain:main.c
	$(CC) $(CFLAGS) -DINCLUDE_CFILE_DIRECT -o $@ -c $<

# local static function should be call in local module
# if not, warnning will be issued
ifoo:foo.c
	$(CC) -DINCLUDE_CFILE_DIRECT -o $@ -c $<

umain:main.c
	$(CC) $(CFLAGS) -o $@ -c $<

ufoo:foo.c
	$(CC) $(CFLAGS) -o $@ -c $<

unincl:umain ufoo
	$(CC) -o $@ $^

incl:imain ifoo
	$(CC) -o $@ $^

clean:
	rm -f $(PROGS) *.o imain ifoo umain ufoo
