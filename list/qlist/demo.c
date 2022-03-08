#include <stdio.h>
#include <stdlib.h>

/* copy from qemu source code base */
#include "queue.h"

struct PersonInfo
{
    int age;
    void *opaque;
    QLIST_ENTRY(PersonInfo) entry;
};
typedef struct PersonInfo PersonInfo;

QLIST_HEAD(, PersonInfo) person_info_list =
    QLIST_HEAD_INITIALIZER(person_info_list);

struct PersonInfo pis[] = {
	{
		.age = 23,
		.opaque = "Jordan"
	},
	{
		.age = 24,
		.opaque = "Kobe"
	},
	{
		.age = 15,
		.opaque = "Carter"
	},
	{
		.age = 3,
		.opaque = "Iverson"
	},
};

int main(int argc, char *argv[])
{
	PersonInfo *pi;
	PersonInfo *npi;
	int i;

	for (i = 0; i < sizeof(pis) / sizeof(PersonInfo); i++)
		QLIST_INSERT_HEAD(&person_info_list, &pis[i], entry);

	if (QLIST_EMPTY(&person_info_list))
		printf("list empty\n");

    QLIST_FOREACH(pi, &person_info_list, entry) {
		printf("Name: %s, Number: %d\n", (char *)pi->opaque, pi->age);
    }

	QLIST_REMOVE(&pis[1], entry);
	printf("Kobe is gone, sad :-(\n");

    QLIST_FOREACH_SAFE(pi, &person_info_list, entry, npi) {
		printf("Name: %s, Number: %d\n", (char *)pi->opaque, pi->age);
	}

	return 0;
}
