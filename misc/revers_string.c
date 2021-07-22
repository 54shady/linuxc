#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#undef DBG_PRINT

#ifdef DBG_PRINT
#define dbg_print	printf
#else
#define dbg_print(_fmt, args...)     do {} while (0)
#endif
void rever(char *str)
{
	char *head;
	char *tail;
	int len;

	len = strlen(str);
	dbg_print("len = %d\n", len);

	head = str;
	tail = str + len - 1;

	dbg_print("%p, %p, %ld\n", head, tail, tail - head);
	while (head < tail)
	{
		dbg_print("head = %p[%c], tail = %p[%c], %ld\n", head, *head,  tail, *tail, tail - head);
		*head ^= *tail;
		*tail ^= *head ;
		*head ^= *tail;

		dbg_print("head = %p[%c], tail = %p[%c], %ld\n", head, *head,  tail, *tail, tail - head);
		head++;
		tail--;
	}

	dbg_print("%s\n", str);
}

int main(int argc, char **argv)
{
	char *str = NULL;
	int len;

	if (argc < 2)
	{
		printf("%s <string>\n", argv[0]);
		return -1;
	}

	len = strlen(argv[1]);
	str = (char *)malloc(sizeof(char) * len);
	if (NULL == str)
	{
		printf("malloc error\n");
		return -2;
	}

	strncpy(str, argv[1], len);

	printf("Org:%s\n", str);
	rever(str);
	printf("Rev:%s\n", str);

	free(str);

	return 0;
}
