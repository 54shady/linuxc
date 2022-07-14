#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

typedef struct HMPCommand {
	const char *name;
} HMPCommand;

/* sort accroding the name */
static int compare_mon_cmd(const void *a, const void *b)
{
	return strcmp(((const HMPCommand *)a)->name,
			((const HMPCommand *)b)->name);
}

int main(int argc, char *argv[])
{
	int i;

	HMPCommand hmp_cmds[] = {
#include "sort-list.h"
		{ .name = "cool" },
		{ .name = "acho" },
		{ .name = "zoom" },
		{ .name = "allo" },

		{ NULL, }, /* need null at the end of comparation */

	};

	for (i = 0; i < ARRAY_SIZE(hmp_cmds) && hmp_cmds[i].name != NULL; i++)
		printf("%s\n", hmp_cmds[i].name);

	printf("\nSort...New order:\n\n");
	qsort(hmp_cmds, ARRAY_SIZE(hmp_cmds) - 1, sizeof(*hmp_cmds), compare_mon_cmd);

	for (i = 0; i < ARRAY_SIZE(hmp_cmds) && hmp_cmds[i].name != NULL; i++)
		printf("%s\n", hmp_cmds[i].name);

	return 0;
}
