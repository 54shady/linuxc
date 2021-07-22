#include <stdio.h>

struct node
{
	char a;
	char b;
	short c;
	int d;
};

/*
 *	 ps--->	+-----------+      |   0x00000000
 * 			|	a = 3	|      |  内存地址增长方向
 * 			-------------      |
 * 			|	b = 5	|      |
 * 			-------------      |
 * 			|	c = 6	|      |
 * 			|			|      |
 * 			-------------      |
 * 			|	d = 99  |      |
 * 			|		    |      |
 * 			|		    |      |
 * 			|		    |      |
 * 			+-----------+      V  0xFFFFFFFF
 */

int main(int argc, char *argv[])
{
	struct node s = {3, 5, 6, 99};
	struct node *ps = &s;

	/* 小端系统低位低地址 result must be 0x60503 */
	printf("0x%x\n", *(int *)ps);
	printf("sizeof(node) = %ld\n", sizeof(struct node));

	return 0;
}
