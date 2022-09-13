#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

char c;
int count;

void read_config_file(int signum)
{
	FILE *fp;
	char *line;
	size_t len = 0;

	/*
	 * reconfigure.conf context as below
	 * a
	 * 10
	 */
	fp = fopen("/tmp/reconfigure.conf", "r");
	if (!fp)
	{
		perror("Open");
		exit(1);
	}

	/* first line first char */
	if (getline(&line, &len, fp) < 1)
	{
		perror("Getline char less then 1");
		exit(1);
	}
	c = line[0];

	/* second line content the char count */
	getline(&line, &len, fp);
	count = atoi(line);

	printf("read char = %c and count = %d\n", c, count);
	fclose(fp);
}

/*
 * Usage :
 * modify the reconfigure.conf file
 * pkill -HUP a.out
 */
int main(int argc, char *argv[])
{
	int i;

	signal(SIGHUP, read_config_file);
	raise(SIGHUP);

	while (1)
	{
		for (i = 0; i < count; i++)
			putchar(c);
		putchar('\n');
		sleep(1);
	}

	return 0;
}
