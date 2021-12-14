project('gdbdemo', 'c')
thread_dep = dependency('threads')
add_global_arguments('-fno-omit-frame-pointer', language : 'c')

executable('gout1', 'test1.c')
executable('gout2', 'test2.c', dependencies : thread_dep)
executable('gout3', 'test3.c', dependencies : thread_dep,
	c_args : '-DBUG_FIX')

src = ['main.c', 'foo.c', 'bar.c']
executable('gout4', src, dependencies : thread_dep)

executable('gout5', 'set_var_return.c')
executable('gout6', 'convenience.c')
executable('gout7', 'variables.c')
executable('gout8', 'astall.c', dependencies : thread_dep)
executable('gout9', 'multi_thread.c', dependencies : thread_dep)
executable('gouta', 'signal_test.c', dependencies : thread_dep)
executable('goutb', 'recursion.c')
executable('goutc', 'multi_process.c', dependencies : thread_dep)
executable('goute', 'struct_display.c')
executable('goutf', 'call_function.c')
executable('goutg', 'stack_smashing.c')
