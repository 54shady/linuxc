#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct {
	double width, height;
} papersize_s;

/*
 * c语言中两种方法来返回多个值
 * 1. 通过返回结构体来达到返回多个值的效果
 * 2. 通过返回指针
 * void width_height_p(char *papertype, double *width, double *height)
 */
papersize_s width_height(char *papertype)
{
	return
		!strcasecmp(papertype, "A4") ? (papersize_s){.width = 210, .height = 297}
		: !strcasecmp(papertype, "Letter") ? (papersize_s){.width = 216, .height = 279}
		: !strcasecmp(papertype, "Legal") ? (papersize_s){.width = 216, .height = 356}
		: (papersize_s){.width = NAN, .height = NAN};
}

int main(int argc, char *argv[])
{
	papersize_s a4size = width_height("a4");
	printf("width = %g, height = %g\n", a4size.width, a4size.height);

	return 0;
}
