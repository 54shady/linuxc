#include <stdio.h>

#define NR_ROW 3
#define NR_COL 4

void foo(void *p)
{
	int i;
	char (*pp)[NR_COL] = p;
	char *tmp;


	for (i = 0; i < NR_ROW; i++)
	{
		tmp = (char *)pp;
		printf("%s\n", tmp);

		/* 直接跳转到二维数组的下一行 */
		pp++;
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	char twodemesion_char_array[NR_ROW][NR_COL] = {
		{'e', 'f', 'h', '\0'},
		{'j', 'K', 'l', '\0'},
		{'o', 'p', 'q', '\0'}
	};

	char string_array[NR_ROW][NR_COL] = {
		"efh",
		"jKl",
		"opq",
	};

	/*
	 * 使用指向数组的指针来访问
	 * 可以方便使用指针加加来访问二维数组的下一行
	 * 所以下面赋值操作后
	 * pd[0] = string_array[0]
	 * pd[1] = string_array[1]
	 * pd[2] = string_array[2]
	 * ...
	 */
	char (*pd)[NR_COL] = &string_array[0];
	char (*pc)[NR_COL] = &twodemesion_char_array[0];

	foo(pd);
	foo(pc);

	return 0;
}
