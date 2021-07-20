#!/bin/bash

while read myarg
do
	# myargs should not be empyt string
    if [ ! ""x = ${myarg}x ]
    then
        echo "version111:"$myarg
    fi

    if [ ! -z ${myarg} ]
    then
        echo "version222:"$myarg
    fi
done < myargs.txt
