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
 */
int main(int argc, char *argv[])
{
	foo();
	sleep(100);

	return 0;
}
