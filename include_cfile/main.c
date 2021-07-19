#include <stdio.h>
#include <string.h>

#ifdef INCLUDE_CFILE_DIRECT
#include "foo.c"
#else
#include "foo.h"
#endif

int main(int argc, char *argv[])
{
/*
 * we can call the module local funciton in main module
 * which means we make the module static function locally
 */
#ifdef INCLUDE_CFILE_DIRECT
	make_one_person_st();

	p->age  = 1111;
	strcpy(p->name, "test");
	show_age_st(p);
	free_person_st(p);
#else
	struct person *p;
	init_person();
	p = get_person();

	p->age = 999;
	strcpy(p->name, "test");
	show_age();
	free_person();
#endif
	return 0;
}
