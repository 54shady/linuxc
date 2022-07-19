#include <stdio.h>

/*
 * 1. 编译文件
 * gcc -g a.c
 *
 * 2. 查看内核dmesg信息
 * segfault at 0 ip 0000563fe3f9a13c sp 00007fffd4ee7e50 error 6 in a.out[563fe3f9a000+1000]
 * 0000563fe3f9a13c - 563fe3f9a000+1000 = 113c
 *
 * 上面的打印信息来自内核的(arch/x86/mm/fault.c)show_signal_msg函数
 *
 * 其中arch/x86/include/asm/trap_pf.h文件内容说明了error code用多少位来表示
 * Page fault error code bits:
 *
 *   bit 0 ==    0: no page found   1: protection fault
 *   bit 1 ==    0: read access     1: write access
 *   bit 2 ==    0: kernel-mode access  1: user-mode access
 *   bit 3 ==               1: use of reserved bit detected
 *   bit 4 ==               1: fault was an instruction fetch
 *   bit 5 ==               1: protection keys block access
 *   bit 15 ==              1: SGX MMU page-fault
 *
 * 3. 生成反汇编文件,并找到出错位置的代码
 * objdump -D a.out > a.dis
 * grep 113c a.dis
 * 113c:       c7 00 0c 00 00 00       movl   $0xc,(%rax)
 *
 * 4. 使用addr2line找到对应到c文件中的代码位置
 * gcc 编译的时候需要添加-g参数才能看到行数
 * addr2line -e a.out 113c
 *
 * 对于没有开启pie功能的gcc编译器(没有配置选项--enable-default-pie)会看到如下错误
 * segfault at 0 ip 0000000000401109 sp 00007ffed69736f0 error 6 in * a.out[401000+1000]
 *
 * addr2line -e a.out 401109
 *
 * 查看默认的链接脚本
 * gcc -Wl,-verbose a.c
 */

int main(int argc, char *argv[])
{
	int *p = NULL;

	*p = 12;

	return 0;
}
