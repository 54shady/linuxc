# android编译错误

在编译android源码是遇到一个错误信息如下

	out/host/linux-x86/bin/mkbootfs: error while loading shared libraries: libcutils.so: cannot open shared object file: No such file or directory
	out/host/linux-x86/bin/minigzip: error while loading shared libraries: libc++.so: cannot open shared object file: No such file or directory

上面错误说明库文件libcutils.so, libc++.so在编译时未被找到

可以使用ldd查看需要的库文件(not found的库即是)

	$ ldd out/host/linux-x86/bin/mkbootfs
	linux-vdso.so.1 =>  (0x00007ffcd3393000)
	libcutils.so => not found
	libc++.so => not found
	libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007fa2bc3b5000)
	libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007fa2bc0ab000)
	libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007fa2bbe95000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fa2bbacb000)
	/lib64/ld-linux-x86-64.so.2 (0x00005638a7da0000)

因为编译主机是64位的,所以需要在下面目录下有上面两个库文件

	out/host/linux-x86/lib64/

修改后ldd查看的结果如下

	$ ldd out/host/linux-x86/bin/mkbootfs
	linux-vdso.so.1 =>  (0x00007ffccb952000)
	libcutils.so => /root/out/host/linux-x86/bin/../lib64/libcutils.so (0x00007f1c25fc2000)
	libc++.so => /root/out/host/linux-x86/bin/../lib64/libc++.so (0x00007f1c25ec6000)
	libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f1c25c98000)
	libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f1c2598e000)
	libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f1c25778000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f1c253ae000)
	liblog.so => /root/out/host/linux-x86/bin/../lib64/../lib64/liblog.so (0x00007f1c253a6000)
	librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007f1c2519e000)
	libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f1c24f99000)
	/lib64/ld-linux-x86-64.so.2 (0x000055646972f000)
