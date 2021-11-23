# [bash script containing binary executable](https://stackoverflow.com/questions/18410785/bash-script-containing-binary-executable)

## 例子1:手动创建能执行二进制程序的shell脚本

先生成一个二进制程序hello

	gcc hello.c -o hello

生成一个自定义的格式文件

	echo "BINARY" > binary_in_txt

将二进制文件转成文本形式存入文本文件(这里使用base64编码)

	base64 hello >> binary_in_txt

还原文本文件中的二进制程序

	tail -n +$[ `grep -n '^BINARY' binary_in_txt | cut -d ':' -f 1` + 1 ] binary_in_txt | base64 -d > newhello
	chmod +x newhello; ./newhello

## 使用脚本自动化例子1

执行方法

	bash a.bash # 执行后生成包含二进制文件的b.bash
	bash b.bash # 执行b.bash

将二进制程序集成在[shell脚本中](./a.bash)

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
