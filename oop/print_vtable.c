#include <stdio.h>
#include "print_vtable.h"

GHashTable *print_vtable;

/*
 * 用于检测用户设置的函数指针类型是否正确
 * 如果和定义的不吻合(print_fn_type)则在
 * 编译的时候就会报错
 * 防止用户设置错误的函数
 */
void check_print_fn(print_fn_type pf) {}

void textlist_print_html(textlist_s *in)
{
	if (!print_vtable)
		print_vtable = g_hash_table_new(g_direct_hash, g_direct_equal);

	print_fn_type pft = g_hash_table_lookup(print_vtable, in->print);
	if (pft)
	{
		pft(in);
		return;
	}

	printf("Default method:\n");
	printf("<title>%s</title>\n", in->title);
	for (int i = 0; i < in->len; i++)
		printf("<li>%s</li>\n", in->items[i]);
	printf("</ul>\n");
}
