# Linux Signal

[Linux信号详解](http://kenby.iteye.com/blog/1173862)

## 信号的种类

- 可靠信号与不可靠信号, 实时信号与非实时信号
- 可靠信号就是实时信号, 那些从UNIX系统继承过来的信号都是非可靠信号, 表现在信号 不支持排队,信号可能会丢失, 比如发送多次相同的信号, 进程只能收到一次. 信号值小于 SIGRTMIN的都是非可靠信号.
- 非可靠信号就是非实时信号, 后来, Linux改进了信号机制, 增加了32种新的信号, 这些信 号都是可靠信号, 表现在信号支持排队, 不会丢失, 发多少次, 就可以收到多少次. 信号值 位于 [SIGRTMIN, SIGRTMAX] 区间的都是可靠信号.

关于可靠信号, 还可以参考WIKI的一段话

	 The real-time signals, ranging from SIGRTMIN to SIGRTMAX, are a set of signals that can be used for application-defined purposes.
	Because SIGRTMIN may have different values on different Unix-like systems, applications should always refer to the signals in the form SIGRTMIN+n, where n is a constant integer expression.
	The real-time signals have a number of properties that differentiate them from other signals and make them suitable for application-defined purposes:
	* Multiple instances of a real-time signal can be sent to a process and all will be delivered.
	* Real-time signals can be accompanied by an integer or pointer value (see sigqueue[2]).
	* Real-time signals are guaranteed to be delivered in the order they were emitted.

命令行输入 kill -l, 可以列出系统支持的所有信号

	 1) SIGHUP	 2) SIGINT	 3) SIGQUIT	 4) SIGILL	 5) SIGTRAP
	 6) SIGABRT	 7) SIGBUS	 8) SIGFPE	 9) SIGKILL	10) SIGUSR1
	11) SIGSEGV	12) SIGUSR2	13) SIGPIPE	14) SIGALRM	15) SIGTERM
	16) SIGSTKFLT	17) SIGCHLD	18) SIGCONT	19) SIGSTOP	20) SIGTSTP
	21) SIGTTIN	22) SIGTTOU	23) SIGURG	24) SIGXCPU	25) SIGXFSZ
	26) SIGVTALRM	27) SIGPROF	28) SIGWINCH	29) SIGIO	30) SIGPWR
	31) SIGSYS	34) SIGRTMIN	35) SIGRTMIN+1	36) SIGRTMIN+2	37) SIGRTMIN+3
	38) SIGRTMIN+4	39) SIGRTMIN+5	40) SIGRTMIN+6	41) SIGRTMIN+7	42) SIGRTMIN+8
	43) SIGRTMIN+9	44) SIGRTMIN+10	45) SIGRTMIN+11	46) SIGRTMIN+12	47) SIGRTMIN+13
	48) SIGRTMIN+14	49) SIGRTMIN+15	50) SIGRTMAX-14	51) SIGRTMAX-13	52) SIGRTMAX-12
	53) SIGRTMAX-11	54) SIGRTMAX-10	55) SIGRTMAX-9	56) SIGRTMAX-8	57) SIGRTMAX-7
	58) SIGRTMAX-6	59) SIGRTMAX-5	60) SIGRTMAX-4	61) SIGRTMAX-3	62) SIGRTMAX-2
	63) SIGRTMAX-1	64) SIGRTMAX

非可靠信号一般都有确定的用途及含义,  可靠信号则可以让用户自定义使用

## 信号的安装

### signal安装信号

早期的Linux使用系统调用 signal 来安装信号

	#include <signal.h>
	void (*signal(int signum, void (*handler))(int)))(int);

该函数有两个参数, signum指定要安装的信号, handler指定信号的处理函数.
该函数的返回值是一个函数指针, 指向上次安装的handler

经典安装方式

	if (signal(SIGINT, SIG_IGN) != SIG_IGN) {
		signal(SIGINT, sig_handler);
	}

先获得上次的handler, 如果不是忽略信号, 就安装此信号的handler

由于信号被交付后, 系统自动的重置handler为默认动作, 为了使信号在handler

处理期间, 仍能对后继信号做出反应, 往往在handler的第一条语句再次调用 signal

	sig_handler(ing signum)
	{
		/* 重新安装信号 */
		signal(signum, sig_handler);
		......
	}

我们知道在程序的任意执行点上, 信号随时可能发生, 如果信号在sig_handler重新安装

信号之前产生, 这次信号就会执行默认动作, 而不是sig_handler. 这种问题是不可预料的.

### sigaction安装信号

使用库函数 sigaction  来安装信号

为了克服非可靠信号并同一SVR4和BSD之间的差异, 产生了 POSIX 信号安装方式, 使用

sigaction安装信号的动作后, 该动作就一直保持, 直到另一次调用 sigaction建立另一个

动作为止. 这就克服了古老的 signal 调用存在的问题

	#include <signal.h>
	int sigaction(int signum,const struct sigaction *act,struct sigaction *oldact));

经典安装方式

	struct sigaction action, old_action;

	/* 设置SIGINT */
	action.sa_handler = sig_handler;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGTERM);
	action.sa_flags = 0;

	/* 获取上次的handler, 如果不是忽略动作, 则安装信号 */
	sigaction(SIGINT, NULL, &old_action);
	if (old_action.sa_handler != SIG_IGN) {
		sigaction(SIGINT, &action, NULL);
	}

基于 sigaction 实现的库函数: signal

sigaction 自然强大, 但安装信号很繁琐, 目前linux中的signal()是通过sigaction()函数

实现的，因此，即使通过signal()安装的信号，在信号处理函数的结尾也不必

再调用一次信号安装函数.

## 如何屏蔽信号

所谓屏蔽, 并不是禁止递送信号, 而是暂时阻塞信号的递送,

解除屏蔽后, 信号将被递送, 不会丢失. 相关API为

	int sigemptyset(sigset_t *set);
	int sigfillset(sigset_t *set);
	int sigaddset(sigset_t *set, int signum);
	int sigdelset(sigset_t *set, int signum);
	int sigismember(const sigset_t *set, int signum);
	int sigsuspend(const sigset_t *mask);
	int sigpending(sigset_t *set);
	int sigprocmask(int  how,  const  sigset_t *set, sigset_t *oldset));

sigprocmask()函数能够根据参数how来实现对信号集的操作，操作主要有三种：

	SIG_BLOCK	在进程当前阻塞信号集中添加set指向信号集中的信号
	SIG_UNBLOCK	如果进程阻塞信号集中包含set指向信号集中的信号，则解除
	 对该信号的阻塞
	SIG_SETMASK	更新进程阻塞信号集为set指向的信号集

屏蔽整个进程的信号

	example1.c

屏蔽其它信号

	example2.c

## 如何获取未决信号

所谓未决信号, 是指被阻塞的信号, 等待被递送的信号

	int sigsuspend(const sigset_t *mask));

	sigpending(sigset_t *set))获得当前已递送到进程，

却被阻塞的所有信号，在set指向的信号集中返回结果。

	example3.c

## 被中断了的系统调用

一些IO系统调用执行时, 如 read 等待输入期间, 如果收到一个信号,

系统将中断read, 转而执行信号处理函数. 当信号处理返回后, 系统

遇到了一个问题: 是重新开始这个系统调用, 还是让系统调用失败?

早期UNIX系统的做法是, 中断系统调用, 并让系统调用失败, 比如read

返回 -1, 同时设置 errno 为 EINTR

中断了的系统调用是没有完成的调用, 它的失败是临时性的, 如果再次调用

则可能成功, 这并不是真正的失败, 所以要对这种情况进行处理, 典型的方式为:

	while (1) {
		n = read(fd, buf, BUFSIZ);
		if (n == -1 && errno != EINTR) {
			printf("read error\n");
			break;
		}
		if (n == 0) {
			printf("read done\n");
			break;
		}
	}

这样做逻辑比较繁琐, 事实上, 我们可以从信号的角度

来解决这个问题,  安装信号的时候, 设置 SA_RESTART属性,

那么当信号处理函数返回后, 被该信号中断的系统 调用将自动恢复.

	example4.c

## 非局部控制转移

API

	int setjmp(jmp_buf env);
	int sigsetjmp(sigjmp_buf env, int savesigs);
	void longjmp(jmp_buf env, int val);
	void siglongjmp(sigjmp_buf env, int val);

setjmp()会保存目前堆栈环境，然后将目前的地址作一个记号，

而在程序其他地方调用 longjmp 时便会直接跳到这个记号位置，

然后还原堆栈，继续程序好执行。

setjmp调用有点fork的味道, setjmp()return 0 if returning directly,

and non-zero when returning from longjmp using  the saved context.

	if (setjmp(jmpbuf)) {
	   printf("return from jmp\n");
	} else {
	   printf("return directly\n");
	}

setjmp 和 sigsetjmp 的唯一区别是: setjmp 不一定会恢复信号集合,

而sigsetjmp可以保证恢复信号集合

	example5.c

## 信号的生命周期

从信号发送到信号处理函数的执行完毕

对于一个完整的信号生命周期(从信号发送到相应的处理函数执行完毕)来说，

可以分为三个重要的阶段，这三个阶段由四个重要事件来刻画：

信号诞生；信号在进程中注册完毕；信号在进程中的注销完毕；信号处理函数执行完毕。

下面阐述四个事件的实际意义：

### 信号诞生

信号的诞生指的是触发信号的事件发生

（如检测到硬件异常、定时器超时以及调用信号发送函数

kill()或sigqueue()等）。信号在目标进程中"注册"；

进程的task_struct结构中有关于本进程中未决信号的数据成员：

	struct sigpending pending：
	struct sigpending{
		struct sigqueue *head, **tail;
		sigset_t signal;
	};

第三个成员是进程中所有未决信号集，第一、第二个成员分别指向一个

sigqueue类型的结构链（称之为"未决信号链表"）的首尾，链表中

的每个sigqueue结构刻画一个特定信号所携带的信息，并指向下一个

sigqueue结构:

	struct sigqueue{
		struct sigqueue *next;
		siginfo_t info;
	}

### 信号的注册

信号在进程中注册指的就是信号值加入到进程的未决信号集中

（sigpending结构的第二个成员sigset_t signal），

并且加入未决信号链表的末尾。 只要信号在进程的未决信号集中，

表明进程已经知道这些信号的存在，但还没来得及处理，或者该信号被进程阻塞。

当一个实时信号发送给一个进程时，不管该信号是否已经在进程中注册，

都会被再注册一次，因此，信号不会丢失，因此，实时信号又叫做"可靠信号"。

这意味着同一个实时信号可以在同一个进程的未决信号链表中添加多次.

当一个非实时信号发送给一个进程时，如果该信号已经在进程中注册，

则该信号将被丢弃，造成信号丢失。因此，非实时信号又叫做"不可靠信号"。

这意味着同一个非实时信号在进程的未决信号链表中，至多占有一个sigqueue结构.
一个非实时信号诞生后，

（1）、如果发现相同的信号已经在目标结构中注册，则不再注册，对于进程来说，

相当于不知道本次信号发生，信号丢失.

（2）、如果进程的未决信号中没有相同信号，则在进程中注册自己。


### 信号的注销

在进程执行过程中，会检测是否有信号等待处理

（每次从系统空间返回到用户空间时都做这样的检查）。如果存在未决

信号等待处理且该信号没有被进程阻塞，则在运行相应的信号处理函数前，

进程会把信号在未决信号链中占有的结构卸掉。是否将信号从进程未决信号集

中删除对于实时与非实时信号是不同的。对于非实时信号来说，由于在未决信

号信息链中最多只占用一个sigqueue结构，因此该结构被释放后，应该把信

号在进程未决信号集中删除（信号注销完毕）；而对于实时信号来说，可能在

未决信号信息链中占用多个sigqueue结构，因此应该针对占用sigqueue结构

的数目区别对待：如果只占用一个sigqueue结构（进程只收到该信号一次），

则应该把信号在进程的未决信号集中删除（信号注销完毕）。否则，不应该在进程

的未决信号集中删除该信号（信号注销完毕）。

进程在执行信号相应处理函数之前，首先要把信号在进程中注销。

### 信号生命终止

进程注销信号后，立即执行相应的信号处理函数，执行完毕后，

信号的本次发送对进程的影响彻底结束。

## 关于可重入函数

在信号处理函数中应使用可重入函数

信号处理程序中应当使用可重入函数

（注：所谓可重入函数是指一个可以被多个任务调用的过程，

任务在调用时不必担心数据是否会出错）。因为进程在收到信号

后，就将跳转到信号处理函数去接着执行。如果信号处理函数中

使用了不可重入函数，那么信号处理函数可能会修改原来进程中

不应该被修改的数据，这样进程从信号处理函数中返回接着执行时，

可能会出现不可预料的后果。不可再入函数在信号处理函数中被视为

不安全函数。

### 满足下列条件的函数多数是不可再入的

	（1）使用静态的数据结构，如getlogin()，gmtime()，getgrgid()， getgrnam()，getpwuid()以及getpwnam()等等；
	（2）函数实现时，调用了malloc（）或者free()函数；
	（3）实现时使用了标准I/O函数的。The Open Group视下列函数为可再入的：
	_exit（）、access（）、alarm（）、cfgetispeed（）、cfgetospeed（）、
	cfsetispeed（）、cfsetospeed（）、chdir（）、chmod（）、chown（） 、
	close（）、creat（）、dup（）、dup2（）、execle（）、execve（）、
	fcntl（）、fork（）、fpathconf（）、fstat（）、fsync（）、getegid（）、
	geteuid（）、getgid（）、getgroups（）、getpgrp（）、getpid（）、
	getppid（）、getuid（）、kill（）、link（）、lseek（）、mkdir（）、
	mkfifo（）、 open（）、pathconf（）、pause（）、pipe（）、raise（）、
	read（）、rename（）、rmdir（）、setgid（）、setpgid（）、setsid（）、
	setuid（）、 sigaction（）、sigaddset（）、sigdelset（）、sigemptyset（）、
	sigfillset（）、sigismember（）、signal（）、sigpending（）、
	sigprocmask（）、sigsuspend（）、sleep（）、stat（）、sysconf（）、
	tcdrain（）、tcflow（）、tcflush（）、tcgetattr（）、tcgetpgrp（）、
	tcsendbreak（）、tcsetattr（）、tcsetpgrp（）、time（）、times（）、
	umask（）、uname（）、unlink（）、utime（）、wait（）、waitpid（）、
	write（）。

即使信号处理函数使用的都是"安全函数"，同样要注意进入处理函数时，

首先要保存errno的值，结束时，再恢复原值。因为，信号处理过程中，

errno值随时可能被改变。另外，longjmp()以及siglongjmp()没有被列为可重入函数，

因为不能保证紧接着两个函数的其它调用是安全的。

## Seven things to do with signals

- Ignore them

- Clean up and terminate

	void cleanup(int sigtype)
	{
		unlink("/tmp/myworkfile");
		kill(my_child_pid, SIGTERM);
		wait(NULL);
		exit(1);
	}

	int main(int argc, char *argv[])
	{
		signal(SIGTERM, cleanup);
		open("/tmp/myworkfile", O_RDWR | O_CREAT, 0644);

		my_child_pid = fork();

		/* Get on with some work... */

		return 0;
	}

- [Reconfigure on the fly](reconfigure.c)
- Report status dynamically

	void print_state_info(int signum)
	{
		printf("%d blocks copies\n", count);
	}

	int main(int argc, char *argv[])
	{
		signal(SIGUSR1, print_state_info);

		for (i = 0; i < A_BIG_NUMBER; i++)
			read_block();
			write_block();

		return 0;
	}

- Turn debugging on and off

	int debug_on = 0;
	void toggle_debug_flag(int signum)
	{
		debug_on ^= 1;
	}

	int main(int argc, char *argv[])
	{
		signal(SIGUSR1, toggle_debug_flag);

		/* Then sometime later in the code ... */

		if (debug_on)
			printf("something useful for debugging");

		return 0;
	}

- [Implement a timeout](timeout_impl.c)
- [Schedule periodic actions](schedule_periodic.c)
