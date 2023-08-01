#include <stdio.h>

static int g_cnt = 0;

void do_something(int time)
{
	printf("%s, %d[%d]\n", __FUNCTION__, __LINE__, time);
}

int main(int argc, char *argv[])
{
	int i, j;

	/*
	 * (gdb) display i
	 * (gdb) display j
	 */
	for (i = 0, j = 100; i < 20 && j > 50; i++, j--)
	{
		do_something(g_cnt);
		g_cnt++; /* do a break here */
	}

	return 0;
}
