#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "wrap.h"

#define MAXLINE 80

int main(int argc, char *argv[])
{
	struct sockaddr_in dest_addr;
	char buf[MAXLINE];
	int sock_fd;
	int bytes;
	char *target_ip;

	if (argc != 3)
	{
		printf("Usage : %s <target ip> <port>\n", argv[0]);
		exit(1);
	}

	target_ip = argv[1];

	sock_fd = Socket(AF_INET, SOCK_DGRAM, 0);

	/* destination address */
	bzero(&dest_addr, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, target_ip, &dest_addr.sin_addr);

	while (fgets(buf, MAXLINE, stdin) != NULL)
	{
		bytes = sendto(sock_fd, buf, strlen(buf), 0 , (struct sockaddr *)&dest_addr, sizeof(dest_addr));
		if (bytes == -1)
			perror("Send to error");

		bytes = recvfrom(sock_fd, buf, MAXLINE, 0, NULL, 0);
		if (bytes == -1)
			perror("Recv error");

		buf[bytes] = '\0';
		printf("Response from serve %s\n", buf);
	}

	Close(sock_fd);

	return 0;
}
