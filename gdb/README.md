# GDB调试

## How to use meson build system

build all

	meson build_dir
	sudo ninja -C build_dir

build single module(build goutc for example)

	meson build_dir
	sudo ninja -C build_dir goutc

## GDB 使用coredump调试

[参考文章gdb调试coredump](https://blog.csdn.net/sunxiaopengsun/article/details/72974548)

## GDB如何正确设置路径

程序在编译时,代码路径在/path/to/compile/program,位于编译机器上

现在将程序运行在运行机器,此时需要在运行机器上调试,最简单的方法是设置
和编译时相同的路径,也可以不相同如下在运行机器上配置.gdbinit

在编译机器上代码位于/path/to/run/and/debug/program

	set substitute-path /path/to/compile /path/to/run/and/debug
	directory /path/to/run/and/debug/program

### coredump配置

查看系统是否打开coredump

	ulimit -c

设置系统开启coredump

	ulimit -c unlimited

自定义coredump文件名(在目录/path/to/下保存, 格式为core_filename_time_pid)

	echo /path/to/core_%e_%t_%p > /proc/sys/kernel/core_pattern

如果没有上面节点,需要配置内核

	CONFIG_COREDUMP

### 使用coredump

使用gdb和coredump调试程序(格式如下)

	gdb a.out core_dump_file

## 例子1

	gdb gout1 core_gout1_1529915161_5890

查看堆栈使用bt或者where命令

	(gdb) bt
	#0  0x00000000004004e6 in error_maker () at test1.c:7
	#1  0x0000000000400500 in main (argc=1, argv=0x7ffe528e2348) at test1.c:12
	(gdb) where
	#0  0x00000000004004e6 in error_maker () at test1.c:7
	#1  0x0000000000400500 in main (argc=1, argv=0x7ffe528e2348) at test1.c:12

## 例子2


加载应用程序

	(gdb) file gout2

加载coredump文件

	(gdb) core-file core_gout2_1529925536_10604

重新执行程序(可以不执行这步, 如果使用coredump的话就不要步)
运行了该命令后就不是使用coredump了,看到的信息有一些差异

	(gdb) r

查看所有线程信息

	(gdb) info thread
	  Id   Target Id         Frame
	  1    Thread 0x7ffff7fd9700 (LWP 10987) "gout2" 0x00007ffff7bc298d in
	pthread_join (
		threadid=140737345681152, thread_return=0x0) at pthread_join.c:90
	  2    Thread 0x7ffff77ef700 (LWP 10991) "gout2" 0x00007ffff78bc30d in nanosleep
	()
		at ../sysdeps/unix/syscall-template.S:84
	* 3    Thread 0x7ffff6fee700 (LWP 10992) "gout2" 0x0000000000400834 in
	  error_maker () at test2.c:14

查看所有线程的调用栈

	(gdb) thread apply all bt

	Thread 3 (Thread 0x7ffff6fee700 (LWP 10992)):
	#0  0x0000000000400834 in error_maker () at test2.c:14
	#1  0x0000000000400881 in start_routine (args=0x7fffffffe884) at test2.c:29
	#2  0x00007ffff7bc16ba in start_thread (arg=0x7ffff6fee700) at
	pthread_create.c:333
	#3  0x00007ffff78f741d in clone () at
	../sysdeps/unix/sysv/linux/x86_64/clone.S:109

	Thread 2 (Thread 0x7ffff77ef700 (LWP 10991)):
	#0  0x00007ffff78bc30d in nanosleep () at ../sysdeps/unix/syscall-template.S:84
	#1  0x00007ffff78bc25a in __sleep (seconds=0) at ../sysdeps/posix/sleep.c:55
	#2  0x0000000000400859 in start_routine (args=0x7fffffffe884) at test2.c:25
	#3  0x00007ffff7bc16ba in start_thread (arg=0x7ffff77ef700) at
	pthread_create.c:333
	#4  0x00007ffff78f741d in clone () at
	../sysdeps/unix/sysv/linux/x86_64/clone.S:109

	Thread 1 (Thread 0x7ffff7fd9700 (LWP 10987)):
	#0  0x00007ffff7bc298d in pthread_join (threadid=140737345681152,
	thread_return=0x0)
		at pthread_join.c:90
	#1  0x0000000000400933 in main (argc=1, argv=0x7fffffffe998) at test2.c:54

- 可以看到有3个线程,主线程Thread1调用pthread_join
- 子线程Thread2 未满足判断条件,所以处在sleep中
- 子线程Thread3 满足判断条件,执行error_maker

## 例子3

分别在代码[test3.c](./test3.c)的18和27行打上断点,观察变量str分配内存前后地址变化

	(gdb) b 18
	Breakpoint 1 at 0x400876: file test2.c, line 18.
	(gdb) b 27
	Breakpoint 2 at 0x4008a9: file test2.c, line 27.
	(gdb) r
	Starting program: gout2
	[Thread debugging using libthread_db enabled]
	Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
	[New Thread 0x7ffff77ef700 (LWP 38330)]
	start_routine1, 41(value = 1)
	[New Thread 0x7ffff6fee700 (LWP 38331)]
	start_routine2, 59(value = 2)
	start_routine1, 41(value = 1)
	start_routine1, 41(value = 1)
	[Switching to Thread 0x7ffff6fee700 (LWP 38331)]

当程序停在第一个断点时,观察str内存地址任然未0,为预期值

	Thread 3 "gout2" hit Breakpoint 1, error_maker () at test2.c:18
	18              printf("Oops..., something terriable will happen!!\n");
	(gdb) x str
	0x0:    Cannot access memory at address 0x0

让程序继续跑起来知道第二个断点的地方,此时str地址变为0x7fffe80008c0

	(gdb) c
	Continuing.
	start_routine1, 41(value = 1)
	Oops..., something terriable will happen!!

	Thread 3 "gout2" hit Breakpoint 2, error_maker () at test2.c:27
	27              printf("str address ==> %p\n", str);

	(gdb) x str
	0x7fffe80008c0: 0x00000000

在让程序执行一步,观察程序中打出来的地址也是0x7fffe80008c0

	(gdb) n
	start_routine1, 41(value = 1)
	str address ==> 0x7fffe80008c0
	30              *str = 'x';

## 例子4

给指定文件中的函数打断点

	$ gdb gtou4
	(gdb) b main.c:stub_func
	(gdb) b foo.c:foo_func
	(gdb) b bar.c:bar_func
	(gdb) r

给程序传递参数,传递两个参数(abc, 123)

	$ gdb --args gout4 abc 123

查看参数个数和所有参数,argv是个数组

	(gdb) p argc
	$5 = 3
	(gdb) p *argv@3
	$6 = {0x7fffffffec5c "gout4", 0x7fffffffec80 "abc", 0x7fffffffec84 "123"}

或者是进入gdb后通过(set args来传递)

	(gdb) set args abc 123

使用条件断点(其中condition里只能判断整数,详细见代码[main.c](./main.c))

	(gdb) b line_number if condition

## 例子5 [set_var and return](./set_var_return.c)

	(gdb) b main
	Breakpoint 1 at 0x40055b: file set_var.c, line 14.
	(gdb) r

	Breakpoint 1, main (argc=1, argv=0x7fffffffea78) at set_var.c:14
	14              a = 911;

使用set var来改变变量的值

	(gdb) n
	16              ret = foo(a);
	(gdb) s
	foo (val=911) at set_var.c:5
	5               printf("val = %d\n", val);
	(gdb) p val
	$1 = 911
	(gdb) set val = 811

使用return来迅速返回函数

	(gdb) return 2
	Make foo return now? (y or n) y
	#0  0x000000000040056c in main (argc=1, argv=0x7fffffffea78) at set_var.c:16
	16              ret = foo(a);
	(gdb) n
	17              printf("ret = %d\n", ret);
	(gdb) p ret
	$2 = 2

## 例子6 打印数组和遍历数组

可以使用下面的格式打印数组array的值,假设数组长度为len

	(gdb) p *array@len

例子[convenience.c](./convenience.c)中打印数组

	(gdb) p *people10@10

或直接使用头指针

	(gdb) p people10

使用convenience变量来操作,先设置一个环境变量index值未0

	(gdb) set $index = 0

使用这个环境变量index来操作

	(gdb) print people10[$index++].name
	$1 = "anonymous_0\000\001\000\000\000\000\000\000"

之后每次按回车即可不断遍历数组

	(gdb)
	$2 = "anonymous_1\000\020ԡ\367\377\177\000"
	(gdb)
	$3 = "anonymous_2\000\000\000\000\000\000\000\377"
	(gdb)
	$4 = "anonymous_3\000\000\000\000\000\000\000\000"
	...

使用show valuse查看历史值

	(gdb) show values
	$9 = "anonymous_1\000\020ԡ\367\377\177\000"
	$10 = "anonymous_2\000\000\000\000\000\000\000\377"
	$11 = "anonymous_3\000\000\000\000\000\000\000\000"
	$12 = "anonymous_4\000\000\000\000\000\000\000\000"
	$13 = "anonymous_5\000\000\000\000\000\000\000\000\377"
	$14 = "anonymous_6\000\000\000\000\000\000\000\000"
	$15 = "anonymous_7\000\000\000\000\000\000\000\000"
	$16 = "anonymous_8\000\001\000\000\000\000\000\000"
	$17 = "anonymous_9\000\000\000\000\000\000\000\000"

另外可以通过设置下面两个属性让结构体显示更清新

	(gdb) set print array on
	(gdb) p people10

	(gdb) set print pretty on
	(gdb) p people10

## 例子7 使用display观察变量变化过程[源码variables.c](./variables.c)

	(gdb) b do_something
	(gdb) r
	(gdb) display i
	(gdb) display j
	(gdb) n

## 例子8 定位死锁问题

[dead lock debug using gdb](./dead_lock.md)

## 例子9 (例子8在android上测试)

[如何使用GDB调试android应用程序](https://github.com/54shady/kernel_drivers_examples/blob/rk3288/debug/android_gdb.md)

在android设备上运行程序

	gdbserver :1234 astall

在调试主机上调试

	(gdb) source android_gdbenv.cmd
	(gdb) source android_debug.cmd

结果发现在android输出的结果并不理想(和PC上不太一致?)

	Breakpoint 1 at 0xb6f1d3d2: file bionic/libc/bionic/pthread_mutex.cpp, line 503.
	Breakpoint 2 at 0xb6f1d424: file bionic/libc/bionic/pthread_mutex.cpp, line 524.

	Breakpoint 2, pthread_mutex_unlock (mutex_interface=0xb6f30208 <__dl__ZL21g_strerror_tls_buffer>)
		at bionic/libc/bionic/pthread_mutex.cpp:524
	524	int pthread_mutex_unlock(pthread_mutex_t* mutex_interface) {
	#0  pthread_mutex_unlock (mutex_interface=0xb6f30208 <__dl__ZL21g_strerror_tls_buffer>) at bionic/libc/bionic/pthread_mutex.cpp:524
	#1  0xb6f11592 in soinfo::call_function (this=0xb6f30224 <__dl__ZL7key_map>, function_name=0x0, function=0x0)
		at bionic/linker/linker.cpp:2235
	#2  0x00000000 in ?? ()

	Breakpoint 2, pthread_mutex_unlock (mutex_interface=0xb6f30f68 <__dl__ZL18g_group_tls_buffer>)
		at bionic/libc/bionic/pthread_mutex.cpp:524
	524	int pthread_mutex_unlock(pthread_mutex_t* mutex_interface) {
	#0  pthread_mutex_unlock (mutex_interface=0xb6f30f68 <__dl__ZL18g_group_tls_buffer>) at bionic/libc/bionic/pthread_mutex.cpp:524
	#1  0xb6f1073a in __dl__GLOBAL__sub_I_stubs.cpp ()
	   from /home/zeroway/rk3288-e810-android6.0/out/target/product/Gemini-E800/symbols/system/bin/linker
	#2  0x00000000 in ?? ()

	Breakpoint 2, pthread_mutex_unlock (mutex_interface=0xb6f30f6c <__dl__ZL19g_passwd_tls_buffer>)
		at bionic/libc/bionic/pthread_mutex.cpp:524
	524	int pthread_mutex_unlock(pthread_mutex_t* mutex_interface) {
	#0  pthread_mutex_unlock (mutex_interface=0xb6f30f6c <__dl__ZL19g_passwd_tls_buffer>) at bionic/libc/bionic/pthread_mutex.cpp:524
	#1  0xb6f11592 in soinfo::call_function (this=0xb6f30224 <__dl__ZL7key_map>, function_name=0x10 <Address 0x10 out of bounds>,
		function=0x0) at bionic/linker/linker.cpp:2235
	#2  0x00000000 in ?? ()

	Breakpoint 2, pthread_mutex_unlock (mutex_interface=0xb6f30f60 <__dl__ZL15g_uselocale_key>) at bionic/libc/bionic/pthread_mutex.cpp:524
	524	int pthread_mutex_unlock(pthread_mutex_t* mutex_interface) {
	#0  pthread_mutex_unlock (mutex_interface=0xb6f30f60 <__dl__ZL15g_uselocale_key>) at bionic/libc/bionic/pthread_mutex.cpp:524
	#1  0xb6f2155c in mbstate_set_byte (ps=0xb6f30f60 <__dl__ZL15g_uselocale_key>, i=0, byte=0 '\000') at bionic/libc/bionic/mbstate.cpp:41
	#2  0xb6f2155c in mbstate_set_byte (ps=0xb6f30f60 <__dl__ZL15g_uselocale_key>, i=0, byte=0 '\000') at bionic/libc/bionic/mbstate.cpp:41
	Backtrace stopped: previous frame identical to this frame (corrupt stack?)

	Program received signal SIGINT, Interrupt.
	syscall () at bionic/libc/arch-arm/bionic/syscall.S:44
	44	    swi     #0
	A debugging session is active.

		Inferior 1 [process 1973] will be killed.

	Quit anyway? (y or n) [answered Y; input not from terminal]
## 例子10 使用ignore跳过断点[源码variables.c](./variables.c)

打上调试断点,查看断点信息

	(gdb) b do_something
	Breakpoint 1 at 0x400531: file variables.c, line 7.
	(gdb) i b
	Num     Type           Disp Enb Address            What
	1       breakpoint     keep y   0x0000000000400531 in do_something at variables.c:7

使用ignore来忽略断点1,忽略5次,再次查看断点信息

	(gdb) ignore 1 5
	Will ignore next 5 crossings of breakpoint 1.
	(gdb) i b
	Num     Type           Disp Enb Address            What
	1       breakpoint     keep y   0x0000000000400531 in do_something at variables.c:7
			ignore next 5 hits

执行程序

	(gdb) r
	Starting program: gout7
	do_something, 7[0]
	do_something, 7[1]
	do_something, 7[2]
	do_something, 7[3]
	do_something, 7[4]

## 例子11 使用commands设置断点执行脚本[源码variables.c](./variables.c)

设置调试断点,查看断点信息

	(gdb) b do_something
	Breakpoint 1 at 0x400531: file variables.c, line 7.
	(gdb) i b
	Num     Type           Disp Enb Address            What
	1       breakpoint     keep y   0x0000000000400531 in do_something at variables.c:7

使用commands设置断点1执行命令,并查看断点信息

	(gdb) commands 1
	Type commands for breakpoint(s) 1, one per line.
	End with a line saying just "end".
	>p time
	>c
	>end
	(gdb) i b
	Num     Type           Disp Enb Address            What
	1       breakpoint     keep y   0x0000000000400531 in do_something at variables.c:7
			p time
			c

### 使用command配合break来修改代码中的变量

[参考例子setvar.c](./setvar.c)

## 例子12 多线程调试(控制线程运行)[multi_thread.c](./multi_thread.c)

多线程调试中,有时需要将其他不关心的线程停止,只运行关心的线程

查看当前锁定线程的模式

	show scheduler-locking

- off：不锁定任何线程,也就是所有线程都执行，这是默认值
- on：只有当前线程会执行
- step：在单步的时候,除了next过一个函数的情况(熟悉情况的人可能知道，这其实是一个设置断点然后continue的行为)以外，只有当前线程会执行

设置线程锁定模式

	set scheduler-locking [off|on|step]

例如这个实例中只想调试线程2

	(gdb) start
	(gdb) c

查看线程情况

	(gdb) i threads
	  Id   Target Id         Frame
	* 1    Thread 0x7ffff7fcf700 (LWP 7375) "gout9" 0x00007ffff7bc298d in pthread_join (threadid=140737345681152, thread_return=0x0)
		at pthread_join.c:90
	  2    Thread 0x7ffff77ef700 (LWP 7379) "gout9" 0x00007ffff78bc30d in nanosleep () at ../sysdeps/unix/syscall-template.S:84
	  3    Thread 0x7ffff6fee700 (LWP 7380) "gout9" 0x00007ffff78bc30d in nanosleep () at ../sysdeps/unix/syscall-template.S:84

切换到线程2

	(gdb) thread 2
	[Switching to thread 2 (Thread 0x7ffff77ef700 (LWP 7379))]
	#0  0x00007ffff78bc30d in nanosleep () at ../sysdeps/unix/syscall-template.S:84
	84      ../sysdeps/unix/syscall-template.S: No such file or directory.

开启当线程调试模式,再次执行就只有一个指定线程在跑

	(gdb) set scheduler-locking on
	(gdb) c
	Continuing.
	***start_routine1, (value = 1)

## 例子13 信号处理

在一个终端执行程序,另一个终端给程序发送信号SIGINT

	./gouta
	kill -2 `pidof gouta`

在GDB调试中给程序发送信号

	(gdb) file gouta
	Reading symbols from gouta...done.
	(gdb) r
	Starting program: /linuxc/gdb/gouta
	Runing...
	Runing...
	^C		<====这里ctr_c停止程序
	Program received signal SIGINT, Interrupt.
	0x00007ffff7ad92f0 in __nanosleep_nocancel () at ../sysdeps/unix/syscall-template.S:84
	84      ../sysdeps/unix/syscall-template.S: No such file or directory.
	(gdb) signal 2		<====发送信号2
	Continuing with signal SIGINT.
	Singal handler processing...
	Signal:: Interrupt
	Runing...
	Runing...

## [例子14 动态库和静态库调试](./libdebug)
## [例子15 多进程调试](./multi_process.md)
## [例子16 同时调试多个应用程序](./multi_program)
## [例子17 符号表的使用方法](./symbol_file.md)
## [gentoo 上使用gdb调试](gentoo_gdb.md)
## 调试过程中的其它问题

调试过程中遇到找不到libc库文件

	Thread 2.1 "goutc" received signal SIGINT, Interrupt.
	0x00007ffff78bc2f0 in __nanosleep_nocancel () at
	../sysdeps/unix/syscall-template.S:84
	84      ../sysdeps/unix/syscall-template.S: No such file or directory.

可以下载libc源代码,添加到GDB搜索目录即可

	(gdb) directory glibc-2.23/
	(gdb) show directories
	Source directories searched: /glibc-2.23:$cdir:$cwd

添加后的结果如下,不报找不到文件的问题

	Thread 2.1 "goutc" received signal SIGINT, Interrupt.
	0x00007ffff78bc2f0 in __nanosleep_nocancel () at
	../sysdeps/unix/syscall-template.S:84
	84      in ../sysdeps/unix/syscall-template.S
