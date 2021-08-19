#include <stdio.h>
#include <stddef.h>

/*
 * 求结构体成员偏移量
 */

/* print %p using this marco */
//#define __OFFSET(type, member) (&((type *)(0))->member)

/* print %d using this marco */
#define __OFFSET(type, member) (size_t)(&((type *)(0))->member)

/*
 * 求结构体首地址
 * 成员变量的地址减去该成员在结构体中的偏移量就是结构体首地址
 * 这个宏的目的就是返回(type*)类型的指针
 */
#define my_container_of(ptr, type, member)	\
	(type *)( (unsigned char *)(ptr) - (size_t)&( ((type *)(0))->member) )

struct test {
	int a;
	int b;
	int c;
	char d;
};

void foo(int *c)
{
	struct test *pt;

	pt = my_container_of(c, struct test, c);
	printf("a = %d\n", pt->a);
	printf("b = %d\n", pt->b);
	printf("c = %d\n", pt->c);
	printf("d = %c\n", pt->d);
}

int main(int argc, char **argv)
{
#ifdef INSTANCE
	struct test t;
	t.a = 1;
	t.b = 2;
	t.c = 3;
	t.d = 'd';
	foo(&t.c);
	return 0;
#else
	struct test t;
	printf("test = %p\n", &t);
	printf("test.c = %p\n", &(t.c));

	/*
	 * 把结构体test的一个实例映射到0这个地址
	 * 这样成员的地址就是在0的基础上往上加
	 * 进而此时成员的地址
	 * 1. 把结构体映射到0地址 (struct test *)(0)
	 * 2. 取结构体中成员 ((struct test *)(0))->c
	 * 3. 取成员地址偏移量 &(((struct test *)(0))->c)
	 */
	printf("offset of struct->c= %p\n", &(((struct test *)(0))->c));

	printf("offset of c= %p\n", __OFFSET(struct test, c));
	printf("offset of d= %p\n", __OFFSET(struct test, d));
	/* using stddef.h macro offsetof */
	printf("offset of c= %p\n", offsetof(struct test, c));

	/*
	 * 求结构体首地址
	 * 成员变量的地址减去该成员在结构体中的偏移量就是结构体首地址
	 *
	 * 指针的加减操作的步长是按照这个指针的类型来定的
	 * 所以这里需要把指向成员变量c的指针类型改为char
	 */
	printf("struct test address is %p\n", (unsigned char *)&(t.c) - (size_t)&(((struct test *)(0))->c));
	printf("struct test address is %p\n", (unsigned char *)&(t.d) - (size_t)&(((struct test *)(0))->d));
	printf("struct test address is %p\n", my_container_of(&t.c, struct test, c));
	printf("struct test address is %p\n", my_container_of(&t.d, struct test, d));

	return 0;
#endif
}
