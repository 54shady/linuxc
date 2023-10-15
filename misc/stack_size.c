#include <stdio.h>
#include <unistd.h>

int i = 0;

void foo()
{
	int buff[256]; /* 256 * 4byte = 1024byte = 1kb*/

	printf("i = %d\n", i);
	i++;
	foo();
}

/*
 * 本测试用例运行在环境ulimit -s为8192的情况下
 *
 * 1. 程序无限调用foo
 * 每调用一次foo都会消耗stack上1kb的大小用于buff的分配
 * 2. 程序运行到大概i约等于7~8千左右崩溃(因为stack不够大了)
 * 所以说明程序的stack大小在8M左右和(ulimit -s来查看的结果吻合)
 *
 * 使用gdb调试, gcc -g stack_size.c
 * 1. 配置ulimit使程序产生coredump文件
 * ulimit -c unlimited
 * 2. 使用gdb加载coredump
 * gdb ./a.out core
 *
 * (gdb) bt
 * ...
 * #7939 0x00005555555551a8 in foo () at stack_size.c:12
 * #7940 0x00005555555551d8 in main (argc=1, argv=0x7fffffffddc8) at stack_size.c:25
 *
 * 可以看到backtrace的最开始一帧是7940,下面计算下整个调用栈sp移动的大小
 *
 * (gdb) frame 0
 * #0  0x00007ffff7e28097 in ?? () from /lib64/libc.so.6
 * (gdb) info registers sp
 * sp             0x7fffff7fee10      0x7fffff7fee10
 * (gdb) frame 7940
 * #7940 0x00005555555551d8 in main (argc=1, argv=0x7fffffffddc8) at stack_size.c:25
 * (gdb) info registers sp
 * sp             0x7fffffffdc80      0x7fffffffdc80
 *
 * (0x7fffffffdc80 - 0x7fffff7fee10) / 1024 = 8187.609375
 * 接近8M大小的栈空间了
 */
int main(int argc, char *argv[])
{
	foo();
	sleep(100);

	return 0;
}
