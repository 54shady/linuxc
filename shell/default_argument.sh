#!/usr/bin/env bash

# using ${:-} format
path=${1:-/home/anonymous}
echo $path

# homepath will be /tmp if $HOME is null
# os if HOME being unset
homepath=${HOME:=/tmp}
echo $homepath

# ${VAR:=value} 执行赋值操作,同时返回运算符右侧的值
# ${VAR:-value} 没有执行赋值操作,只返回右侧的值
