# usage:
# 1. create a build dir via `meson build_dir'
# 2. compile via `sudo meson compile -C build_dir
#
# meson build
# sudo meson compile -C build
project('socket-test', 'c')
thread_dep = dependency('threads')

src = ['udp_client.c', 'wrap.c']
executable('udp-cli', src)
executable('udp-srv', 'udp_server.c', 'wrap.c')

executable('io-fcntl-srv', 'echo_tcp_server_fcntl.c', 'vector_fd.c', dependencies : thread_dep)
executable('io-client', 'echo_tcp_client_fcntl.c')
executable('io-select-srv', 'echo_tcp_server_select.c', 'vector_fd.c', dependencies : thread_dep)

