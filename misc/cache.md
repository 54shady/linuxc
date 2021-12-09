# Cache or Buffer

- cache 针对的是读(读磁盘时会将磁盘内容cache到内存中)
- buffer 针对的是写(写磁盘时会将要写入磁盘的数据buffer到内存中)

通过free命令看cache/buff情况

	free -h

手动清除内存中的cache/buff

	sync && echo 1 > /proc/sys/vm/drop_caches

下面命令手动将指定文件(800M大小)放到内存中

	dd if=/dev/zero of=file-800M bs=1024 count=819200
	mount -t ramfs ramfs /mnt/
	sync && echo 1 > /proc/sys/vm/drop_caches && free -h
	cp file-800M /mnt
	sync && echo 1 > /proc/sys/vm/drop_caches && free -h

使用vmtouch来将指定文件放到内存

	vmtouch -t file-800M
	或者
	vmtouch -l -t file-800M

使用vmtouch来将指定文件移出内存

	vmtouch -e file-800M
