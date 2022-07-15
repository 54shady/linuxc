#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBERS_OF_PEOPLE 10
#define NAME_LEN 30

struct person {
	int age;
	char name[NAME_LEN];
};

struct human {
	int sexaul;

	struct person **p;
};

int main(int argc, char *argv[])
{
	int i;

	struct human *h;

	h = malloc(sizeof(struct human));
	if (h == NULL)
	{
		printf("no memory\n");
		return -1;
	}

#ifdef SHOW_WRONG_USAGE
	/* just alloc a point */
	h->p = malloc(sizeof(struct person *));
#else
	/* alloc array size of point */
	h->p = malloc(sizeof(struct person *) * NUMBERS_OF_PEOPLE);
#endif
	if (h->p == NULL)
	{
		printf("no memory\n");
		return -1;
	}

	for (i = 0; i < NUMBERS_OF_PEOPLE; i++)
	{
		h->p[i] = malloc(sizeof(struct person));
		if (h->p[i] == NULL)
		{
			printf("no memory\n");
			return -1;
		}

	}

	for (i = 0; i < NUMBERS_OF_PEOPLE; i++)
		snprintf(h->p[i]->name, NAME_LEN, "%s_%d", "annomous", i);

	for (i = 0; i < NUMBERS_OF_PEOPLE; i++)
		printf("name = %s\n", h->p[i]->name);

	/* free memory */
	for (i = 0; i < NUMBERS_OF_PEOPLE; i++)
	{
		free(h->p[i]);
		h->p[i] = NULL;
	}

	free(h->p);
	free(h);

	return 0;
}
