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
#define sum(...) sum_array((double []){__VA_ARGS__, NAN})

int main(int argc, char *argv[])
{
	double two_and_two = sum(2, 2);

	printf("2 + 2 = %g\n", two_and_two);
	printf("(2 + 2)*3 = %g\n", sum(two_and_two, two_and_two, two_and_two));
	printf("sum(asst) = %g\n", sum(3.1415, two_and_two, 2, 98.1));

	return 0;
}

