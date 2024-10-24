#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sched.h>
#include <pthread.h>

double waste_time(long n)
{
	double res = 0;
	long i = 0;
	while (i < n * 200000)
	{
		i++;
		res += sqrt(i);
	}
	return res;
}

void *start_routine(void *arg)
{
	cpu_set_t mask;
	int *cpu_index = (int*)arg;

	CPU_ZERO(&mask);
	CPU_SET(*cpu_index ,&mask);

	printf("thread run on cpu_%d \n",*cpu_index);

	if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) == -1)
	{
		printf("warning ! set affinity failed! %d \n", *cpu_index);
	}
	else
	{
		while (1)
		{
			printf ("result: %f\n", waste_time (1000));
		}
	}

	return NULL;
}

/*
 * 让应用或驱动独占cpu(这里是独占cpu 7)运行
 *
 * 将线程指定到具体cpu运行
 * gcc -lpthread -lm run_on_dedicated_cpu.c
 *
 * on rk3588 debian system:
 * gcc run_on_dedicated_cpu.c -lpthread -lm
 *
 * 1. 内核中开启配置
 *	CONFIG_CPU_ISOLATION=y
 *	CONFIG_NO_HZ_FULL=y
 *
 * 2. 内核启动参数中添加"isolcpus=7 nohz_full=7"(in bootargs or GRUB_CMDLINE_LINUX_DEFAULT)
 *
 * 上述配置将cpu 7 单独隔离开,系统调度器默认不会将应用程序调度到该核
 *
 * 修改后可以通过下面命令查看cpu使用情况
 * mpstat -P ALL 1
 *
 * 查看对应cpu的信息
 * cat /sys/devices/system/cpu/cpu7/uevent
 *
 * 此时因为应用(或驱动)任然可以通过绑定到指定cpu运行
 * 比如这里运行应用程序并绑定到7的话(./a.out 7)
 * 就能实现类似一个应用程序独占cpu使用的情况
 *
 * ps -eo pid,args:50,psr
 *
 * 查看1号进程的cpu亲和性
 * taskset -cp 1
 *
 * grep Cpus_allowed_list /proc/$$/status
 */
int main(int argc, char *argv[])
{
	pthread_t tid;
	int cpu_index;
	int num;

	if (argc != 2)
	{
		printf("./a.out <cpu_index>\n");
		return -1;
	}

	/* 获取CPU核数 */
	num = sysconf(_SC_NPROCESSORS_CONF);

	cpu_index = atoi(argv[1]);
	cpu_index %= num;
	pthread_create(&tid, NULL, (void *)start_routine, &cpu_index);

	pthread_join(tid ,NULL);

	printf("main thread exit.\n");

	return 0;
}
