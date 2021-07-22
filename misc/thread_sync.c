#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/*
 * Usage:
 * gcc -lpthread thread_sync.c
 */

static pthread_t g_thread_id;
static pthread_mutex_t g_pthread_mutex_t  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  g_pthread_cond_t = PTHREAD_COND_INITIALIZER;

static void *start_routine(void *p)
{
	while (1)
	{
		printf("sleeping...\n");
		pthread_mutex_lock(&g_pthread_mutex_t);
		pthread_cond_wait(&g_pthread_cond_t, &g_pthread_mutex_t);
		pthread_mutex_unlock(&g_pthread_mutex_t);

		printf("wake up now, do something...\n");
		/* assume do some things take 2 seconds */
		sleep(2);
		printf("done!\n");
	}

	return (void *)(0);
}

void wake_sleep_thread_up(void)
{
	pthread_mutex_lock(&g_pthread_mutex_t);
	pthread_cond_signal(&g_pthread_cond_t);
	pthread_mutex_unlock(&g_pthread_mutex_t);
}

int main(int argc, char *argv[])
{
	pthread_create(&g_thread_id, NULL, start_routine, NULL);

	/* main thread */
	while (1)
	{
		sleep(3);

		/* let's wake up the sub thread */
		wake_sleep_thread_up();
	}
	return 0;
}
