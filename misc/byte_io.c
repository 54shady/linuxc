#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE *fp;
	int ch;

	/* open the file for write */
	if ( (fp = fopen("iotestfile", "w+")) == NULL )
	{
		perror("Open file error\n");
		exit(1);
	}

	/*
	 * read from stdin and write to file
	 * hit ctrl_d on keyborad which means EOF
	 */
	while ( (ch = getchar()) != EOF )
		fputc(ch, fp);

	/* make read write point to start */
	rewind(fp);

	/* read from file and write to stdio */
	while ( (ch = fgetc(fp)) != EOF )
		putchar(ch);

	/* close the file */
	fclose(fp);

	return 0;
}
