#ifndef __FOO_H__
#define __FOO_H__

struct person {
	int age;
	char name[20];
};

#ifdef INCLUDE_CFILE_DIRECT
#else
void init_person(void);
struct person *get_person(void);
void show_age(void);
void free_person(void);
#endif

#endif
