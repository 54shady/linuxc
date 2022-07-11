/*
 * This is a datagram socket server sample program for UNIX
 * domain sockets. This program creates a socket and
 * receives data from a client.
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "common.h"

int main(int argc, char *argv[])
{
    int server_sock, len, rc;
    int bytes_rec = 0;
    struct sockaddr_un server_sockaddr, peer_sock;
    char buf[256];

    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(buf, 0, 256);

    /* Create a UNIX domain datagram socket */
    server_sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (server_sock == -1) {
        printf("SOCKET ERROR = %d", sock_errno());
        exit(1);
    }

    /*
	 * Set up the UNIX sockaddr structure
     * by using AF_UNIX for the family and
     * giving it a filepath to bind to.
     */
    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SERVER_PATH);
    len = sizeof(server_sockaddr);
	/*
     * Unlink the file so the bind will
     * succeed, then bind to that file.
	 */
    unlink(SERVER_PATH);
    rc = bind(server_sock, (struct sockaddr *) &server_sockaddr, len);
    if (rc == -1) {
        printf("BIND ERROR = %d", sock_errno());
        close(server_sock);
        exit(1);
    }

    /*
	 * Read data on the server from clients
	 * and print the data that was read.
	 */
    printf("waiting to recvfrom...\n");
    bytes_rec = recvfrom(server_sock, buf, 256, 0, (struct sockaddr *)&peer_sock,
			(socklen_t *restrict)&len);
    if (bytes_rec == -1) {
        printf("RECVFROM ERROR = %d", sock_errno());
        close(server_sock);
        exit(1);
    } else {
       printf("DATA RECEIVED = %s\n", buf);
    }

    /* Close the socket and exit */
    close(server_sock);

    return 0;
}
