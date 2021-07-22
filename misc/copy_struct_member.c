#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int a;
	short s[2];
}MSG;

int main(int argc, char **argv)
{
	MSG *pmsg = NULL;
	MSG msg = {
		.a = 4,
		.s = {1, 0}
	};
	char *src;
	char *dst;

	pmsg = (MSG *)malloc(sizeof(MSG));

	/*
	 * src指向msg的s起始地址
	 * dst指向pmsg的s起始地址
	 * 且dst范围不超过pmsg的结束地址
	 * 拷贝msg里成员s内容到pmsg里的s成员
	 */
	for (src = (char *)msg.s, dst = (char *)pmsg->s; dst < (char *)(pmsg + 1); )
		*dst++ = *src++;

	printf("pmsg->s[0] = %d, pmsg->s[1] = %d\n", pmsg->s[0], pmsg->s[1]);

	return 0;
}
