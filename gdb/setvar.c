#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int r;

	srand(time(NULL));

	while (1)
	{
		/* 0-4 */
		r = rand() % 5;

		switch (r) /* 使用gdb在这一行打断点修改r的值 */
		{
			case 0:
				printf("r = %d\n", r);
				break;
			case 1:
				printf("r = %d\n", r);
				break;
			case 2:
				printf("r = %d\n", r);
				break;
			case 3:
				printf("r = %d\n", r);
				break;
			case 4:
				printf("r = %d\n", r);
				break;
			default:
				printf("r = %d\n", r);
				break;
		};

		sleep(1);
	}
	return 0;
}

/*
 * (gdb) b setvar.c:17 if r == 3
 * (gdb) command
 * > set variable r = 911
 * > c
 * > end
 * (gdb) c
 *
 * 通过save命令来保存断点信息
 * (gdb) save breakpoints ~/breakinfo.cfg
 *
 * 下次启动调试可以加载这个断点信息
 * gdb -p `pidof setvar` -x ~/breakinfo.cfg
 *
 * 或是在gdb中source这个断点信息
 * (gdb) source ~/breakinfo.cfg
 */
