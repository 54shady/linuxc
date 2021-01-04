#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/* Fix compile error */
#include <sys/syscall.h>
#include <sys/types.h>
#define gettid() syscall(__NR_gettid)

#define TRY_COUNT 10
static int g_cnt = 1;

void init_routine(void)
{
	printf("%s, Times:%d\n", __FUNCTION__, g_cnt++);

	return;
}

void *start_routine(void *p)
{
	printf("<%ld>, Times:%d\n", gettid(), g_cnt++);

	pthread_exit((void *)0);
}

int main(int argc, char *argv[])
{
	int i;
	int err;
	pthread_once_t once_control = PTHREAD_ONCE_INIT;
	pthread_t tids[TRY_COUNT];
	int runonce = 0;

	if (argc < 2)
	{
		fprintf(stderr, "%s <0 | 1>\n", argv[0]);
		fprintf(stderr, "0 : normal run\n");
		fprintf(stderr, "1 : run one time\n");
		exit(-1);
	}

	runonce = atoi(argv[1]);

	for (i = 0; i < TRY_COUNT; i++)
	{
		if (runonce)
		{
			if ((err = pthread_once(&once_control, init_routine)) != 0)
			{
				perror("Pthread create error\n");
				exit(1);
			}
		}
		else
		{
			if ((err = pthread_create(&tids[i], NULL, start_routine, NULL)) != 0)
			{
				perror("Pthread create error\n");
				exit(1);
			}
		}
	}

	/*
	 * wait for threads exit
	 * make sure all the threads run well
	 */
	if (!runonce)
		for (i = 0; i < TRY_COUNT; i++)
			pthread_join(tids[i], NULL);

	return 0;
}
