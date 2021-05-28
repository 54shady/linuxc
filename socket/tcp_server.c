#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 80
#define SERVER_PORT 8000
#define TRUE 1
#define FALSE 0

void do_something_for_client(char *buf, int bytes)
{
	int i;
	for (i = 0; i < bytes; i++)
	{
#ifdef DEBUG
		printf("%c\t", buf[i]);
#endif
		buf[i] = toupper(buf[i]);
	}
	fflush(stdout);
}

/*
 * Debug this without a single client demo
 *
 * Example1 using socat:
 * socat - TCP4:localhost:8000
 * <Enter test message here>
 * [Get replay from server]
 *
 * Example2 using netcat utility:
 * nc localhost 8000
 * <Enter test message here>
 * [Get replay from server]
 */
int main(int argc, char *argv[])
{
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len;
	int listen_fd, connect_fd;
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	int i, bytes;
	int ret;
	int optval = TRUE;

	/*
	 * AF_INET for IPv4
	 * SOCK_STREAM for TCP
	 * protocol : 0
	 */
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);

	/* set reuse address option */
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		close(listen_fd);
		return -1;
	}

	/* server address for bind */
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);

	/* bind fd and address (fixed address) */
	ret = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (ret < 0)
		perror("Bind error\n");

	/* listen fd */
	listen(listen_fd, 20);

	printf("Accepting connecting...\n");
	while (1)
	{
		client_addr_len = sizeof(client_addr);
		connect_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len);

#ifdef USE_RW
		/* recv client requst */
		bytes = read(connect_fd, buf, MAXLINE);
		printf("recv %d bytes from %s at port %d\n", bytes, inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)), ntohs(client_addr.sin_port));

		/* do something for client */
		do_something_for_client(buf, bytes);

		/* answer client request */
		write(connect_fd, buf, bytes);
#else
		/* recv client requst */
		struct sockaddr_in recv_addr;
		socklen_t addrlen = sizeof(recv_addr);
		bzero(&recv_addr, sizeof(recv_addr));

		bytes = recvfrom(connect_fd, buf, MAXLINE, 0, (struct sockaddr*)&recv_addr, &addrlen);

		/* do something for client */
		do_something_for_client(buf, bytes);

		/* answer client request */
		bytes = sendto(connect_fd, buf, bytes, 0, (struct sockaddr*)&recv_addr, sizeof(recv_addr));
#endif
		close(connect_fd);
	}

	return 0;
}
