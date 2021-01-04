#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* print out the thread status */
void printout_status(pthread_attr_t *attr)
{
	int state;

	if (pthread_attr_getdetachstate(attr, &state) != 0)
		perror("get attribute error");
	else
	{
		if (state == PTHREAD_CREATE_JOINABLE)
			printf("Joinable mode\n");
		else if (state == PTHREAD_CREATE_DETACHED)
			printf("Detached mode\n");
		else
			printf("Error state\n");
	}
}

/* thread routine */
void *start_routine(void *arg)
{
	int i;
	int sum = 0;

	for (i = 0; i < 101; i++)
		sum += i;

	return (void *)sum;
}

int main(int argc, char *argv[])
{
	int err;
	int res;
	pthread_t default_th, detach_th;

	pthread_attr_t attr;

	/* init attribute */
	pthread_attr_init(&attr);

	/* printout_status */
	printout_status(&attr);

	/* start thread with normal attribute */
	if ((err = pthread_create(&default_th, &attr,
					start_routine, (void *)0)) != 0)
		perror("pthread create error");

	/* wait sub thread finished */
	/* 调用join来主动回收子线程的资源 */
	if ((err = pthread_join(default_th,
					(void *)&res)) != 0)
		perror("pthread join error");
	else
		printf("normal return is %d\n", res);

	printf("==============================\n");

	/* setup attribuite with detach */
	/* 网络编程中使用较多 */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	printout_status(&attr);

	/* start thread with detach attribute */
	/* 分离状态启动的子线程会自动回收资源 */
	if ((err = pthread_create(&detach_th, &attr,
					start_routine, (void *)0)) != 0)
		perror("pthread create error");

#if 0 /* 以分离状态启动的子线程不需要调用join来回收子线程的资源*/
	/* wait sub thread finished */
	if ((err = pthread_join(detach_th,
					(void *)&res)) != 0)
		fprintf(stderr, "%s\n", strerror(err));
	else
		printf("detach return is %d\n", res);
#endif

	/* destroy attribute */
	pthread_attr_destroy(&attr);

	printf("0x%lx finished\n", pthread_self());
	sleep(1);

	return 0;
}
