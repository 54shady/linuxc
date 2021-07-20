#!/bin/bash

print_usage()
{
	echo "Usage : $0 [-c <chip>] [-t <target>] [-r <rootfs>] [-h]"
}

if [ $# -lt 1 ]
then
	print_usage $0
fi

# c,t,r右边都有':'所以必须跟选项参数(optarg)
# h右边没有':'所以可以不需要跟选项参数(optarg)
while getopts "c:t:r:h" flag; do
	case $flag in
		c)
			CHIP="$OPTARG"
			echo "option c's optargs is $CHIP"
			;;
		t)
			TARGET="$OPTARG"
			echo "option t's optargs is $TARGET"
			;;
		r)
			ROOTFS_PATH="$OPTARG"
			echo "option r's optargs is $ROOTFS_PATH"
			;;
		h)
			print_usage $0
			;;
		*)
			print_usage $0
			;;
	esac
done
