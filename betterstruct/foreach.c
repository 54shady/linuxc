#include <stdio.h>

typedef char * string;

#if 0
int main(int argc, char *argv[])
{
	string *str = "thread";
	for (string *iterator = (string []){"yarn", str, "rope", NULL}; *iterator;
			iterator++)
		printf("%s\n", *iterator);
}
#endif

#define Foreach_string(iterator, ...) \
	for (char **iterator = (char *[]){__VA_ARGS__, NULL}; *iterator; iterator++)

int main(int argc, char *argv[])
{
	char *group1 = "Group1";
	char *group2 = "G2";

	Foreach_string(iter, "yarn", group1, "rope", group2)
	{
		printf("%s\n", *iter);
	}

	return 0;
}
