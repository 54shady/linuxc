#!/bin/bash

if [ "$0" == "b.bash" ];then
	tail -n +$[ `grep -n '^BINARY' $0 | cut -d ':' -f 1` + 1 ] $0 | base64 -d > a2.out
	chmod +x a2.out
	./a2.out
	echo $?
	exit
fi

cat "$0" > b.bash
echo "BINARY" >> b.bash
cat > a.c << EOF
#include <stdio.h>
int     main(){
printf("%s %d\n", __FUNCTION__, __LINE__);
        return 12;
}
EOF
gcc a.c
base64 a.out >> b.bash
