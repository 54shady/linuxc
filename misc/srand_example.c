#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include <pthread.h>
#include <assert.h>
#include <stdbool.h>

static bool rd_set = false;
static pthread_mutex_t rd_mutex = PTHREAD_MUTEX_INITIALIZER;

struct iscsi_context {
	int a;
};

/* code from libiscsi */
static void iscsi_srand_init(struct iscsi_context *iscsi)
{
	unsigned int seed;
	int urand_fd;
	ssize_t rc;
	int err;

	if (rd_set) {
		/* fast case, seed has been set */
		return;
	}

	err = pthread_mutex_lock(&rd_mutex);
	assert(err == 0);

	if (rd_set) {
		/* another thread initialized it in the meantime */
		goto out;
	}

	urand_fd = open("/dev/urandom", O_RDONLY);
	if (urand_fd == -1) {
		goto fallback;
	}

	rc = read(urand_fd, &seed, sizeof(seed));
	close(urand_fd);
	if (rc == -1) {
		goto fallback;
	}

	srand(seed);
	goto out;

fallback:
	/* seed based on @iscsi */
	printf("Using fallback method\n");
	srand(getpid() ^ (unsigned int)((unsigned int *) iscsi));

out:
	rd_set = true;
	err = pthread_mutex_unlock(&rd_mutex);
	assert(err == 0);
}

int main(int argc, char *argv[])
{
	int ret;

	struct iscsi_context *iscsi;

	iscsi = malloc(sizeof(struct iscsi_context));
	if (!iscsi)
	{
		perror("No memory\n");
		return -1;
	}

#if 1
	/*
	 * 在一个应用程序中只需要初始化一次随机种子
	 * 且需要保证线程安全
	 */
	iscsi_srand_init(iscsi);
	ret = rand();
	printf("%d\n", ret);
	free(iscsi);
#else
	/* 使用时间作为随机种子 */
	srand(time(NULL));
	ret = rand();
	printf("%d\n", ret);
#endif
	return 0;
}
