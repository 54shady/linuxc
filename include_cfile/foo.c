#include <stdio.h>
#include <stdlib.h>

#include "foo.h"

static struct person *p;

static void make_one_person_st(void)
{
	p = malloc(sizeof(struct person));
}

#ifdef INCLUDE_CFILE_DIRECT
static void show_age_st(struct person *p)
{
	printf("%s, %s:%d\n", __FUNCTION__, p->name, p->age);
}

static void free_person_st(struct person *p)
{
	free(p);
}
#else
void init_person(void)
{
	make_one_person_st();
}

struct person *get_person(void)
{
	return p;
}

void show_age(void)
{
	printf("%s %s:%d\n", __FUNCTION__, p->name, p->age);
}

void free_person(void)
{
	free(p);
}
#endif
