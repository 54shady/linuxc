#include <stdio.h>

#define MAX_LINE 1024
#define SHELL_SCRIPT "./test.sh"

int main(int argc, char *argv[])
{
	FILE *fp;
	char buff[MAX_LINE];

	if ((fp = popen(SHELL_SCRIPT, "r")) == NULL)
		perror("popen error\n");

	fgets(buff, MAX_LINE, fp);
	printf("buff = %s\n", buff);

	pclose(fp);

	return 0;
}
