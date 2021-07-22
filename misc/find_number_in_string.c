#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG_PRINT
#define DBG_PRINT	printf
#else
#define DBG_PRINT(_fmt, args...)     do {} while (0)
#endif

int main(int argc, char *argv[])
{
	char *buf = "find_my_number123, eminem";
	char *s = NULL;
	int idx;

	DBG_PRINT("Input str is \"%s\"\n", buf);

	for (s = buf; *s; s++)
	{
		if ((*s >= '0' && *s <= '9') || *s == ',')
		{
			DBG_PRINT("%s\n", s);
			break;
		}
	}

    idx = strtoul(s, NULL, 10);

	printf("number in str is %d\n", idx);

	return 0;
}
