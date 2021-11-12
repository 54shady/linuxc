#include <stdio.h>
#include "para_callback.h"

static int cnt = 0;
static int cnt2 = 0;

void say_hello(void *str)
{
	/* void * ==> char * */
	printf("Hello %s, %d times\n", (const char *)str, cnt2++);
}

void count_number(void *num)
{
	/*
	 * void * ==> int *
	 * 再对int *取值
	 */
	int i;

	printf("cnt = %d\n", cnt++);
	for (i = 0; i < *(int *)num; i++)
		printf("%d ", i);
	printf("\n");
}

int main(int argc, char *argv[])
{
	int repeat_time = 4;
	const char *name = "anonymous";

	/* convert 'char *' to type 'void *' */
	repeat_three_times(say_hello, (void *)name);

	/* convert 'int *' to 'void *' */
	repeat_three_times(count_number, (void *)&repeat_time);

	return 0;
}
