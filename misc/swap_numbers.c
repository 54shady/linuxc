#include <stdio.h>

#define BUG_FIX

void myswap(int *a, int *b)
{
#ifdef BUG_FIX
	/*
	 * 这里面却存在着一个非常隐蔽的陷阱
	 * 对于arr[n]中如果arr[i] == arr[j]
	 * 在交换arr[i]和arr[j]的时候需要确保这两个数不是同一个数
	 * 否则下面的代码就变成了
	 * a ^= b
	 * a ^= b
	 * a ^= b
	 * 把同一个数异或了3次
	 */
	if (a == b)
		return;
#endif

	/*
	 * if a and b point to the same area
	 * *a ^= *b equal to *a = a[0] & a[0]
	 * result is *a = 0
	 */
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

int main(int argc, char **argv)
{
	int i;
	int arr[] = {1, 2};

	for (i = 0; i < 2; i++)
		printf("%d\t", arr[i]);
	printf("\n");

	/* swap the same number will cause error */
	for (i = 0; i < 2; i++)
		myswap(&arr[i], &arr[i]);

	for (i = 0; i < 2; i++)
		printf("%d\t", arr[i]);
	printf("\n");

	return 0;
}
