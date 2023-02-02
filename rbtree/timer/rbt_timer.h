#ifndef __RBT_TIMER__
#define __RBT_TIMER__

#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "ngx_rbtree.h"

typedef struct timer_node_s timer_node_t;
typedef void (*timer_handler_pt)(timer_node_t *);

/*
 * 自定义的定时器数据结构
 * @node: 其中第一个成员为红黑树根节点,以便进行转换
 * @handler: 定时器超时处理函数
 *
 * _s后缀表示结构体变量
 * _t后缀表示typedef定义的变量
 */
struct timer_node_s {
	ngx_rbtree_node_t node;
	timer_handler_pt handler;
};

/* 全局的定时器:红黑树的根节点 */
ngx_rbtree_t g_timer;

/* 全局的哨兵, 所有的叶子节点都指向哨兵 */
static ngx_rbtree_node_t g_sentinel;

/* 哨兵节点和根节点相等则表示树是空的:即没有定时器 */
int is_rbt_empty()
{
	return g_timer.root == g_timer.sentinel;
}

/* 初始化红黑树 */
void init_timer()
{
	/*
	 * 选择ngx_rbtree_insert_timer_value的原因
	 * 是在红黑数中可能插入相同key值的定时器
	 * 即添加了相同超时时间的定时器
	 * 此时插入方法是将相同key值的节点插入到右子树
	 * 这样能实现在有相同超时时间的定时器时,执行的顺序是按照其插入的顺序
	 * 比如同时插入三个超时时间为1秒中的定时器a,b,c
	 * 则执行定时器的顺序仍然为a,b,c
	 * 如果定时器底层数据结构改用hlist(哈希表)的化,这里需要用链式来处理
	 */
	ngx_rbtree_init(&g_timer, &g_sentinel, ngx_rbtree_insert_timer_value);
}

unsigned int current_time()
{
	struct timespec tp;

	clock_gettime(CLOCK_MONOTONIC, &tp);

	return tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
}

void del_timer(timer_node_t *tn)
{
	ngx_rbtree_delete(&g_timer, &tn->node);
	free(tn);
}

void handle_timer()
{
	ngx_rbtree_node_t *node;
	timer_node_t *tn;

	for (;;)
	{
		/* 空树: 此时没有定时器 */
		if (is_rbt_empty())
			break;

		/* 找到最小的,即最近一个要超时的定时器 */
		node = ngx_rbtree_min(g_timer.root, g_timer.sentinel);

		/*
		 * 当前时间比树中最小的定时器时间还小
		 * 说明还未超时
		 */
		if (current_time() < node->key)
			break;

		/*
		 * 找到了需要处理的定时器
		 * 在timer_node_t的数据结构设计时将红黑数节点放在了结构体第一个成员位置
		 * 所以可以利用其和结构体首地址相同的原理来强行转换为timer_node_t的指针
		 */
		tn = (timer_node_t *)node;
		tn->handler(tn);

		/* 将已经处理的定时器从红黑树中删除 */
		del_timer(tn);
	}
}

timer_node_t *add_timer(int expire, timer_handler_pt func)
{
	timer_node_t *tn = malloc(sizeof(timer_node_t));
	assert(tn != NULL);

	/* 以定时器的超时时间作为节点的key值插入到红黑树 */
	tn->node.key = expire + current_time();
	tn->handler = func;
	ngx_rbtree_insert(&g_timer, &tn->node);

	return tn;
}

/*
 * 找到最新需要处理的定时器
 * 因为定时器是按超时时间作为key值插入到红黑树
 * 所以更加红黑树特性可知,最近将要超时的定时器必定是最左边的子节点
 */
int find_nearest_timer()
{
	int diff;
	ngx_rbtree_node_t *node;

	/*
	 * 空树则返回-1让epoll一直阻塞
	 * 这里需要返回-1,给epoll_wait,让其处理网咯事件
	 */
	if (is_rbt_empty())
		return -1;

	node = ngx_rbtree_min(g_timer.root, g_timer.sentinel);
	diff = node->key - current_time();

	return diff > 0 ? diff : 0;
}

#endif
