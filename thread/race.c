#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
	char name[20];
	int sleep_time;
	int start;
	int end;
}RacerInfo;

void *start_routine(void *p)
{
	int i;
	RacerInfo *ri = (RacerInfo *)p;

	for (i = ri->start; i <= ri->end; i++)
	{
		sleep(ri->sleep_time);
		printf("%s, %lx, run %d\n", ri->name, pthread_self(), i);
	}

	//return (void *)0;
	pthread_exit((void *)0);
}

int main(int argc, char *argv[])
{
	int err;
	pthread_t rabbit, turtle;

	RacerInfo racer_rabbit = {
		"Rabbit",
		1,
		0,
		10
	};

	RacerInfo racer_turtle = {
		"Turtul",
		2,
		5,
		15
	};

	/* create two threads */
	if ((err = pthread_create(&rabbit, NULL, start_routine, (void *)&racer_rabbit)) != 0)
	{
		perror("Pthread create error\n");
		exit(1);
	}
	if ((err = pthread_create(&turtle, NULL, start_routine, (void *)&racer_turtle)) != 0)
	{
		perror("Pthread create error\n");
		exit(1);
	}

	/* wait for threads exit */
	pthread_join(rabbit, NULL);
	pthread_join(turtle, NULL);

	printf("Racing end\n");

	return 0;
}
