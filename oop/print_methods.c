#include <stdio.h>
#include "print_typedef.h"

static void print_ad(textlist_s *in)
{
	printf("BUY THIS %s !!! Features:\n", in->title);
	for (int i = 0; i < in->len; i++)
		printf(">> %s\n", in->items[i]);
}

static void print_song(textlist_s *in)
{
	printf("%s Lyrics:\n", in->title);
	for (int i = 0; i < in->len; i++)
		printf(">>>> %s\n", in->items[i]);
}

textlist_s save = {
	.title = "God Save the Queen",
	.len = 3,
	.items = (char *[]){"There is no feature", "No future", "No future for me"},
	.print = print_song
};

textlist_s spend = {
	.title = "Never mind the Bollocks LP",
	.items = (char *[]){"By the Sex Pistols", "Anti-consumption thems"},
	.len = 2,
	.print = print_ad
};

#ifndef SKIP_MAIN
int main(int argc, char *argv[])
{
#ifndef MACRO_WRAP
	save.print(&save);
	printf("------------------\n");
	spend.print(&spend);
#else
#define Print(in) in.print(&in)
	Print(save);
	printf(">>>>>>>>>>>>>>>>>>\n");
	Print(spend);
#undef Print
#endif
	return 0;
}
#endif
