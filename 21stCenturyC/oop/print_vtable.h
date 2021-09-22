#include <glib.h>
#include "print_typedef.h"

extern GHashTable *print_vtable;

typedef void (*print_fn_type)(textlist_s *);

void check_print_fn(print_fn_type pf);

#define print_hash_add(object, print_fn) { \
	check_print_fn(print_fn); \
	g_hash_table_insert(print_vtable, (object)->print, print_fn); \
}

void textlist_print_html(textlist_s *in);
