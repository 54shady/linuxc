#include <stdio.h>
#include <string.h>

/*
 * gcc -g c.c
 *
 * segfault at 561cc2ae2004 ip 0000561cc2ae113f sp 00007ffe0913ba10 error 6 in * a.out[561cc2ae1000+1000]
 * 0000561cc2ae113f - 561cc2ae1000+1000 = 113f
 *
 * objdump -D a.out > a.dis
 * grep 113f a.dis
 * 113f:       c7 00 54 65 73 74       movl   $0x74736554,(%rax)
 *
 * addr2line -e a.out 113f
 *
 * 对于没有开启pie功能的gcc编译器(没有配置选项--enable-default-pie)会看到如下错误
 * segfault at 400590 ip 00000000004004e4 sp 00007fffa24d72a0 error 7 in * a.out[400000+1000]
 *
 * grep 4004e4 a.dis
 *   4004e4:       c7 00 54 65 73 74       movl   $0x74736554,(%rax)
 *
 * addr2line -e a.out 4004e4
 */

int main(int argc, char *argv[])
{
	char *p = "test";

	strcpy(p, "Test");

	return 0;
}
