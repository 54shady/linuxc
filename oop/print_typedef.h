#ifndef __PRINT_TYPEDEF_H__
#define __PRINT_TYPEDEF_H__

/* a rather generic struct, with a built-in print method */
typedef struct textlist_s {
	char *title;
	char **items;
	int len;
	void (*print)(struct textlist_s *);
} textlist_s;
#endif
