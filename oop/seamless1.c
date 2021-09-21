#include <stdio.h>
#include <math.h>

typedef struct point {
	double x, y;
} point;

typedef struct {
	/*
	 * 如果不用匿名成员的话
	 * struct point twopoint;
	 *
	 * 这样的话来访问数据的时候代码如下
	 *
	 * threepoint.twopoint.x;
	 * threepoint.twopoint.y;
	 *
	 * 此处用匿名成员的好处是在访问的时候变成如下
	 *
	 * threepoint.x;
	 * threepoint.y;
	 */
	struct point; /* anonymous member */

	double z;
} threepoint;

double threelength(threepoint p)
{
	/* 用匿名成员的好处就是在访问的时候 */
	return sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
}

/*
 * gcc中的-fms-extensions能够让c的结构体更好的扩展
 * gcc seamlessone.c -fms-extensions -lm
 */
int main(int argc, char *argv[])
{
	threepoint p = {.x = 3, .y = 0, .z = 4}; /* 初始化的时候能直接用x, y */
	printf("p is %g units from the origin\n", threelength(p));

	return 0;
}
