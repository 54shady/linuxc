#include "stopif.h"

int main(int argc, char *argv[])
{
	double x;

	printf("Enter a number: ");
	scanf("%lg", &x);
	Stopif(x < 0 || x > 1, return -1, "x has value %g "
			"but it should be "
			"between zero and one.", x);

	return 0;
}
