# 数据镜像文件制作

[Linux中dd命令详解](https://blog.csdn.net/xizaihui/article/details/53307578)

## 应用实例1

先生成一个数据为0的镜像文件(data.bin)

	dd if=/dev/zero of=data.bin bs=512 count=1

查看镜像文件数据内容

	hexdump –C data.bin

修改镜像里的数据(往镜像里写数据LoaderMode)

	echo LoaderMode | dd of=data.bin

## 应用实例2

使用脚本([gen_image.sh](./gen_image.sh))制作一个misc.img内容如下

	00000000  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
	*
	00004000  62 6f 6f 74 2d 72 65 63  6f 76 65 72 79 00 00 00  |boot-recovery...|
	00004010  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
	*
	00006000  67 65 6e 74 6f 6f 2d 6c  69 6e 75 78 00 00 00 00  |gentoo-linux....|
	00006010  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
	*
	0000c000

脚本内容如下
```shell
#!/usr/bin/env bash

out=./misc.img

pad()
{
   busybox dd if=/dev/zero bs=$1 count=1
}

echo_pad()
{
   echo -n "$1" | busybox dd bs=$2 conv=sync;
}

{
	pad 16k
	echo_pad boot-recovery 8k
	echo_pad gentoo-linux 8k
	pad 16k
} > $out
```
