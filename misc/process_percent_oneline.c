#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int i;

	for (i = 0; i < 100; i++)
	{
		/* \33[2K清除一行内容, \r光标回到行首 */
		printf("\33[2K\r");
		printf("Install: %d%%", i);
		fflush(stdout);
		usleep(100000);
	}
	printf("\n");
	return 0;
}
