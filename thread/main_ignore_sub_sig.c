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

	return (void *)0; /* pthread_exit((void *)0) */
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

	/*
	 * 主线程中将alarm信号屏蔽掉
	 * 所以信号就会被子线程收到
	 */
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGALRM); /* main thread mask alarm signal */
	pthread_sigmask(SIG_SETMASK, &set, NULL);

	while (1)
	{
		printf("Main control thread 0x%lx is running\n", pthread_self());
		sleep(10);
	}

	printf("Main control done.\n");

	pthread_attr_destroy(&attr);

	return 0;
}
