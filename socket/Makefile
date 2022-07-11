CC=gcc
COMPILE.c=$(CC) $(CFLAGS) $(CPPFLAGS) -c
LINK.c=$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
CFLAGS= -Wall -DUSE_RW

PROGS =	tcp_server_wrap tcp_client_wrap tcp_server tcp_client \
	tcp_client_inter tcp_server_inter udp_server udp_client \
	getinfo_svr getinfo_cli io_fcntl_server io_client io_select_server \
	udp_server_broadcast udp_client_broadcast tcp_server_msg tcp_client_msg \
	uds-client-stream uds-server-stream \
	uds-client-datagram uds-server-datagram \

all: $(PROGS)

# unix domain socket example
# stream
uds-client-stream : unix-domain-socket-client-stream.o
	$(CC) $(CFLAGS) -o $@ $^

uds-server-stream : unix-domain-socket-server-stream.o
	$(CC) $(CFLAGS) -o $@ $^

# datagram
uds-client-datagram : unix-domain-socket-client-datagram.o
	$(CC) $(CFLAGS) -o $@ $^

uds-server-datagram : unix-domain-socket-server-datagram.o
	$(CC) $(CFLAGS) -o $@ $^

# tcp io multiplexing example
io_fcntl_server : echo_tcp_server_fcntl.o vector_fd.o
		$(CC) $(CFLAGS) -o $@ $^ -lpthread

io_client : echo_tcp_client_fcntl.o
		$(CC) $(CFLAGS) -o $@ $^

io_select_server : echo_tcp_server_select.o vector_fd.o
		$(CC) $(CFLAGS) -o $@ $^ -lpthread
# tcp io multiplexing example

getinfo_svr: getaddrinfo_server.o
		$(CC) -o $@ $^

getinfo_cli: getaddrinfo_client.o
		$(CC) -o $@ $^

tcp_server_msg:	tcp_server_msg.o msg.o
		$(CC) $(CFLAGS) -o $@ $^

tcp_client_msg:	tcp_client_msg.o msg.o
		$(CC) $(CFLAGS) -o $@ $^

tcp_server:	tcp_server.o
		$(CC) $(CFLAGS) -o $@ $^

tcp_client:	tcp_client.o
		$(CC) $(CFLAGS) -o $@ $^

tcp_server_wrap:	tcp_server_wrap.o wrap.o
		$(CC) $(CFLAGS) -o $@ $^

tcp_client_wrap:	tcp_client_wrap.o wrap.o
		$(CC) $(CFLAGS) -o $@ $^

tcp_client_inter:	tcp_client_interactive.o wrap.o
		$(CC) $(CFLAGS) -o $@ $^

tcp_server_inter:	tcp_server_interactive.o wrap.o
		$(CC) $(CFLAGS) -o $@ $^

udp_server_broadcast:	udp_server_broadcast.o wrap.o
		$(CC) -o $@ $^

udp_client_broadcast:	udp_client_broadcast.o wrap.o
		$(CC) -o $@ $^

udp_server:	udp_server.o wrap.o
		$(CC) -o $@ $^

udp_client:	udp_client.o wrap.o
		$(CC) -o $@ $^

clean:
	rm -f $(PROGS) *.o
