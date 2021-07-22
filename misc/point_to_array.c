#include <stdio.h>

#define DEBUG_PRINT
#ifdef DEBUG_PRINT
#define DBG_PRINT	printf
#else
#define DBG_PRINT
#endif

/*
 * &a是一个隐式的指向int [5]数组的指针,它和int* ptr是不一样的
 * 如果真要定义这个指针,应该是int (*ptoa)[5].
 * 所以ptoa每一次加一操作都相当于跨越int a[5]的内存步长(也就是5个int长度)
 * 也就是说&a + 1其实就是指向了a[5]这个位置
 * 实际上内存里面这个位置是非法的,但是对ptr的强制转换导致了后面ptr-1的内存步长改为了1个int长度
 * 所以ptr-1实际指向了a[4]
 */
int main(int argc, char *argv[])
{
	/* 指向数组的指针 */
	int (*ptoa)[5] = NULL;
	int *ptr = NULL;

	int a[5] = { 1, 2, 3, 4, 5 };

	ptoa = &a;
	DBG_PRINT("ptoa = %p, &a = %p\n", ptoa, &a);
	ptoa = &a + 1;
	DBG_PRINT("ptoa + 1 = %p, &a = %p, ptoa - &a = %ld\n", ptoa, &a, (unsigned char *)ptoa - (unsigned char *)&a);

	ptr = (int*)(&a + 1);

	printf("%d, %d\n", *(a + 1), *(ptr -1));

	return 0;
}
