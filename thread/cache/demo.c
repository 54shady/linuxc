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
