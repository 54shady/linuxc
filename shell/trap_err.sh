#! /bin/bash

err_report()
{
	echo "Error on line $1"
	echo "caller is $(caller)"
}

# shell 中使用trap
# trap 'commands' signal_list
# 出现signal_list中的信号后将执行commands
trap 'err_report $LINENO' ERR

# 出错的话就将产生一个ERR
echo hello | grep foo
