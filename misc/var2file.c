#include <stdio.h>
#include <stdarg.h>

/* format writing to file the variable way */
void var2file_fixed_path(const char *fmt, ...)
{
	va_list args;
	FILE *fp;

	fp = fopen("./testfile", "a+");

	va_start(args, fmt);
	vfprintf(fp, fmt, args);
	va_end(args);

	fclose(fp);
}

void var2file_pickfile(const char *fmt, const char* file, ...)
{
	va_list args;
	FILE *fp;

	fp = fopen(file, "a+");

	va_start(args, fmt);
	vfprintf(fp, fmt, args);
	va_end(args);

	fclose(fp);
}
int main(int argc, char *argv[])
{
	var2file_fixed_path("%s-%d\n", "mike", 23);
	var2file_fixed_path("%s-%d, %s:%d\n", "mike", 23, "jordan", 45);

	const char *path1 = "./testfile1";
	const char *path2 = "./testfile2";

	var2file_pickfile("%s-%d\n", path1, "mike", 23);
	var2file_pickfile("%s-%d, %s:%d\n", path2, "mike", 23, "jordan", 45);

	return 0;
}
