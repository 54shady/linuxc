#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <stdarg.h>

#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)

void die_check_func(char *f, int line, int cond, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	if(unlikely(cond)) {
		printf("DIE at %s:%i: ", f, line);
		vprintf(fmt, args);
		printf("\n");
		abort();
	}
	va_end(args);
}

#define DIE_IF(cond, fmt, ...) \
	die_check_func(__FILE__, __LINE__, !!(cond), fmt, ##__VA_ARGS__)

struct task {
	pthread_t thread;
	pthread_attr_t attr;
	void *arg;
};

/* get current system cpu numbers */
int get_ncpu(void)
{
	return sysconf(_SC_NPROCESSORS_ONLN);
}

/* new thread will run on oncpu */
struct task *create_task_oncpu(void *(*start_routine) (void *), void *arg,
		int oncpu, int cpu_nr)
{
	struct task *t;
	cpu_set_t *cpuset;
	int cpuset_sz;
	int ret;

	t = (struct task *)malloc(sizeof(struct task));
	DIE_IF(!t, "malloc");

	t->arg = arg;

	ret = pthread_attr_init(&t->attr);
	if (oncpu >= 0)
	{
		cpuset = CPU_ALLOC(cpu_nr);
		cpuset_sz = CPU_ALLOC_SIZE(cpu_nr);
		DIE_IF(!cpuset, "cpuset alloc");
		CPU_ZERO_S(cpuset_sz, cpuset);
		CPU_SET_S(oncpu, cpuset_sz, cpuset);
		pthread_attr_setaffinity_np(&t->attr, cpuset_sz, cpuset);
		CPU_FREE(cpuset);
	}
	ret = pthread_create(&t->thread, &t->attr, start_routine, (void *)t);
	DIE_IF(ret, "pthread_create");

	return t;
}

void join_task(struct task *t)
{
	pthread_join(t->thread, NULL);
	pthread_attr_destroy(&t->attr);
	free(t);
}

void *thread_routin(void *arg)
{
	while (1)
		sleep(1);
}

/*
 * Usage:
 * gcc -lpthread thread_oncpu.c -D_GNU_SOURCE
 *
 * run the program on specifiy cpu
 */
int main(int argc, char *argv[])
{
	struct task *t;
	int cpu;
	int cpu_nr = 0;

	cpu_nr = get_ncpu();
	DIE_IF(!cpu_nr, "get cpu numbers error");
	printf("cpu numbers:%d\n", cpu_nr);

	printf("Run on which cpu?: ");
	scanf("%d", &cpu);

	t = create_task_oncpu(thread_routin, NULL, cpu, cpu_nr);
	join_task(t);

	return 0;
}
