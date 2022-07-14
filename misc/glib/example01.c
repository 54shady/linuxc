#include <glib.h>

/*
 * glib bug fix, include the header file manually
 * which shoud be include by glib.h
 */
#include <glib/gprintf.h>

/* gcc  example01.c `pkg-config --cflags --libs glib-2.0` */
int main(int argc, char *argv[])
{
	/*
	 * g_str_hash: 将字符串转hash值函数(将key算出hash值),可自定义
	 * g_str_equal: 判断两个字符串是否相等,可自定义
	 */
	GHashTable* hash = g_hash_table_new(g_str_hash, g_str_equal);

	char *key1 = "Virginia";
	char *val1 = "Richmond";
	char *key2 = "China";
	char *val2 = "BeiJin";

	g_hash_table_insert(hash, key1, val1);
	g_hash_table_insert(hash, key2, val2);
	g_hash_table_insert(hash, "Ohio", "Columbus");

	g_printf("There are %d keys in the hash\n", g_hash_table_size(hash));
	g_printf("The capital of %s is %s\n", key2, (char *)g_hash_table_lookup(hash, key2));
	gboolean found = g_hash_table_remove(hash, key1);
	g_printf("The value '%s' was %s found and removed, %d items left\n", key1, found ? "not" : " ", g_hash_table_size(hash));
	g_hash_table_destroy(hash);

	return 0;
}
