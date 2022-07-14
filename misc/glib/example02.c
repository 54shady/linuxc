#include <glib.h>

/*
 * glib bug fix, include the header file manually
 * which shoud be include by glib.h
 */
#include <glib/gprintf.h>

typedef struct SuperStarInfo {
	char *key;
	char *lastname;
	int data;
} SuperStarInfo, *pSuperStarInfo;

/* 通过hash函数算出key对应的hash值 */
static unsigned int wrap_g_str_hash(const void *key)
{
	const SuperStarInfo *ssi = key;
	unsigned int hash = ssi->data * 2;

	/* 用结构体中key变量作为key值和data获得hash值 */
	hash += g_str_hash(ssi->key);

	return hash;
}

static gboolean wrap_g_str_equal(const void *a, const void *b)
{
	const struct SuperStarInfo *s0 = a;
	const struct SuperStarInfo *s1 = b;

	if (s0->data != s1->data)
		return FALSE;

	return !strcmp(s0->key, s1->key);
}

/* gcc  example02.c `pkg-config --cflags --libs glib-2.0` */
int main(int argc, char *argv[])
{
	/* 使用用户自定的hash函数和比较函数 */
	GHashTable* hash = g_hash_table_new(wrap_g_str_hash, wrap_g_str_equal);

	SuperStarInfo ssi0 = {
		.key = "Jordan",
		.lastname = "Michael",
		.data = 23
	};

	SuperStarInfo ssi1 = {
		.key = "Kobe",
		.lastname = "Bryant",
		.data = 24
	};

	SuperStarInfo ssi2 = {
		.key = "Carter",
		.lastname = "Vance",
		.data = 15
	};

	g_hash_table_add(hash, &ssi0);
	g_hash_table_add(hash, &ssi1);
	g_hash_table_add(hash, &ssi2);

	g_printf("There are %d Super Star\n", g_hash_table_size(hash));
	g_printf("%s lastname is %s\n", ssi1.key, ((pSuperStarInfo)g_hash_table_lookup(hash, &ssi1.key))->lastname);

	gboolean found = g_hash_table_remove(hash, &ssi1);
	g_printf("%s was %s found and removed, %d items left\n", ssi1.key, found ? "not" : " ", g_hash_table_size(hash));
	g_hash_table_destroy(hash);

	return 0;
}
