#include <stdio.h>

#define SKIP_MAIN
/* no need to compile and link the c file below, while the file is included */
#include "print_methods.c" /* include c file */

textlist_s recipe = {
	.title = "1 egg for baking",
	.len = 2,
	.items = (char *[]){"1 Tbsp ground flax seeds", "3 Tbsp water"}
};

/* dispatch functon */
void textlist_print(textlist_s *in)
{
	if (in->print)
	{
		in->print(in);
		return;
	}

	/* Default method to fall back on */
	printf("Tittle: %s\n\nItems:\n", in->title);
	for (int i = 0; i < in->len; i++)
		printf("==> %s\n", in->items[i]);
}

int main(int argc, char *argv[])
{
	textlist_print(&save);
	printf("--------------\n");
	textlist_print(&recipe);

	return 0;
}
