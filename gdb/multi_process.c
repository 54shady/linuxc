#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>

#define PARENT_PROCESS_TAG "parent_p: "
#define CHILD_PROCESS_TAG "child_p: "
#define PARENT_THREAD_TAG "parent_t: "
#define CHILD_THREAD_TAG "child_t: "

#define __DEBUG__
#ifdef __DEBUG__
#define DBG_PRINT(fmt, args...)\
        printf("DEBUG: [%s]: "fmt, __func__, ##args)
#else
#define DBG_PRINT(fmt, args...) do {} while (0)
#endif

/* global child thread id */
pthread_t g_tid;

/*
 * checkout the thread id
 * ps -eLf | grep goutc
 */

pid_t my_gettid()
{
	/*
	 * don't use the raw syscall, use libc wraper instead
	 * gcc -D_GNU_SOURCE ...
	 */
	return gettid(); /* syscall(SYS_gettid); */
}

void loop_work(char *msg)
{
	while (1)
	{
		DBG_PRINT("%s\n", msg);
		sleep(1);
	}
}

void *start_routine(void *arg)
{
	/* process prefix */
	char pprefix[] = PARENT_PROCESS_TAG;

	/* thread prefix */
	char *tprefix = (char *)arg;

	pid_t pid = getpid();
	pid_t tid = my_gettid();
	pthread_t ptid = pthread_self();

	/* child thread ?*/
	if (!strcmp(tprefix, CHILD_THREAD_TAG))
		printf("child thread tid = %lu\n", g_tid);

	printf("%s pid[%d] %s tid[%d] ptid[%lu] created\n", pprefix, pid, tprefix, tid, ptid);

	/* do the works */
	loop_work(tprefix);

	return NULL;
}

void parent_process()
{
	/* process prefix */
	char pprefix[] = PARENT_PROCESS_TAG;
	int tstatus;
	pid_t pid = getpid();

	printf("%s pid[%d] created\n", pprefix, pid);
	printf("Start to create two threads\n");

	tstatus = pthread_create(&g_tid, NULL, start_routine, CHILD_THREAD_TAG);
	if (tstatus != 0)
		printf("%s: Can not create new thread.", PARENT_PROCESS_TAG);

	/* parent thread do */
	start_routine(PARENT_THREAD_TAG);
}

void child_process()
{
	/* process prefix */
	char pprefix[] = CHILD_PROCESS_TAG;

	pid_t pid = getpid();
	printf("%s pid[%d] created\n", pprefix, pid);

	/* do the works */
	loop_work(pprefix);
}

int main(int argc, char *argv[])
{
	int pid;

	printf("Start to create two processes\n");
	pid = fork();

	if (pid != 0)
		parent_process();
	else
		child_process();

	return 0;
}
