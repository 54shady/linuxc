#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "lnxc.h"

int print_usage(const char *name)
{
	printf("%s <text_file>\n", name);
	return -1;
}

int main(int argc, char *argv[])
{
	int fd;
	char *mapped_mem;
	size_t map_len = 1024;
	void *start_addr = 0;

	if (argc != 2)
		return print_usage(argv[0]);

	fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd < 0)
		handle_error("Open file error");

	/* read only, private */
	mapped_mem = mmap(start_addr, map_len, PROT_READ, MAP_PRIVATE, fd, 0);
	if (mapped_mem == MAP_FAILED)
		handle_error("mapp error");

	/* now, file can be close safety */
	close(fd);

	/* read from map */
	printf("%s\n", mapped_mem);

	/* unmape the file */
	munmap(mapped_mem, map_len);

	return 0;
}
