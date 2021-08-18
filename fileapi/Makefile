CC=gcc
CFLAGS= -Wall

PROGS =	char_read char_write miss_file string_read \
	string_write structured_ascii_read \
	structured_ascii_write structured_bin_read_nonseq \
	structured_bin_write byteio

all:	$(PROGS)

byteio:byte_io.o
	$(CC) $(CFLAGS) -o $@ $^

char_read:char_read.o
	$(CC) $(CFLAGS) -o $@ $^

char_write:char_write.o
	$(CC) $(CFLAGS) -o $@ $^

miss_file:miss_file.o
	$(CC) $(CFLAGS) -o $@ $^

string_read:string_read.o
	$(CC) $(CFLAGS) -o $@ $^

string_write:string_write.o
	$(CC) $(CFLAGS) -o $@ $^

structured_ascii_read:structured_ascii_read.o
	$(CC) $(CFLAGS) -o $@ $^

structured_ascii_write:structured_ascii_write.o
	$(CC) $(CFLAGS) -o $@ $^

structured_bin_read_nonseq:structured_bin_read_nonseq.o
	$(CC) $(CFLAGS) -o $@ $^

structured_bin_write:structured_bin_write.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(PROGS) *.o *.txt *.bin iotestfile
