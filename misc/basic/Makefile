all:hello

# 1. 预处理(删除注释,展开宏, 输出物仍然还是文本文件)
hello.i:hello.c
	gcc -E $< -o $@

# 2. 编译(检测语法错误,输出物是汇编文件)
hello.s:hello.i
	gcc -m64 -S $< -o $@

# 3. 汇编(输出物是包含机器码的elf目标文件)
# readelf -S hello.o
#
# 查看输出物里的符号表
# objdump -t hello.o 或 readelf -s hello.o
hello.o:hello.s
	gcc -m64 -c $< -o $@

# 4. 链接(输出物是elf可执行文件)
hello:hello.o
	gcc -static -m64 $< -o $@

clean:
	rm -rf hello.i hello.s hello.o hello
