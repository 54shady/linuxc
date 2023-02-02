#include <stdio.h>
#include <sys/epoll.h>

#include "rbt_timer.h"

#define MAX_EVENTS_NR 512

void print_time(timer_node_t *tn)
{
	printf("handle timer = %u\n", tn->node.key);
}

int main(int argc, char *argv[])
{
	int epFd, n, i, nearest;
	struct epoll_event evs[MAX_EVENTS_NR] = {0};

	init_timer();
	add_timer(1000, print_time); /* 1秒后超时 */
	add_timer(3000, print_time); /* 3秒后超时 */
	timer_node_t *tn = add_timer(3000, print_time);
	add_timer(5000, print_time); /* 5秒后超时 */
	del_timer(tn);

	/* 传入的参数必须大于0,用于兼容以前的接口 */
	epFd = epoll_create(1);

	for (;;)
	{
		nearest = find_nearest_timer();

		/*
		 * 第四个参数:
		 * 0: 不阻塞,查询完就继续
		 * -1: 一直阻塞
		 * >0: 执行等待相应的时间
		 */
		n = epoll_wait(epFd, evs, MAX_EVENTS_NR, nearest);

		/*
		 * 这里一般用于处理网咯事件
		 * epoll一般将网咯事件处理和定时任务相结合,
		 * 所以当没有网咯事件到来的时候则执行定时器的超时任务
		 * 所以在第四个参数中,将最近的定时器超时时间传递给epoll_wait
		 */
		for (i = 0; i < n; i++)
		{
			/* 这里假设没有需要处理的网咯事件 */
		}

		handle_timer();
	}

	return 0;
}
