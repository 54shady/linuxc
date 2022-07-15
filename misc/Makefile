TOP_DIR ?=..
INCLUDES=$(TOP_DIR)/include
CFLAGS= -Wall -I$(INCLUDES)

# 生成可执行文件和c文件同名的话无需写编译规则
PROGS = ab \
		cmd_and_action \
		copy_struct_member \
		cping \
		daemon \
		debug_message \
		dnotify_test \
		dynamic_or_zero_array \
		echo \
		environ \
		epoll \
		escapecode_ansi_color \
		escapecode_progress_percent_oneline \
		exit_code \
		find_number_in_string \
		find_sub_str \
		force_input_zeroone_only \
		fseek_example \
		get_cpu_speed_from_cpuinfo \
		get_exe_path \
		get_hostbyname \
		get_hostent \
		glob_example \
		init_array \
		inotify \
		inotify_epoll \
		inotify_example \
		jmp \
		jmp_example \
		macro_strcat \
		mmap2 \
		mmap_test1 \
		mmap_test3w \
		mmap_test3r \
		multi_level_debug_info \
		mystrdup \
		nsstest \
		pc \
		percent_star_d \
		point2array \
		point_to_array \
		pos_rw \
		print_binary_rever_bit \
		printf_order \
		proc_ctl \
		protect_function \
		record \
		recursion \
		revers_string \
		revert180 \
		scan_calculator \
		selfident \
		sep_test1 \
		sep_test2 \
		set_big_endless \
		socketpair \
		stack \
		stdout_stderr \
		str2num \
		struct_point_array \
		struct_without_pack \
		swap_numbers \
		thread_sync \
		toggle \
		ttyname \
		two_dimension_array \
		typedef_struct_array \
		use_index_find_string \
		virtual_keyboard \
		var2file \
		hack_scanf \
		container_of

# compile manually
PROGS+= localva incomptest

all:	$(PROGS)

# no warn for no used variable
# why need dot c file here
localva:    local_variable.c
	$(CC) -o $@ $^

# gcov test demo
incomptest : incomptest.c
	$(CC) $(CFLAGS) -o $@ $^ -ftest-coverage -fprofile-arcs

clean:
	rm -f $(PROGS) *.o a.out log.txt recfile
