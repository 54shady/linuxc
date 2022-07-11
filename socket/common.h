#ifndef __COMMON_H__
#define __COMMON_H__

#undef sock_errno
#define sock_errno() errno

/* domain socket file */
#define SERVER_PATH "unix_domain_sock.server"
#define CLIENT_PATH "unix_domain_sock.client"

#define DATA0 "Hello from server"
#define DATA1 "Hello from client"

#endif
