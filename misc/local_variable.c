#include <stdio.h>
#include <unistd.h>

/*
 *  验证局部变量存储空间的分配和释放
 *  由于 i 未初始化,打印的应该是一个不确定的值
 *  然后把 i 赋值为 911,函数返回,释放 i 的存储空间
 *  第二次调用 foo 函数,分配变量 i 的存储空间,然后打印 i 的值
 *  由于 i 未初始化,如果打印的又是一个不确定的值就证明了
 *  局部变量的存储空间在每次函数调用时分配,在函数返回时释放
 */
static int cnt = 0;

void foo(void)
{
	int i;

	printf("cnt = %d, i = %d\n", cnt++, i);
	i = 911;
}

int main(int argc, char **argv)
{
	int j;
	for (j = 0; j < 100; j++)
	{
		foo();
		printf("Zzz...\n");
		sleep(1);
	}

	return 0;
}
