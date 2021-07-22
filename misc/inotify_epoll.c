#include <unistd.h>
#include <stdio.h>
#include <sys/inotify.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define DATA_MAX_LEN 500
#define MAX_FILES 1000

/* g_full_name 文件全路径名 */
static char *g_full_name;

/* 文件路径名 */
static const char *g_dir_name;

static char *epoll_files[MAX_FILES];

void rm_from_epoll(int fd, int epollFd)
{
	epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL);
}

int add_to_epoll(int fd, int epollFd)
{
	int result;
    struct epoll_event eventItem;
    memset(&eventItem, 0, sizeof(eventItem));
    eventItem.events = EPOLLIN;
    eventItem.data.fd = fd;
    result = epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &eventItem);
	return result;
}

int get_epoll_fd_by_name(char *name)
{
	int i;
	for (i = 0; i < MAX_FILES; i ++)
	{
		if (!epoll_files[i])
			continue;

		if (!strcmp(epoll_files[i], name))
			return i;
	}

	return -1;
}

void print_usage(const char *name)
{
	printf("Usage:\n");
	printf("%s <watch_path>\n", name);
}

int read_process_inotify_fd(int iNotifyFd, int iEpollFd)
{
	int tmpFd;
	int res;
    char event_buf[512];
    int event_size;
    int event_pos = 0;
    struct inotify_event *event;

	/*
	 * 从文件句柄中读出事件
	 * 如果有事件的话最少应该有一个事件
	 * 所以res >= sizeof(struct inotify_event) 表示有事件发生
	 * 否则程序会休眠在这里
	 */
	res = read(iNotifyFd, event_buf, sizeof(event_buf));
	if (res < (int)sizeof(struct inotify_event))
	{
		if (errno == EINTR)
			return 0;
        printf("could not get event, %s\n", strerror(errno));
        return -1;
	}

	/* event_buf里可能会有多个事件,所以需要挨个处理 */
	while (res >= sizeof(struct inotify_event))
	{
		event = (struct inotify_event *)(event_buf + event_pos);
		if (event->len)
		{
			if(event->mask & IN_CREATE)
			{
				printf("create file: %s\n", event->name);
				g_full_name = malloc(512);
				sprintf(g_full_name, "%s/%s", g_dir_name, event->name);
				tmpFd = open(g_full_name, O_RDWR);
				printf("Add to epoll: %s\n", g_full_name);
				add_to_epoll(tmpFd, iEpollFd);
				epoll_files[tmpFd] = g_full_name;
			}
			else
			{
				printf("delete file: %s\n", event->name);
				sprintf(g_full_name, "%s/%s", g_dir_name, event->name);
				tmpFd = get_epoll_fd_by_name(g_full_name);
				if (tmpFd >= 0)
				{
					printf("remove from epoll: %s/%s\n", g_dir_name, event->name);
					rm_from_epoll(tmpFd, iEpollFd);
					free(epoll_files[tmpFd]);
				}
			}
		}
        event_size = sizeof(struct inotify_event) + event->len;
        res -= event_size;
        event_pos += event_size;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int i;
	int pollResult;
	int len;
	char buf[DATA_MAX_LEN];
	int iNotifyFd;
	int iEpollFd;
    /* Maximum number of signalled FDs to handle at a time */
    static const int EPOLL_MAX_EVENTS = 16;

    /* The array of pending epoll events and the index of the next event to be handled */
    struct epoll_event mPendingEventItems[EPOLL_MAX_EVENTS];

	if (argc < 2)
	{
		print_usage(argv[0]);
		return -1;
	}

	g_dir_name = argv[1];

	/* 创建一个用于notify的句柄 */
	iNotifyFd = inotify_init();

	/* 打开一个epoll的句柄 */
    iEpollFd = epoll_create(8);

	/* 向这个notify添加需要监控的文件 */
	inotify_add_watch(iNotifyFd, g_dir_name, IN_DELETE | IN_CREATE);

	/* 把notify fd 添加到epoll fd里 */
	add_to_epoll(iNotifyFd, iEpollFd);

	/* 等待文件有数据读或写 */
	while (1)
	{
		pollResult = epoll_wait(iEpollFd, mPendingEventItems, EPOLL_MAX_EVENTS, -1);
		for (i = 0; i < pollResult; i++)
		{
			/*
			 * 如果是nofity fd
			 * 表明是创建或是删除了文件
			 * 需要将变动的文件同步到epoll
			 */
			if (mPendingEventItems[i].data.fd == iNotifyFd)
			{
				read_process_inotify_fd(iNotifyFd, iEpollFd);
			}
			else
			{
				printf("Reason: 0x%x\n", mPendingEventItems[i].events);
				len = read(mPendingEventItems[i].data.fd, buf, DATA_MAX_LEN);
				buf[len] = '\0';
				printf("get data: %s\n", buf);
			}
		}
	}
}
