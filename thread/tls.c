#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define THREAD_NR 8

#ifdef SHARE_VALUE_TEST
/* 所有线程共享一个变量 */
int share_value = 911;
#else
/* 所有线程有各自的一个变量 */
/* Thread Local Storage(TLS) */
__thread int share_value = 911;
#endif

void *start_routine(void *p)
{
	pthread_mutex_t mutex = *(pthread_mutex_t *)p;

	pthread_mutex_trylock(&mutex);
	share_value += 1;
	printf("%d:%p[%d]\n", gettid(), &share_value, share_value);
	pthread_mutex_unlock(&mutex);

	pthread_exit((void *)0);
}

int main(int argc, char *argv[])
{
	int err;
	int i;
	pthread_t tids[THREAD_NR];
	pthread_mutex_t mutex;

	pthread_mutex_init(&mutex, NULL);

	for (i = 0; i < THREAD_NR; i++)
	{
		if ((err = pthread_create(&tids[i], NULL,
						start_routine, (void *)&mutex)) != 0)
		{
			perror("Pthread create error\n");
			exit(1);
		}
	}

	pthread_mutex_trylock(&mutex);
	share_value += 1;
	printf("%d:%p[%d]\n", getpid(), &share_value, share_value);
	pthread_mutex_unlock(&mutex);

	/* wait for threads exit */
	for (i = 0; i < THREAD_NR; i++)
		pthread_join(tids[i], NULL);

	pthread_mutex_destroy(&mutex);

	return 0;
}
