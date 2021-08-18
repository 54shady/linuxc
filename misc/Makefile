TOP_DIR ?=..
CC=gcc
COMPILE.c=$(CC) $(CFLAGS) $(CPPFLAGS) -c
LINK.c=$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
CFLAGS= -Wall
INCLUDES=$(TOP_DIR)/include

PROGS =	cmd_action containerof cpmem daemon envi \
		findnumber findsubstr cpuspd getpwd initdarr \
		inotify inotifyep jmp jmp_ex localva strdup parr pbrb \
		recd recu reverstr revert180 scan_cal \
		bigend socketp stack stdouterr structnop swapno \
		threadsync ttyname tdas idx2str pc spa epl inotify_example \
		dnotify_test percent_star_d toggle_value mmap_test1 mmap_test2 \
		mmap_test3w mmap_test3r fsk_demo incomptest selfident \
		prot_fun glob_example colorprint ghbn ghent sep_test1 sep_test2 \
		zero_array posrw proc_ctl cping nsstest concat v2f multd ab str2num

all:	$(PROGS)

str2num : str2num.o
	$(CC) $(CFLAGS) -o $@ $^

ab : ab.o
	$(CC) $(CFLAGS) -o $@ $^

multd : multi_level_debug_info.o
	$(CC) $(CFLAGS) -o $@ $^

ppo : escapecode_progress_percent_oneline.o
	$(CC) $(CFLAGS) -o $@ $^

v2f : var2file.o
	$(CC) $(CFLAGS) -o $@ $^

concat : macro_strcat.o
	$(CC) $(CFLAGS) -o $@ $^

nsstest : nsstest.o
	$(CC) $(CFLAGS) -o $@ $^

cping : cping.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

proc_ctl : proc_ctl.o
	$(CC) $(CFLAGS) -o $@ $^

posrw : pos_rw.o
	$(CC) $(CFLAGS) -o $@ $^

zero_array : dynamic_or_zero_array.o
	$(CC) $(CFLAGS) -o $@ $^

sep_test1 : strsep.o
	$(CC) $(CFLAGS) -o $@ $^

sep_test2 : strtok.o
	$(CC) $(CFLAGS) -o $@ $^

ghbn : get_hostbyname.o
	$(CC) $(CFLAGS) -o $@ $^

ghent : get_hostent.o
	$(CC) $(CFLAGS) -o $@ $^

colorprint : escapecode_ansi_color.o
	$(CC) $(CFLAGS) -o $@ $^

glob_example : glob_example.o
	$(CC) $(CFLAGS) -o $@ $^

prot_fun : protect_function.o
	$(CC) $(CFLAGS) -o $@ $^

selfident: selfident.o
	$(CC) $(CFLAGS) -o $@ $^

fsk_demo: fseek_example.o
	$(CC) $(CFLAGS) -o $@ $^

mmap_test3w: mmap3_w.o
	$(CC) $(CFLAGS) -o $@ $^

mmap_test3r: mmap3_r.o
	$(CC) $(CFLAGS) -o $@ $^

mmap_test2: mmap2.o
	$(CC) $(CFLAGS) -o $@ $^

mmap_test1: mmap1.c
	$(CC) -I$(INCLUDES) $(CFLAGS) -o $@ $^

toggle_value: toggle.o
	$(CC) $(CFLAGS) -o $@ $^

percent_star_d: percent_star_d.o
	$(CC) $(CFLAGS) -o $@ $^

dnotify_test: dnotify_test.o
	$(CC) $(CFLAGS) -o $@ $^

spa: struct_point_array.o
	$(CC) $(CFLAGS) -o $@ $^

pc:	pc.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

idx2str:	use_index_find_string.o
	$(CC) $(CFLAGS) -o $@ $^

tdas:	two_dimension_array.o
	$(CC) $(CFLAGS) -o $@ $^

ttyname:	ttyname.o
	$(CC) $(CFLAGS) -o $@ $^

threadsync:	thread_sync.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

swapno:	swap_numbers.o
	$(CC) $(CFLAGS) -o $@ $^

structnop:	struct_without_pack.o
	$(CC) $(CFLAGS) -o $@ $^

stdouterr:	stdout_stderr.o
	$(CC) $(CFLAGS) -o $@ $^

stack:	stack.o
	$(CC) $(CFLAGS) -o $@ $^

socketp:	socketpair.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

bigend:	set_big_endless.o
	$(CC) $(CFLAGS) -o $@ $^

scan_cal:	scan_calculator.o
	$(CC) $(CFLAGS) -o $@ $^

revert180:	revert180.o
	$(CC) $(CFLAGS) -o $@ $^

reverstr:	revers_string.o
	$(CC) $(CFLAGS) -o $@ $^

recu:	recursion.o
	$(CC) $(CFLAGS) -o $@ $^

recd:	record.o
	$(CC) $(CFLAGS) -o $@ $^

pbrb:	print_binary_rever_bit.o
	$(CC) $(CFLAGS) -o $@ $^

parr:	point_to_array.o
	$(CC) $(CFLAGS) -o $@ $^

strdup:	mystrdup.o
	$(CC) $(CFLAGS) -o $@ $^

# no warn for no used variable
# why need dot c file here
localva:	local_variable.c
	$(CC) -o $@ $^

jmp:	jmp.o
	$(CC) $(CFLAGS) -o $@ $^

jmp_ex:	jmp_example.o
	$(CC) $(CFLAGS) -o $@ $^

epl:	epoll.o
	$(CC) $(CFLAGS) -o $@ $^

inotifyep:	inotify_epoll.o
	$(CC) $(CFLAGS) -o $@ $^

inotify:	inotify.o
	$(CC) $(CFLAGS) -o $@ $^

initdarr:	init_array.o
	$(CC) $(CFLAGS) -o $@ $^

gopt:	getopt.o
	$(CC) $(CFLAGS) -o $@ $^

gopt_long:	getopt_long.o
	$(CC) $(CFLAGS) -o $@ $^

cmd_action:	cmd_and_action.o
	$(CC) $(CFLAGS) -o $@ $^

containerof :	container_of.o
	$(CC) $(CFLAGS) -o $@ $^

cpmem :	copy_struct_member.o
	$(CC) $(CFLAGS) -o $@ $^

envi :	environ.o
	$(CC) $(CFLAGS) -o $@ $^

daemon :	daemon.o
	$(CC) $(CFLAGS) -o $@ $^

findnumber :	find_number_in_string.o
	$(CC) $(CFLAGS) -o $@ $^

findsubstr :	find_sub_str.o
	$(CC) $(CFLAGS) -o $@ $^

cpuspd :	get_cpu_speed_from_cpuinfo.o
	$(CC) $(CFLAGS) -o $@ $^

getpwd :	get_exe_path.o
	$(CC) $(CFLAGS) -o $@ $^

inotify_example :	inotify_example.o
	$(CC) $(CFLAGS) -o $@ $^

# gcov test demo
incomptest : incomptest.c
	$(CC) $(CFLAGS) -o $@ $^ -ftest-coverage -fprofile-arcs

clean:
	rm -f $(PROGS) *.o a.out log.txt recfile
