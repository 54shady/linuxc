#!/bin/bash

# $1 start with abcxxxxx can get right
# abc, abcdef, abcabc, abchijo ==> right
# a, b, ab ==> can't get right
if [[ $1 =~ "abc" ]]
then
	echo "right"
fi
