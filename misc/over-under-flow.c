#include <stdio.h>
#include <limits.h>

/*
 * ref:  https://www.scaler.com/topics/c/overflow-and-underflow-in-c/
 *
 * When we attempt to store a value that cannot be represented correctly by a data type
 * an Integer Overflow (or) Underflow occurs.
 * If the value is more than the maximum representable value, the phenomenon is called Integer Overlow.
 * If the value is less than the least representable value, the phenomenon is called' Integer Underflow'
 */

int overflow(void)
{
    /* INT_MAX is the maximum representable integer. */
    int a = INT_MAX;
    printf("a = %d\n", a);
    printf("Adding 1 to a...\n");
    a = a + 1;
    printf("a = %d\n", a);
    return 0;
}

int underflow(void)
{
    /* INT_MIN in the least representable integer. */
    int a = INT_MIN;
    printf("a = %d\n", a);
    printf("Subtracting 1 from a...\n");
    a = a - 1;
    printf("a = %d\n", a);
    return 0;
}

int main(int argc, char *argv[])
{
	overflow();
	underflow();

	return 0;
}
