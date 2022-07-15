#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char str[] = "hello, world! welcome to Cstring!";
	char delim[] = " ,!";
	char *token;

	for (token = strtok(str, delim); token != NULL; token = strtok(NULL, delim))
	{
		printf("%s", token);
		printf("+");
	}

	printf("\n");

	return 0;
}
