#!/usr/bin/env  bash

# man strftime
# strftime: string fromat date and time
# 格式如: %(strftime format)T

#printf "%(%u_%a)T"
#printf "%(%d)T"
#printf "%(%m)T"

function mylog {
	# define a local variable
	local today

	# init the variable via printf
	printf -v today "%(%u-%a)T"
	echo $today

	# what date is tomorrow
	local tom # init via date
	tom=$(date -d 'tomorrow' '+%u-%a')
	echo $tom
}

mylog
