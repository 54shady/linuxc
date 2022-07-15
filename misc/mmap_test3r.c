#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mmap_test.h"

#define PEOPLE_IN_FILE_NR 5

int print_usage(const char *name)
{
	printf("%s <text_file>\n", name);
	return -1;
}

int main(int argc, char *argv[])
{
	int i;
	int fd;
	char *mapped_mem;
	void *start_addr = 0;
	struct people *tmp_po;

	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		perror("open file error\n");
		exit(-1);
	}

	/* read only */
	mapped_mem = mmap(start_addr, sizeof(struct people) * PEOPLE_IN_FILE_NR, PROT_READ, MAP_SHARED, fd, 0);
	tmp_po = (struct people *)mapped_mem;

	/* now, file can be close safety */
	close(fd);

	/* write the map */
	for (i = 0; i < PEOPLE_IN_FILE_NR; i++, tmp_po++)
		printf("%s : age %d\n", tmp_po->name, tmp_po->age);

	/* unmape the file */
	munmap(mapped_mem, sizeof(struct people) * PEOPLE_IN_FILE_NR);

	return 0;
}
