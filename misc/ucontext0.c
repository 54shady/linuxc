#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	ucontext_t context;
	int i = 0;

	/* 初始化context结构体,将当前的上下文保存到context中 */
	getcontext(&context);

	printf("Hello world %d\n", i);
	sleep(1);
	i += 1;

	/* 设置当前的上下文为context,并跳转至其中 */
	setcontext(&context);

	return 0;
}
