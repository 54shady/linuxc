#include <stdio.h>

typedef struct {
	char *name;
	int left, right, up, down;
} direction_s;

int main(int argc, char *argv[])
{
	/* designated initializer, zero all the elements */
	int array[] = {};

	/*
	 * the elements not declared are initialized to zero
	 * no elements are left undefined
	 */
	direction_s d = {.right = 1, .down = 3};
	printf("%d %d %d %d\n", d.left, d.right, d.up, d.down);

	/* zero out all elements */
	direction_s dd = {};
	printf("%s %d %d %d %d\n", dd.name, dd.left, dd.right, dd.up, dd.down);

	double list[] = {[2] = 1.1, [5] = 2.3};

	for (int i = 0; i < 6; i++)
		printf("%g ", list[i]);
	printf("\n");

	return 0;
}
