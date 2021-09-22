#include <stdio.h>
#include <math.h>

typedef struct point {
	double x, y;
} point;

typedef struct {
	union {
		struct point; /* anonnymous element */
		point p2; /* named element */
	};

	double z;
} threepoint;

double length(point p)
{
	return sqrt(p.x*p.x + p.y*p.y);
}

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

	/*
	 * 这里需要用到named element
	 * 所以结构体中用了一个联合体
	 * 既定义了anonymous element也定义了named element
	 */
	double xylength = length(p.p2);
	printf("Its projection onto the XY plane "
			"is %g units from the origin\n", xylength);
	return 0;
}
