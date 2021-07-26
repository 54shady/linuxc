#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#undef DEFINE_STR_ARRAY

/*
 * strsep和strtok
 * 相同点：两者都会改变源字符串,想要避免,可以使用strdupa(由allocate函数实现)或strdup(由malloc函数实现)
 *
 * strtok函数第一次调用时会把s字符串中所有在delim中出现的字符替换为NULL
 * 然后通过依次调用strtok(NULL, delim)得到各部分子串
 *
 * 如果输入的串的有连续的多个字符属于delim,(此例str中的逗号+空格,感叹号+空格等就是这种情况)
 * strtok会返回NULL,而strsep会返回空串""
 * 因而我们如果想用strsep函数分割字符串必须进行返回值是否是空串的判断
 * 这也就解释了strsep的例子中有多个"+"的原因
 *
 * 建议尽量使用strsep
 */
int main(int argc, char *argv[])
{
#ifdef DEFINE_STR_ARRAY
	char str[] = "hello, world! welcome to Cstring!";
#else
	char *str = "hello, world! welcome to Cstring!";
#endif

	char delim[] = " ,!"; /* delimter is ' ' ',' '!' */

#ifdef DEFINE_STR_ARRAY
	char *s = str;
#else
	/*
	 * strdup will malloc for pointer, which MUST free explicity
	 * strdup不是标准的c函数
	 * strdup()在内部调用了malloc()为变量分配内存,不需要使用返回的字符串时,
	 * 需要用free()释放相应的内存空间,否则会造成内存泄漏
	 */
	char *s = strdup(str);
#endif
	char *token;

	for (token = strsep(&s, delim); token != NULL; token = strsep(&s, delim))
	{
		printf("%s", token);
		printf("+");
	}

	printf("\n");

#ifndef DEFINE_STR_ARRAY
	free(s);
#endif
	return 0;
}
