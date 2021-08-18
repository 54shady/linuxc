#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * 参考链接: https://misc.flogisoft.com/bash/tip_colors_and_formatting
 * 在bash中可以用下面三个语法来表示Esc
 * \e
 * \033
 * \x1B
 *
 * echo -e "\e[31mHello World\e[0m"
 * echo -e "\033[31mHello\e[0m World"
 *
 * 其中-e选项表示进入escape模式
 *
 * Foreground color(text)
 *	echo -e "Default \e[39mDefault"
 *	echo -e "Default \e[30mBlack"
 *	echo -e "Default \e[31mRed"
 *	echo -e "Default \e[32mGreen"
 *	echo -e "Default \e[33mYellow"
 *	echo -e "Default \e[34mBlue"
 *	echo -e "Default \e[35mMagenta"
 *	echo -e "Default \e[36mCyan"
 *	echo -e "Default \e[37mLight gray"
 *	echo -e "Default \e[90mDark gray"
 *	echo -e "Default \e[91mLight red"
 *	echo -e "Default \e[92mLight green"
 *	echo -e "Default \e[93mLight yellow"
 *	echo -e "Default \e[94mLight blue"
 *	echo -e "Default \e[95mLight magenta"
 *	echo -e "Default \e[96mLight cyan"
 *	echo -e "Default \e[97mWhite"
 *
 * Background color
 *	echo -e "Default \e[49mDefault"
 *	echo -e "Default \e[40mBlack"
 *	echo -e "Default \e[41mRed"
 *	echo -e "Default \e[42mGreen"
 *	echo -e "Default \e[43mYellow"
 *	echo -e "Default \e[44mBlue"
 *	echo -e "Default \e[45mMagenta"
 *	echo -e "Default \e[46mCyan"
 *	echo -e "Default \e[47mLight gray"
 *	echo -e "Default \e[100mDark gray"
 *	echo -e "Default \e[101mLight red"
 *	echo -e "Default \e[102mLight green"
 *	echo -e "Default \e[103mLight yellow"
 *	echo -e "Default \e[104mLight blue"
 *	echo -e "Default \e[105mLight magenta"
 *	echo -e "Default \e[106mLight cyan"
 *	echo -e "Default \e[107mWhite"
 */

void exit_func(void)
{
	/* make cursor visable while exit this program */
	printf("\033[?25h");
}

int main(int argc, char *argv[])
{
	int i;

	atexit(exit_func);

	for (i = 0; i < 100; i++)
	{
		/*
		 * 使用escape code 进行编程
		 *
		 * 1. 读到\033,说明后面的字符是ANSI escape sequence,将切换到escape模式
		 * 2. 然后读取'['字符移入到CSI模式(Command Sequence Introduction)
		 *
		 * \033进入到escape mode
		 * [ 切换到CSI mode
		 * 2 读入的参数
		 * K 指定要执行的动作
		 * \r光标回到行首
		 *
		 * 字背景颜色范围:40-49
		 * 40:黑
		 * 41:深红
		 * 42:绿
		 * 43:黄色
		 * 44:蓝色
		 * 45:紫色
		 * 46:深绿
		 * 47:白色
		 *
		 * 字颜色:30-39
		 * 30:黑
		 * 31:红
		 * 32:绿
		 * 33:黄
		 * 34:蓝色
		 * 35:紫色
		 * 36:深绿
		 * 37:白色
		 *
		 * ANSI控制码的说明:
		 * 33[0m 关闭所有属性
		 * 33[1m 设置高亮度
		 * 33[4m 下划线
		 * 33[5m 闪烁
		 * 33[7m 反显
		 * 33[8m 消隐 (userful for passwords)
		 * 33[30m -- 33[37m 设置前景色
		 * 33[40m -- 33[47m 设置背景色
		 * 33[nA 光标上移n行
		 * 33[nB 光标下移n行
		 * 33[nC 光标右移n行
		 * 33[nD 光标左移n行
		 * 33[y;xH设置光标位置
		 * 33[2J 清屏
		 * 33[K 清除从光标到行尾的内容
		 * 33[s 保存光标位置
		 * 33[u 恢复光标位置
		 * 33[?25l 隐藏光标
		 * 33[?25h 显示光标
		 */
#ifdef ONE_PROGRESS
		printf("\033[2K\r");
		printf("Install1: %d%%", i);
#endif

#ifdef TWO_PROGRESS
		printf("\033[2K\r");
		printf("Install1: %d%%", i);

		/* add one more */
		printf("\n");
		printf("\033[2K\r");
		printf("Install2: %d%%", i*5);
		printf("\033[1A");
#endif

#ifdef THREE_PROGRESS
		printf("\033[2K\r");
		printf("Install1: %d%%", i);

		/* add one more */
		printf("\n");
		printf("\033[2K\r");
		printf("Install2: %d%%", i*5);

		/* add one more */
		printf("\n");
		printf("\033[2K\r");
		printf("Install3: %d%%", i+1);
		printf("\033[2A");
#endif

#define USING_XY_COORDINATE
#ifdef USING_XY_COORDINATE
		/* clear the screen first */
		printf("\033[J");

		/* XY from lefttop */
		printf("\033[1;0H");
		printf("\033[2K\r");
		printf("Install1: %d%%", i);

		printf("\033[2;0H");
		printf("\033[2K\r");
		printf("Install2: %d%%", i+1);

		printf("\033[3;0H");
		printf("\033[2K\r");
		printf("Install3: %d%%", i+2);

		/* make cursor invisable */
		printf("\033[?25l");
#endif

		fflush(stdout);
		usleep(100000);
	}
	printf("\n");

	return 0;
}
