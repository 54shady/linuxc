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
	size_t map_len;
	void *start_addr = 0;
	struct people *tmp_po;
	struct stat st;

	fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd < 0)
	{
		perror("open file error\n");
		exit(-1);
	}

	/* 设置文件大小为5个数据结构大小 */
	map_len = lseek(fd, sizeof(struct people) * PEOPLE_IN_FILE_NR, SEEK_END);
	write(fd, "\0", 1);
	lseek(fd, 0, SEEK_SET);

	fstat(fd, &st);
	printf("filesize = %lu\n", st.st_size);

#ifdef MAP_SIZE_LARGE_THEN_FILE
	/*
	 * try to map 5 more people struct size
	 * map will not change the file size
	 */
	mapped_mem = mmap(start_addr, sizeof(struct people) * 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
#else
	/* read write, private */
	mapped_mem = mmap(start_addr, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
#endif
	fstat(fd, &st);
	printf("filesize = %lu\n", st.st_size);

	tmp_po = (struct people *)mapped_mem;

	/* now, file can be close safety */
	close(fd);

	/* write the map */
	for (i = 0; i < PEOPLE_IN_FILE_NR; i++, tmp_po++)
	{
		tmp_po->age = i + 11;
		strcpy(tmp_po->name, "anonymous");
	}

	/* unmape the file */
	munmap(mapped_mem, map_len);

	return 0;
}
