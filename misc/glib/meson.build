# usage:
# 1. create a build dir via `meson build_dir'
# 2. compile via `sudo meson compile -C build_dir
#
# meson build
# sudo meson compile -C build
project('glibtest', 'c')
glib = dependency('glib-2.0')

executable('example01', 'example01.c', dependencies : glib)
executable('example02', 'example02.c', dependencies : glib)
executable('queue-test', 'queue.c', dependencies : glib)
