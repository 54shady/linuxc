#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define X_PIXEL	3
#define Y_PIXEL 6

void rever180(int dst[X_PIXEL][Y_PIXEL], int src[X_PIXEL][Y_PIXEL], int col, int row)
{
	int i, j;
	for (i = col - 1; i >= 0; i--) {
		for (j = row - 1; j >= 0; j--) {
			dst[col - i - 1][row - j - 1] = src[i][j];
		}
	}

}

int main(int argc, char *argv[])
{
	int i, j;
	int src[X_PIXEL][Y_PIXEL];
	int dst[X_PIXEL][Y_PIXEL];
	int org[X_PIXEL][Y_PIXEL];

	srand( (unsigned int)time(0) );

	printf("gensrc:\n");
	for (i = 0; i < X_PIXEL; i++) {
		for (j = 0; j < Y_PIXEL; j++)
			src[i][j] = rand() % 99;
		printf("\n");
	}

	printf("src:\n");
	for (i = 0; i < X_PIXEL; i++) {
		for (j = 0; j < Y_PIXEL; j++)
			printf("%d\t", src[i][j]);
		printf("\n");
	}

	rever180(dst, src, X_PIXEL, Y_PIXEL);

	printf("dst:\n");
	for (i = 0; i < X_PIXEL; i++) {
		for (j = 0; j < Y_PIXEL; j++)
			printf("%d\t", dst[i][j]);
		printf("\n");
	}

	printf("compare dst and src\n");
	if (memcmp(src, dst, X_PIXEL * Y_PIXEL) == 0)
		printf("total equal, so nice\n");
	else
		printf("no the same\n");

	printf("let's make back to orignal\n");
	rever180(org, dst, X_PIXEL, Y_PIXEL);
	printf("org:\n");
	for (i = 0; i < X_PIXEL; i++) {
		for (j = 0; j < Y_PIXEL; j++)
			printf("%d\t", org[i][j]);
		printf("\n");
	}

	printf("compare src and org\n");
	if (memcmp(src, org, X_PIXEL * Y_PIXEL) == 0)
		printf("total equal, so nice\n");
	else
		printf("no the same\n");
	return 0;
}
