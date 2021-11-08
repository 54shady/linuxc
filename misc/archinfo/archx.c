#include <stdio.h>

int main(int argc, char *argv[])
{

	/* ABI: 使用预处理器的预定义宏 */
#ifdef __x86_64__
	printf("On this architecture\n");
#endif

#ifdef __i386__
	printf("On this architecture\n");
#endif

#ifdef __aarch64__
	printf("On this architecture\n");
#endif

#ifdef __arm__
	printf("On this architecture\n");
#endif

	return 0;
}
