#include <stdio.h>
#include <stdarg.h>

/* format writing to file the variable way */
void var2file(const char *fmt, ...)
{
	va_list args;
	FILE *fp;

	fp = fopen("./testfile", "a+");

	va_start(args, fmt);
	vfprintf(fp, fmt, args);
	va_end(args);

	fclose(fp);
}

int main(int argc, char *argv[])
{
	var2file("%s-%d\n", "mike", 23);
	var2file("%s-%d, %s:%d\n", "mike", 23, "jordan", 45);

	return 0;
}
