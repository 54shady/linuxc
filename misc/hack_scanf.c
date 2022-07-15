#include <stdio.h>

/*
 * %[*][输入数据宽度][长度]类型
 * "*"星号符:用以表示该输入项,读入后不赋予相应的变量,即跳过该输入值
 */
int main(int argc, char **argv)
{
	int a, b;
	char tmp;
	int i;

	printf("Enter 3 number: ");
	scanf("%d %*d %d", &a, &b);
	printf("First:%d, Third:%d\n", a, b);
	printf("\n===========================\n");

	/*
	 * 输入一个字符的情况下:
	 * 用%*c过滤掉第二个字符(回车符)
	 * 不然下一次读字符,就会读到回车符
	 */
	for (i = 0; i < 10; i++)
	{
		scanf("%c%*c", &tmp);
		printf("tmp = %d, i = %d\n", tmp, i);
	}

	printf("\n===========================\n");
	/* 异常1 : 输入一次打印两次i */
	for (i = 0; i < 10; i++)
	{
		scanf("%c", &tmp);
		printf("i = %d\n", i);
	}

	printf("\n===========================\n");
	/*
	 * 异常2 : i值一直都不递增
	 * 此时栈已被破坏
	 * 应为scanf %d输入的是一个32bit
	 * 而存储空间只有8bit
	 */
	for (i = 0; i < 10; i++)
	{
		/* 编译时有编译警告 */
		scanf("%d", &tmp);
		printf("i = %d\n", i);
	}

	printf("end main\n");
}
