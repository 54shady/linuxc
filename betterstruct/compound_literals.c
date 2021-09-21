#include <stdio.h>
#include <math.h> /* for nan(Not A Number) */

double sum(double in[])
{
	double out = 0;

	for (int i = 0; !isnan(in[i]); i++)
		out += in[i];

	return out;
}

int main(int argc, char *argv[])
{
	/* typical way to call the sum */
	double list[] = {1.1, 2.2, 3.3, NAN}; /* via throwaway variable */
	printf("sum: %g\n", sum(list));

	/* compound literal way */
	printf("sum: %g\n", sum((double[]){1.1, 2.2, 3.3, NAN}));

	return 0;
}
