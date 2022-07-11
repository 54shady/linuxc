/*
 * This is a stream socket server sample program for UNIX
 * domain sockets.
 * This program listens for a connection from a client program,
 * accepts it, reads data from the client,
 * then sends data back to connected UNIX socket.
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
    int server_sock, client_sock, len, rc;
    int bytes_rec = 0;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    char buf[256];
    int backlog = 10;

    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(buf, 0, 256);

    /* Create a UNIX domain stream socket */
    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock == -1) {
        printf("SOCKET ERROR: %d\n", sock_errno());
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
        printf("BIND ERROR: %d\n", sock_errno());
        close(server_sock);
        exit(1);
    }

    /* Listen for any client sockets */
    rc = listen(server_sock, backlog);
    if (rc == -1) {
        printf("LISTEN ERROR: %d\n", sock_errno());
        close(server_sock);
        exit(1);
    }
    printf("socket listening...\n");

    /* Accept an incoming connection */
    client_sock = accept(server_sock, (struct sockaddr *) &client_sockaddr,
			(socklen_t *restrict)&len);
    if (client_sock == -1) {
        printf("ACCEPT ERROR: %d\n", sock_errno());
        close(server_sock);
        close(client_sock);
        exit(1);
    }

    /* Get the name of the connected socket */
    len = sizeof(client_sockaddr);
    rc = getpeername(client_sock, (struct sockaddr *) &client_sockaddr,
			(socklen_t *restrict)&len);
    if (rc == -1) {
        printf("GETPEERNAME ERROR: %d\n", sock_errno());
        close(server_sock);
        close(client_sock);
        exit(1);
    } else {
        printf("Client socket filepath: %s\n", client_sockaddr.sun_path);
    }

    /* Read and print the data incoming on the connected socket */
    printf("waiting to read...\n");
    bytes_rec = recv(client_sock, buf, sizeof(buf), 0);
    if (bytes_rec == -1) {
        printf("RECV ERROR: %d\n", sock_errno());
        close(server_sock);
        close(client_sock);
        exit(1);
    } else {
        printf("RECEIVED = %s\n", buf);
    }

    /* Send data back to the connected socket */
    memset(buf, 0, 256);
    strcpy(buf, DATA0);
    printf("Sending data...\n");
    rc = send(client_sock, buf, strlen(buf), 0);
    if (rc == -1) {
        printf("SEND ERROR: %d", sock_errno());
        close(server_sock);
        close(client_sock);
        exit(1);
    } else {
        printf("Data sent!\n");
    }

    /* Close the sockets and exit */
    close(server_sock);
    close(client_sock);

    return 0;
}
