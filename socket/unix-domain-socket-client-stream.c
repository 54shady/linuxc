/*
 * This is a stream socket client sample program for UNIX domain sockets.
 * This program creates a socket, connects
 * to a server, sends data, then receives and prints a
 * message from the server.
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
    int client_sock, rc, len;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    char buf[256];

    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));

    /* Create a UNIX domain stream socket */
    client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_sock == -1) {
        printf("SOCKET ERROR = %d\n", sock_errno());
        exit(1);
    }

    /*
	 * Set up the UNIX sockaddr structure
     * by using AF_UNIX for the family and
     * giving it a filepath to bind to.
	 */
    client_sockaddr.sun_family = AF_UNIX;
    strcpy(client_sockaddr.sun_path, CLIENT_PATH);
    len = sizeof(client_sockaddr);
	/*
     * Unlink the file so the bind will
     * succeed, then bind to that file.
	 */
    unlink(CLIENT_PATH);
    rc = bind(client_sock, (struct sockaddr *) &client_sockaddr, len);
    if (rc == -1) {
        printf("BIND ERROR: %d\n", sock_errno());
        close(client_sock);
        exit(1);
    }

    /*
	 * Set up the UNIX sockaddr structure
     * for the server socket and connect to it.
	 */
    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SERVER_PATH);
    rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
    if(rc == -1) {
        printf("CONNECT ERROR = %d\n", sock_errno());
        close(client_sock);
        exit(1);
    }

    /*
	 * Copy the data to the buffer and
     * send it to the server socket.
	 */
    strcpy(buf, DATA1);
    printf("Sending data...\n");
    rc = send(client_sock, buf, strlen(buf), 0);
    if (rc == -1) {
        printf("SEND ERROR = %d\n", sock_errno());
        close(client_sock);
        exit(1);
    } else {
        printf("Data sent!\n");
    }

    /* Read the data sent from the server and print it. */
    printf("Waiting to recieve data...\n");
    memset(buf, 0, sizeof(buf));
    rc = recv(client_sock, buf, sizeof(buf), 0);
    if (rc == -1) {
        printf("RECV ERROR = %d\n", sock_errno());
        close(client_sock);
        exit(1);
    } else {
        printf("RECEIVED = %s\n", buf);
    }

    /* Close the socket and exit. */
    close(client_sock);

    return 0;
}
