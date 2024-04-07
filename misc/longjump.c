#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

void B(void)
{
	printf("Enter %s()\n", __FUNCTION__);
	printf("Try long jump? (y|n) ");
	if (getchar() == 'y')
		longjmp(env, 1234);
	printf("Exit %s()\n", __FUNCTION__);
}

void A(void)
{
	printf("Enter %s()\n", __FUNCTION__);
	B();
	printf("Exit %s()\n", __FUNCTION__);
}

/*
 * 调用顺序是 main -> A -> B, 则返回是应该是 B->A->main
 * 如果在B中执行长跳转,则不需要返回到A即可返回到main(B->main)
 */
int main(int argc, char *argv[])
{
	int r, a = 100;

	printf("%s() call setjmp to save enviroment\n", __FUNCTION__);

	if ((r = setjmp(env)) == 0) {
		A();
		printf("normal return to %s()\n", __FUNCTION__);
	} else {
		printf("back to %s() via long jump, r = %d, a = %d\n", __FUNCTION__, r, a);
	}

	return 0;
}
