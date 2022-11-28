#include <stdio.h>

#undef CONST_POINTER
#undef POINTER_CONST

int main(int argc, char *argv[])
{
	int a = 1;
	int b = 2;

//#define CONST_POINTER
#ifdef CONST_POINTER
	/*
	 * 常 指针: 只读指针,不可用与修改变量值
	 *
	 * const int *pa
	 * ~~~~~~~~~ ---
	 * ^         ^
	 * |         |
	 * 常        指针
	 *
	 * 等价写法: int const *pa = &a;
	 *			 ~~~~~~~~~~ ---
     *			 ^          ^
	 *			 |          |
	 *			 常         指针
	 */
	const int *pa = &a;

	/* 常 指针: 不能通过这个指针来改变它指向的变量值 */
	/* *pa = 2; */

	/* 常 指针: 可以指向新的变量 */
	pa = &b;
	printf("*pa = %d\n", *pa);
#endif

//#define POINTER_CONST
#ifdef POINTER_CONST
	/*
	 * 指针 常量: 指针的地址在初始化时就固定,后续不可变(不能再指向新变量)
	 *
	 * int * const pa;
	 * ~~~~  -----
	 * ^      ^
	 * |      |
	 * 指针   常量
	 */
	int * const pa = &a;

	/* 修改变量a的值 */
	*pa = 2;
	printf("a = %d\n", a);

	/* 不能指向新的变量 */
	/* pa = &b; */
#endif

	return 0;
}
