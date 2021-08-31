#!/bin/bash

# usage
# using the script as a crontab script
# nohup bash ./crontab.sh &

# one hour
one_hour=$((1*60*60))

function do_someting()
{
	notify-send "Take a break ;-)"
}

while true;
do
	do_someting
	sleep $one_hour
done
