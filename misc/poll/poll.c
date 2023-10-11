#define _XOPEN_SOURCE 700
#include <fcntl.h> /* creat, O_CREAT */
#include <poll.h> /* poll */
#include <stdio.h> /* printf, puts, snprintf */
#include <stdlib.h> /* EXIT_FAILURE, EXIT_SUCCESS */
#include <unistd.h> /* read */

/*
 * https://stackoverflow.com/questions/24791625/how-to-handle-the-linux-socket-revents-pollerr-pollhup-and-pollnval
 *
 * gcc -o poll.out -std=c99 poll.c
 * sudo mknod -m 666 poll0.tmp p
 *
 * 执行./poll.out后
 * 在另外一个teminal窗口中执行printf a > poll0.tmp
 *
 * 输出如下
 * loop
 * POLLIN n=1 buf=a
 * POLLHUP
 * loop
 *
 * 或输出如下
 * loop
 * POLLIN n=1 buf=a
 * loop
 * POLLHUP
 * loop
 *
 * 其中
 * - POLLIN happens when input becomes available
 * - POLLHUP happens when the file is closed by the printf
 * - close(pfd.fd); and pfd.fd *= -1; clean things up, and we stop receiving POLLHUP
 * - poll hangs forever
 *
 * 注释掉pfd.fd *= -1后测试
 * gcc -o poll.out -std=c99 poll.c -DTEST_POLLNVAL
 *
 * 发现输出如下
 * loop
 * POLLIN n=1 buf=a
 * loop
 * POLLHUP
 * loop
 * POLLNVAL
 * loop
 * POLLNVAL
 * loop
 * ...无限循环
 *
 *
 * 总结:
 * - A POLLHUP means the socket is no longer connected.
 *	 In TCP, this means FIN has been received and sent.
 *
 * - A POLLERR means the socket got an asynchronous error. In TCP, this
 *   typically means a RST has been received or sent. If the file descriptor is
 *   not a socket, POLLERR might mean the device does not support polling.
 *
 * For both of the conditions above, the socket file descriptor is still open,
 * and has not yet been closed (but shutdown() may have already been called). A
 * close() on the file descriptor will release resources that are still being
 * reserved on behalf of the socket. In theory, it should be possible to reuse
 * the socket immediately (e.g., with another connect() call).
 *
 * - A POLLNVAL means the socket file descriptor is not open.
 *   It would be an error to close() it.
 * */
int main(void) {
    char buf[1024];
    int fd, n;
    short revents;
    struct pollfd pfd;

    fd = open("poll0.tmp", O_RDONLY | O_NONBLOCK);
    pfd.fd = fd;
    pfd.events = POLLIN;
    while (1) {
        puts("loop");
        poll(&pfd, 1, -1);
        revents = pfd.revents;
        if (revents & POLLIN) {
            n = read(pfd.fd, buf, sizeof(buf));
            printf("POLLIN n=%d buf=%.*s\n", n, n, buf);
        }
        if (revents & POLLHUP) {
            printf("POLLHUP\n");
            close(pfd.fd);
#ifdef TEST_POLLNVAL
#else
			/* 如果没有设置成-1会一直poll fd, 导致无限POLLNVAL */
            pfd.fd *= -1;
#endif
        }
        if (revents & POLLNVAL) {
            printf("POLLNVAL\n");
        }
        if (revents & POLLERR) {
            printf("POLLERR\n");
        }
    }
}
