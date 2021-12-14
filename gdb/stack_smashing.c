/* https://stackoverflow.com/questions/33488672/cannot-insert-breakpoint-0 */
#include <string.h>

int foo(void)
{
	char buf[1];

	/*
	 * 1. 由于这里写了1024字节大小,写到栈了,导致的栈溢出
	 * 2. 这里故意写数据5,方便代码中看到栈确实是被这行代码破坏了
	 * 如果时0的话栈里数据就都是0
	 */
#ifdef BUG_FIXED
	memset(buf, 5, 1);
#else
	/* force make stack overflow */
	memset(buf, 5, 1024);
#endif

	/* 故意返回值1是为了在调试时区分,明白原理后无所谓 */
	return 1;
}

int main(int argc, char *argv[])
{

	/*
	 * main函数调用foo会进行压栈操作
	 * 汇编代码中call foo 会将rbp+8设置成返回地址
	 * 可以用gdb在调用完call foo后查看,对应的就是main函数调用完foo后面的地址
	 * (gdb) x/a $rbp+8
	 *
	 * 在栈溢出错误的代码中可以看到这个值已经写满了数据5,所以程序最后出错
	 *
	 * 1. 首先将要传给foo的参数进行压栈
	 * 2. 最后会将返回值放到栈上,这样再调用完foo后才能正确返回
	 * 3. 设置ebp为栈底,将foo函数中的变量进行压栈(如果有的话)
	 *
	 * 所以可以比较清晰的理解,调用完foo后需要跳转到ret返回地址
	 * 这个地址位于ebp+8的地址(32位系统ebp+4,64位系统ebp+8)
	 * 这个可以将上述代码修正
	 *
	 * +------+<----esp 栈顶指针
	 * |      |
	 * +------+
	 * |      |
	 * +------+
	 * |      |
	 * +------+
	 * | arg2 |<----foo函数中的参数
	 * +------+
	 * | arg1 |<----foo函数中的参数
	 * +------+<----ebp 栈底指针
	 * | ret  |
	 * +------+
	 * | arg2 |<----传递给foo函数的参数
	 * +------+
	 * | arg1 |<----传递给foo函数的参数
	 * +------+
	 */
	return foo();
}

/*
 * gcc -g stack_smashing.c
 * foo函数没有返回值
 * 使用gdb在foo上打断点,观察这个函数的最后的返回值
 * (gdb) b foo
 * (gdb) layout split
 *
 * 让程序运行到memset这一行后检查栈上情况(这里检查返回值的数据)
 * (gdb) x/a $rbp+8
 * 0x7fffffffdbf8: 0x5555555551a1 <main+20>
 * (gdb) s
 * (gdb) x/a $rbp+8
 * 0x7fffffffdbf8: 0x505050505050505 <==执行完memset后栈溢出导致保存的返回值被修改
 *
 * 在复杂项目中不太容易一下就定位到memset这行
 * 可以通过下面办法来调试
 * 因为知道是栈被破坏了,所以可以在这个地址(这里以返回地址的地方)设置一个观察点
 * 当程序发生修改这个地址的内容时,导致错误就会停下来
 * (gdb) watch *(int**)0x7fffffffdbf8
 * (gdb) c
 * Hardware watchpoint 2: *(int**)0x7fffffffdbf8
 *
 * Old value = (int *) 0x5555555551a1 <main+20>
 * New value = (int *) 0x505050505050505 <==这里就是栈溢出了,内存里的数据都是memset写的5
 * 0x00007ffff7f3af9d in ?? () from /lib64/libc.so.6
 *
 * 此时程序停在值被修改的地方,也就是栈被破坏的现场(在foo函数中memset的地方)
 * 然后观察栈回溯情况,发现main函数看不到,因为栈被破坏掉了
 * 从下面可以看到,#1之后,栈里都是数据5
 * (gdb) bt
 * #0  0x00007ffff7f3afa2 in ?? () from /lib64/libc.so.6
 * #1  0x0000555555555172 in foo () at stack_smashing.c:13
 * #2  0x0505050505050505 in ?? ()
 * #3  0x0505050505050505 in ?? ()
 * #4  0x0505050505050505 in ?? ()
 * #5  0x0505050505050505 in ?? ()
 * #6  0x0505050505050505 in ?? ()
 * #7  0x0005050505050505 in ?? ()
 * #8  0x000055555555518d in foo () at stack_smashing.c:17
 * #9  0x00000001000007f0 in ?? ()
 * #10 0x00007fffffffdd78 in ?? ()
 * #11 0x0000000000000000 in ?? ()
 *
 * (gdb) disable 2
 *
 * 保存断点可以给下次调试使用
 * (gdb) save breakpoints bp.txt
 * gdb -x bp.txt ./a.out
 *
 * 关于rbp和rsp的关系
 * rbp是x86_64上的帧指针,在生成的代码中,它会获取堆栈指针rsp的快照
 * 以便在对rsp进行调整(即为局部变量保留空间或将push的值保留在堆栈上)
 * 局部变量和仍可以从rbp的恒定偏移量访问功能参数
 * 许多编译器都提供了省略帧指针作为优化选项.
 * 这将使生成的汇编代码访问变量相对于rsp,并释放rbp作为另一个通用寄存器供功能使用.
 * gcc中通过-fomit-frame-pointer来开关编译代码
 * gcc -g -fomit-frame-pointer stack_smashing.c之后上述汇编代码中rbp就变为rsp
 */
