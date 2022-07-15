# usage:
# 1. create a build dir via `meson build_dir'
# 2. compile via `sudo meson compile -C build_dir
# 3. test via `meson test -C build_dir
#
# meson build
# sudo meson compile -C build
# sudo meson test -C build
project('linuxcmisc', 'c')
thread_dep = dependency('threads')

exe = executable('nsstest', 'nsstest.c')
# run `sudo meson test -C build' to test the executable file can run or not
test('simple test', exe)

executable('cping', 'cping.c', dependencies : thread_dep)
