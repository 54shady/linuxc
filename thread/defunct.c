#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/* Fix compile error */
#include <sys/syscall.h>
#include <sys/types.h>

int g_cnt = 0;
void *start_routine(void *p)
{
	while (1)
	{
		printf("<%d>, Times:%d\n", gettid(), g_cnt++);
		sleep(1);
	}
}

int main(int argc, char *argv[])
{
	int err;
	pthread_t tids;

	if ((err = pthread_create(&tids, NULL, start_routine, NULL)) != 0)
	{
		perror("Pthread create error\n");
		exit(1);
	}

	/*
	 * 主线程退出了(defunct)变成僵尸状态,但是子线程还在工作
	 * ps -eL | grep def-demo
	 */
	pthread_exit(NULL);

	return 0;
}
