#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/cdefs.h>

#define SMP_CACHE_BYTES 64

volatile struct share_struct {
	unsigned int a;
#ifndef CACHELINE_ALIGNED
	unsigned int b;
#else
	//unsigned int b __attribute__((__aligned__(SMP_CACHE_BYTES)));
	//unsigned int b __attribute__((___cacheline_aligned));
	/*
	 * getconf -a
	 * getconf LEVEL1_DCACHE_LINESIZE 查询到的data cache line 大小是64
	 * 64
	 */
	unsigned int b __attribute__((aligned(64))); // TODO 这里为什么设置成64
#endif
};

struct share_struct shared;
void *start_routine1(void *p)
{
	unsigned int val;

	do {
		val = shared.a;
		(void)val;

	} while (1);

	//return (void *)0;
	pthread_exit((void *)0);
}

void *start_routine2(void *p)
{

	unsigned int val, i = 0;

	do {
		shared.b += i;
		i++;
	} while (1);

	//return (void *)0;
	pthread_exit((void *)0);
}

#if 0
参考 Using perf on arm platforms

Test on rk3588

	gcc demo.c -lpthread -o demo
	./demo

首先收集全局系统的性能数据

	perf_5.10 record -a -e cache-references,cache-misses -- sleep 10

查看是什么程序cache miss高,下面数据可以看到是demo应用程序里对应的两个线程

	perf_5.10 report --stdio

	# To display the perf.data header info, please use --header/--header-only options.
	#
	#
	# Total Lost Samples: 0
	#
	# Samples: 161K of event 'cache-references'
	# Event count (approx.): 34019959751
	#
	# Overhead  Command          Shared Object             Symbol
	# ........  ...............  ........................  .................................................
	#
			44.13%  demo             demo                      [.] start_routine2
			41.74%  demo             demo                      [.] start_routine1
			 0.27%  ls               ld-2.31.so                [.] do_lookup_x
			 0.22%  swapper          [kernel.kallsyms]         [k] _raw_spin_unlock_irq
			 0.22%  sh               [kernel.kallsyms]         [k] next_uptodate_page
			 0.21%  ls               [kernel.kallsyms]         [k] next_uptodate_page
			 0.19%  sh               [kernel.kallsyms]         [k] unmap_page_range
			 0.19%  ls               [kernel.kallsyms]         [k] unmap_page_range
			 0.17%  ls               ld-2.31.so                [.] _dl_relocate_object
			...
			... 下面数据在后半部分
	# Samples: 159K of event 'cache-misses'
	# Event count (approx.): 466534832
	#
	# Overhead  Command          Shared Object             Symbol
	# ........  ...............  ........................  .................................................
	#
				29.24%  demo             demo                      [.] start_routine2
				29.23%  demo             demo                      [.] start_routine1
				 1.08%  swapper          [kernel.kallsyms]         [k] _raw_spin_unlock_irq
				 0.89%  ls               ld-2.31.so                [.] do_lookup_x
				 0.76%  sh               [kernel.kallsyms]         [k] _raw_spin_unlock_irqrestore
				 0.54%  ls               ld-2.31.so                [.] _dl_lookup_symbol_x
				 0.44%  ls               ld-2.31.so                [.] strcmp

直接统计下全局的cache miss率(100次访问cache,有1.348次会miss)

	perf_5.10 stat -a -e cache-references,cache-misses -- sleep 10

	Performance counter stats for 'system wide':

		34200920992      cache-references
		  460990855      cache-misses              #    1.348 % of all cache refs

	10.002385770 seconds time elapsed

这里有个有意思的事是，当没有运行demo程序是，系统全局由于访问cache总数不过，但是miss率却达到3.716%,所以不能光看比率,还要比总数

	perf_5.10 stat -a -e cache-references,cache-misses -- sleep 10

	Performance counter stats for 'system wide':

		3763809824      cache-references
		 139868725      cache-misses              #    3.716 % of all cache refs

	10.005543006 seconds time elapsed

编译一个使能cache line aligned的程序再测试,发下cache miss率下降

	gcc -DCACHELINE_ALIGNED demo.c -lpthread -o demo-a
	./demo-a

	perf_5.10 stat -a -e cache-references,cache-misses -- sleep 10

	Performance counter stats for 'system wide':

       55371813321      cache-references
         198114143      cache-misses              #    0.358 % of all cache refs

      10.003022489 seconds time elapsed

上面可以对比看出,使能了cache line aligned后system wide的cache miss率从1.348降低到了0.358

同样可以比对指定进程的cache miss率也是从0.949降到了0.001

	perf_5.10 stat -a -e cache-references,cache-misses -p `pidof demo-a` sleep 10

	 Performance counter stats for process id '76547':

		   49911634013      cache-references
				263118      cache-misses              #    0.001 % of all cache refs

		  10.002514356 seconds time elapsed

	perf_5.10 stat -a -e cache-references,cache-misses -p `pidof demo` sleep 10

	 Performance counter stats for process id '199096':

		   28987918719      cache-references
			 274952074      cache-misses              #    0.949 % of all cache refs

		  10.002989110 seconds time elapsed

--------------
	  同样的代码在x86上测试的结果却刚好相反,开启了cache line aligned反而cache miss更高

	  Test on x86 linux(ubuntu 22.04)

		./demo
	  sudo perf stat -a -e cache-references,cache-misses -- sleep 10

	 Performance counter stats for 'system wide':

		   396,029,383      cpu_core/cache-references/
			50,379,837      cpu_atom/cache-references/
			19,708,459      cpu_core/cache-misses/           #    4.98% of all cache refs
			14,516,746      cpu_atom/cache-misses/           #    3.67% of all cache refs

		  10.002234728 seconds time elapsed

	./demo-a
	sudo perf stat -a -e cache-references,cache-misses -- sleep 10

	 Performance counter stats for 'system wide':

			62,361,178      cpu_core/cache-references/
			49,364,735      cpu_atom/cache-references/
			17,731,427      cpu_core/cache-misses/           #   28.43% of all cache refs
			13,397,570      cpu_atom/cache-misses/           #   21.48% of all cache refs

		  10.001857423 seconds time elapsed
#endif

int main(int argc, char *argv[])
{
	int err;
	pthread_t rabbit, turtle;

	/* create two threads */
	if ((err = pthread_create(&rabbit, NULL, start_routine1, NULL)) != 0)
	{
		perror("Pthread create error\n");
		exit(1);
	}
	if ((err = pthread_create(&turtle, NULL, start_routine2, NULL)) != 0)
	{
		perror("Pthread create error\n");
		exit(1);
	}

	/* wait for threads exit */
	pthread_join(rabbit, NULL);
	pthread_join(turtle, NULL);

	return 0;
}
