#include <stdio.h>
#include <stdlib.h>

/*
 * safely parse string to numbers
 *
 * Usage:
 *
 * ./str2num x11  表示十六进制数0x11
 * ./str2num b11  表示二进制数111
 * ./str2num 11  表示二进制数111
 */
int main(int argc, char *argv[])
{
	char *endptr;
	long input;
	int base;
	char *nptr;

	if (argc < 2)
	{
		printf("Usage example:\n");
		printf("%s x11, Input Hexadecimal number 0x11\n", argv[0]);
		printf("%s b11, Input Binary number 11\n", argv[0]);
		printf("%s  11, Input Decimal number 11\n", argv[0]);
		return 0;
	}

	nptr = argv[1];
	nptr++;
	switch (argv[1][0])
	{
		case 'x':
			base = 16;
			break;
		case 'b':
			base = 2;
			break;
		default:
			base = 10;
			nptr--;
			break;
	};

	input = strtol(nptr, &endptr, base);
		printf("%ld\n", input);

	return 0;
}
