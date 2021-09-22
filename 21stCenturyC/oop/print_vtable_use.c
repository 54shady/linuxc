#include <stdio.h>
#define SKIP_MAIN
#include "print_methods.c"
#include "print_vtable.h"

static void song_print_html(textlist_s *in)
{
	printf("<title> %s</title>\n", in->title);
	for (int i = 0; i < in->len; i++)
		printf("%s<br>\n", in->items[i]);
}

/*
 * gcc print_vtable_use.c print_vtable.c `pkg-config --cflags --libs glib-2.0`
 */
int main(int argc, char *argv[])
{
	textlist_print_html(&save);
	printf("-------------\n");

	/*
	 * 虚函数的目的就是子类重写父类的方法
	 * 通过给对象在外部添加一种方法来解决
	 * oop中虚函数的实现
	 */
	print_hash_add(&save, song_print_html);
	textlist_print_html(&save);

	printf("-------------\n");
	print_hash_add(&save, save.print);
	textlist_print_html(&save);

	return 0;
}
