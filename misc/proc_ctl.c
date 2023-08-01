#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/types.h>

/*
 * name a thread(set a name for thread)
 * Usage
 * cat /proc/`pidof a.out`/comm
 */
int main(int argc, char *argv[])
{
	pid_t pid;
	char tmp_name[20];
	const char *default_name = "defname";

	pid = getpid();
	prctl(PR_GET_NAME, tmp_name);
	printf("pid : %d-%s\n", pid, tmp_name);

	/* change the process name */
	if (argc < 2)
		prctl(PR_SET_NAME, default_name);
	else
		prctl(PR_SET_NAME, argv[1]);

	prctl(PR_GET_NAME, tmp_name);
	printf("pid : %d-%s\n", pid, tmp_name);

	while (1)
		sleep(1);

	return 0;
}
