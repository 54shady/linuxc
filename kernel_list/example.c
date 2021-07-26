#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "example.h"

int main(int argc, char *argv[])
{
	int i;
	struct people *p;
	struct people *pos, *tmp;
	char name[10];

	for (i = 0; i < 6; i++)
	{
		sprintf(name, "name_%d", i);
		p = malloc(sizeof(struct people));
		strcpy(p->name, name);
		list_add(&p->list1, &programer);
	}

	for (i = 6; i < 11; i++)
	{
		sprintf(name, "name_%d", i);
		p = malloc(sizeof(struct people));
		strcpy(p->name, name);
		list_add(&p->list2, &good_man);
	}

	printf("Programer: ");
	list_for_each_entry(pos, &programer, list1)
		printf("%s ", pos->name);
	printf("\n");

	printf("GoodMan: ");
	list_for_each_entry(pos, &good_man, list2)
		printf("%s ", pos->name);
	printf("\n");

	/* delete node from list, solution 1 */
	list_for_each_entry(pos, &good_man, list2)
	{
		if (!strcmp(pos->name, "name_7"))
		{
			printf("Delete name_7 from list2\n");
			list_del(&pos->list2);
			free(pos);
			pos = NULL;
			break; /* need break here */
		}
	}
	printf("GoodMan: ");
	list_for_each_entry(pos, &good_man, list2)
		printf("%s ", pos->name);
	printf("\n");

	/* delete node from list, solution 2 */
	list_for_each_entry_safe(pos, tmp, &good_man, list2)
	{
		if (!strcmp(pos->name, "name_8"))
		{
			printf("Delete name_8 from list2\n");
			list_del(&pos->list2);
			free(pos);
			pos = NULL;
		}
	}
	printf("GoodMan: ");
	list_for_each_entry(pos, &good_man, list2)
		printf("%s ", pos->name);
	printf("\n");
}
