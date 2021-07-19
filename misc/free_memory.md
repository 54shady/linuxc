# Free Linux Memory(DropCaches)

释放前最好先sync保存下缓存内容,防止数据丢失

	sync

To free pagecache

	echo 1 > /proc/sys/vm/drop_caches

To free dentries and inodes

	echo 2 > /proc/sys/vm/drop_caches

To free pagecache, dentries and inodes

	echo 3 > /proc/sys/vm/drop_caches

因为LINUX的内核机制,一般情况下不需要特意去释放已经使用的cache.这些cache起来的内容可以增加文件以及的读写速度.

使用free命令看内存

	total   used   free     shared buffers cached
	Mem: 515588 295452 220136 0      2060   64040
	-/+ buffers/cache: 229352 286236
	Swap: 682720 112 682608

第一行用全局角度描述系统使用的内存状况

	total	总物理内存
	used	已使用内存,一般情况这个值会比较大,因为这个值包括了cache+应用程序使用的内存
	free	完全未被使用的内存
	shared	应用程序共享内存
	buffers	缓存,主要用于目录方面,inode值等(ls大目录可看到这个值增加)
	cached	缓存,用于已打开的文件

    total=used+free
    used=buffers+cached (maybe add shared also)

第二行描述应用程序的内存使用

前个值表示-buffers/cache应用程序使用的内存大小,used减去缓存值

	-buffers/cache=used-buffers-cached

后个值表示+buffers/cache所有可供应用程序使用的内存大小,free加上缓存值

	+buffers/cache=free+buffers+cached

第三行表示swap的使用

	used	已使用
	free	未使用

手动执行sync命令(sync命令运行sync子例程,如果必须停止系统,则运行sync 命令以确保文件系统的完整性
sync命令将所有未写的系统缓冲区写到磁盘中,包含已修改的inode,已延迟的块I/O 和读写映射文件)
