#include <stdio.h>
#include <math.h>

double sum_array(double in[])
{
	double out = 0;

	for (int i = 0; !isnan(in[i]); i++)
		out += in[i];

	return out;
}

/* 使用宏来保证结尾一定有NAN */
/* compound literal and variadic macros are the cutest couple */
/* 通过可变长度宏来实现可变长度参数函数 */
#define sum(...) sum_array((double []){__VA_ARGS__, NAN})

int main(int argc, char *argv[])
{
	/* 使用宏sum就像使用函数一样, 让调用者认为是个可变长度函数 */
	double two_and_two = sum(2, 2);

	printf("2 + 2 = %g\n", two_and_two);
	printf("(2 + 2)*3 = %g\n", sum(two_and_two, two_and_two, two_and_two));
	printf("sum(asst) = %g\n", sum(3.1415, two_and_two, 2, 98.1));

	return 0;
}

