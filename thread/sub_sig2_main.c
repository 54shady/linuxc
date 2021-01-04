#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void sig_handler(int signo)
{
	printf("%s %lx\n", __FUNCTION__, pthread_self());

	if (signo == SIGALRM)
	{
		printf("Timeout...");
		alarm(2);
	}
}

void *start_routine(void *arg)
{
	int i;

	if (signal(SIGALRM, sig_handler) == SIG_ERR)
		perror("signal error");

	/* raise a alarm in 2s */
	alarm(2);

	for (i = 0; i < 100; i++)
	{
		printf("0x%lx ==> %d\n", pthread_self(), i);
		sleep(1);
	}

	return (void *)0;
}

int main(int argc, char *argv[])
{
	int err;
	pthread_t pid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	if ((err = pthread_create(&pid, &attr,
					start_routine, (void *)0)) != 0)
		perror("create thread error");

	/* 子线程中发出信号,居然会中断主线程 */
	while (1)
	{
		/*
		 * 可以看到主线程并非每10s打印一次
		 * 所以应该是被信号打断了
		 */
		printf("Main control thread 0x%lx is running\n", pthread_self());

		/* 因为这里调用了sleep函数,当接收到信号后会被唤醒 */
		sleep(10);
	}

	printf("Main control done.\n");

	pthread_attr_destroy(&attr);

	return 0;
}
