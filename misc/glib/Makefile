CFLAGS= -Wall $(shell pkg-config --cflags glib-2.0)
LDFLAGS= $(shell pkg-config --libs glib-2.0)

# 使用对应的.c文件,如果没有的话则需要写相应的规则
PROGS =	queue-test example01 example02

all:	$(PROGS)

# 这里演示: 程序名和文件名不一致的需要单独写编译
queue-test: queue.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^

clean:
	rm -f $(PROGS) *.o a.out
