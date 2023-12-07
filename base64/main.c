#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "base64.h"

#define ENCODE_BUFFER_LEN 1024

int main(int argc, char *argv[])
{
	unsigned char en_output[ENCODE_BUFFER_LEN];
	int len_input; /* length of input string */
	int len_output; /* output length should equal to or longer than input length */
	int len_encode; /* length of encode string */
	int len_decode; /* length of decode string */
	unsigned char *de_output;

	if (argc < 3)
	{
		printf("%s <-e | -d> <string>\n", argv[0]);
		return -1;
	}

	len_input = strlen((const char *)argv[2]);

	switch (argv[1][1])
	{
		case 'e':
			printf("encode string: %s\n", argv[2]);

			/* encode will longer the stirng */
			/* each 3 bytes encode to 4 bytes */
			assert(strlen((const char *)argv[2]) < ENCODE_BUFFER_LEN * 4 / 3);

			/* base64 encoding */
			len_encode = base64encode((const unsigned char *)argv[2], len_input, en_output, sizeof(en_output));
			en_output[len_encode] = '\0';
			printf("encoding : %s\n", en_output);

			break;
		case 'd':
			printf("decode string: %s\n", argv[2]);

			/* malloc buffer for output string */
			len_output = len_input;

			de_output = malloc(len_output * sizeof(char) + 1);
			if (NULL == de_output)
			{
				perror("Malloc error");
				return -2;
			}

			/* base64 decoding */
			len_decode = base64decode(argv[2], strlen((const char *)argv[2]), de_output, len_output);
			de_output[len_decode] = '\0';
			printf("decoding : %s\n", de_output);

			free(de_output);

			break;
		default:
			printf("Unkonw action\n");
			break;
	}

	return 0;
}
