#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#define N_ELEMENT 4

void set_value(int *addr, int value)
{
	*addr = value;
}

void get_value(int *addr, int value)
{
	printf("%p = %d(%d) \n", addr, *addr, value);
}

void traverse_array(void *p, int len, void (*fn)(int *addr, int value))
{
	int *tmp;
	int i;

	tmp = (int *)p;
	for (i = 0; i < len; i++)
		fn(tmp++, i+99);
	printf("\n");
}

int main(int argc, char *argv[])
{
	int *p;

	/* 分配N字节的内存空间 */
	p = g_malloc0(sizeof(int) * N_ELEMENT);
	if (!p)
		return -1;

	/* 操作内存空间 */
	traverse_array(p, N_ELEMENT, set_value);
	traverse_array(p, N_ELEMENT, get_value);

	/* 释放 */
	g_free(p);

	return 0;
}
