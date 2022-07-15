#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/uinput.h>
#include <linux/input.h>
#include <termios.h>

#define KEY_CUSTOM_UP 0x20
#define KEY_CUSTOM_DOWN 0x30

/* for input */
static struct termios initial_settings, new_settings;
static int peek_character = -1;

/* uinput */
static struct uinput_user_dev uinput_dev;
static int uinput_fd;

struct keycode_name {
	int key_ascii; /* 按键的ascii值 */
	int key_code; /* 按键编码 */
	const char name[30]; /* 按键名字 */
};

/* key map ain't covery all */
struct keycode_name keys[104] = {
	/* ascii, keycode, name */
	{0, 1,    	"ESC"},
	{0, 59,   	"F1"},
	{0, 60,   	"F2"},
	{0, 61,   	"F3"},
	{0, 62,   	"F4"},
	{0, 63,   	"F5"},
	{0, 64,   	"F6"},
	{0, 65,   	"F7"},
	{0, 66,   	"F8"},
	{0, 67,   	"F9"},
	{0, 68,   	"F10"},
	{0, 87,   	"F11"},
	{0, 88,   	"F12"},
	{0, 41,   	"` ~"},
	{49, 2,    	"1 !"},
	{50, 3,    	"2 @"},
	{51, 4,    	"3 #"},
	{52, 5,    	"4 $"},
	{53, 6,    	"5 %"},
	{54, 7,    	"6 ^"},
	{55, 8,    	"7 &"},
	{56, 9,    	"8 *"},
	{57, 10,   	"9 ("},
	{48, 11,   	"0 )"},
	{45, 12,   	"- _"},
	{0, 13,   	"+ ="},
	{8, 14,   	"Backspace(退格键)"},
	{9, 15,   	"Tab"},
	{81, 16,   	"q Q"},
	{87, 17,   	"w W"},
	{69, 18,   	"e E"},
	{82, 19,   	"r R"},
	{84, 20,   	"t T"},
	{89, 21,   	"Y y"},
	{85, 22,   	"u U"},
	{73, 23,   	"i I"},
	{79, 24,   	"o O"},
	{80, 25,   	"p P"},
	{0, 26,   	"[ {"},
	{0, 27,   	"] }"},
	{13, 28,   	"Enter(回车键)"},
	{0, 58,   	"CapsLock"},
	{65, 30,   	"a A"},
	{83, 31,   	"s S"},
	{68, 32,   	"d D"},
	{70, 33,   	"f F"},
	{71, 34,   	"g G"},
	{72, 35,   	"h H"},
	{74, 36,   	"j J"},
	{75, 37,   	"k K"},
	{76, 38,   	"l L"},
	{0, 39,   	"; :"},
	{0, 40,   	"' \""},
	{0, 43,   	"\\ |"},
	{0, 42,   	"Left_shift(左上档键)"},
	{90, 44,   	"z Z"},
	{88, 45,   	"x X"},
	{67, 46,   	"c C"},
	{86, 47,   	"v V"},
	{66, 48,   	"b B"},
	{78, 49,   	"n N"},
	{77, 50,   	"m M"},
	{0, 51,   	", <"},
	{46, 52,   	". >"},
	{47, 53,   	"/ ?"},
	{0, 54,   	"Right_shift(右上档键)"},
	{0, 29,   	"Left_ctrl(左Ctrl键)"},
	{0, 125,   	"左Super键(windows键)"},
	{0, 56,   	"Left_alt(左alt键)"},
	{32, 57,   	"space(空格键)"},
	{0, 100,   	"Right_alt(右alt键)"},
	{0, 126,  	"右Super键(windows键)"},
	{0, 127,  	"菜单键"},
	{0, 97,   	"Right_ctrl(右Ctrl键)"},
	{0, 99,   	"Print Screen键"},
	{0, 70,   	"ScrollLock键"},
	{0, 119,  	"PauseBreak键"},
	{0, 110,  	"Insert"},
	{0, 102,  	"Home"},
	{0, 104,  	"PageUp"},
	{0, 111,  	"Delete"},
	{0, 107,  	"End"},
	{0, 109,  	"PageDown"},
	{0, 103,  	"方向键上"},
	{0, 108,  	"方向键下"},
	{0, 105,  	"方向键左"},
	{0, 106,  	"方向键右"},
	{0, 69,   	"NumLock"},
	{0, 98,   	"/　(小键盘)"},
	{0, 55,   	"*　(小键盘)"},
	{0, 74,   	"-　(小键盘)"},
	{0, 78,   	"+　(小键盘)"},
	{0, 96,   	"Enter(小键盘)"},
	{0, 83,   	". (小键盘)"},
	{0, 82,   	"0 (小键盘)"},
	{0, 79,   	"1 (小键盘)"},
	{0, 80,   	"2 (小键盘)"},
	{0, 81,   	"3 (小键盘)"},
	{0, 75,   	"4 (小键盘)"},
	{0, 76,   	"5 (小键盘)"},
	{0, 77,   	"6 (小键盘)"},
	{0, 71,   	"7 (小键盘)"},
	{0, 72,   	"8(小键盘)"},
	{0, 73,   	"9(小键盘)"}
};

void init_keyboard()
{
	tcgetattr(0, &initial_settings);
#if 0
	termios_p->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
			| INLCR | IGNCR | ICRNL | IXON);
	termios_p->c_oflag &= ~OPOST;
	termios_p->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	termios_p->c_cflag &= ~(CSIZE | PARENB);
	termios_p->c_cflag |= CS8;
#endif
	new_settings = initial_settings;

	//	new_settings.c_iflag |= IGNBRK;
	new_settings.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
			| INLCR | IGNCR | ICRNL | IXON);

	new_settings.c_oflag |= ONLRET;
	new_settings.c_oflag |= ONOCR;

	new_settings.c_lflag |= ICANON;
	new_settings.c_lflag |= ~ECHO;
	new_settings.c_lflag &= ISIG;
	new_settings.c_cc[VMIN] = 1;
	new_settings.c_cc[VTIME] = 0;

	tcsetattr(0, TCSANOW, &new_settings);
}

void close_keyboard()
{
	tcsetattr(0, TCSANOW, &initial_settings);
}

int kbhit(void)
{
	unsigned char ch;
	int nread;

	if (peek_character != -1)
		return 1;

	new_settings.c_cc[VMIN]=0;
	tcsetattr(0, TCSANOW, &new_settings);
	nread = read(0, &ch, 1);
	new_settings.c_cc[VMIN]=1;
	tcsetattr(0, TCSANOW, &new_settings);
	if (nread == 1)
	{
		peek_character = ch;
		return 1;
	}

	return 0;
}

int readch(void)
{
	char ch;

	if (peek_character != -1)
	{
		ch = peek_character;
		peek_character = -1;
		return ch;
	}

	read(0, &ch, 1);

	return ch;
}

int keyascii_to_keycode(int key_ascii)
{
	int i;

	//printf("\nkey_ascii:%d\n", key_ascii);
	for (i = 0; i < 104; i++)
	{
		/* a-z */
		if (key_ascii >= 97 && key_ascii <= 122)
		{
			if (key_ascii == keys[i].key_ascii + 32)
				return keys[i].key_code;
		}
		else
		{
			if (key_ascii == keys[i].key_ascii)
			{
				//printf("kcode[%d]:%d\n", i, keys[i].key_code);
				return keys[i].key_code;
			}
		}
	}

	return -1;
}

int creat_user_uinput(void)
{
	int i;
	int ret = 0;

	/* 基于内核实现uinput */
	uinput_fd = open("/dev/uinput", O_RDWR | O_NDELAY);
	if (uinput_fd < 0) {
		printf("%s:%d\n", __func__, __LINE__);
		return -1;
	}

	/* to set uinput dev */
	memset(&uinput_dev, 0, sizeof(struct uinput_user_dev));
	snprintf(uinput_dev.name, UINPUT_MAX_NAME_SIZE, "uinput-custom-dev");
	uinput_dev.id.version = 1;
	uinput_dev.id.bustype = BUS_VIRTUAL;

	ioctl(uinput_fd, UI_SET_EVBIT, EV_SYN);
	ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
	ioctl(uinput_fd, UI_SET_EVBIT, EV_MSC);

	for (i = 0; i < 256; i++)
		ioctl(uinput_fd, UI_SET_KEYBIT, i);

	ioctl(uinput_fd, UI_SET_MSCBIT, KEY_CUSTOM_UP);
	ioctl(uinput_fd, UI_SET_MSCBIT, KEY_CUSTOM_DOWN);

	ret = write(uinput_fd, &uinput_dev, sizeof(struct uinput_user_dev));
	if (ret < 0) {
		printf("%s:%d\n", __func__, __LINE__);
		return ret;
	}

	ret = ioctl(uinput_fd, UI_DEV_CREATE);
	if (ret < 0) {
		printf("%s:%d\n", __func__, __LINE__);
		close(uinput_fd);
		return ret;
	}

	return 0;
}

int report_key(unsigned int type, unsigned int keycode, unsigned int value)
{
	struct input_event key_event;
	int ret;

	memset(&key_event, 0, sizeof(struct input_event));

	gettimeofday(&key_event.time, NULL);
	key_event.type = type;
	key_event.code = keycode;
	key_event.value = value;
	ret = write(uinput_fd, &key_event, sizeof(struct input_event));
	if (ret < 0) {
		printf("%s:%d\n", __func__, __LINE__);
		return ret;
	}

	gettimeofday(&key_event.time, NULL);
	key_event.type = EV_SYN;
	key_event.code = SYN_REPORT;
	key_event.value = 0; /* event status sync */
	ret = write(uinput_fd, &key_event, sizeof(struct input_event));
	if (ret < 0) {
		printf("%s:%d\n", __func__, __LINE__);
		return ret;
	}

	return 0;
}

void input_sync(unsigned int type, unsigned int keycode, int shift)
{
	/* shift key no press */
	if (shift == 0)
	{
		report_key(type, keycode, 1); /* Report BUTTON CLICK PRESS event */
		report_key(type, keycode, 0); /* Report BUTTON CLICK RELEASE event */
	}
	else /* press shift key */
	{
		report_key(EV_KEY, KEY_LEFTSHIFT, 1); /* Report BUTTON CLICK PRESS event */
		report_key(type, keycode, 1); /* Report BUTTON CLICK PRESS event */
		report_key(type, keycode, 0); /* Report BUTTON CLICK RELEASE event */
		report_key(EV_KEY, KEY_LEFTSHIFT, 0); /* Report BUTTON CLICK RELEASE event */
	}
}

/*
 * Virtual Keyboard(Remote Keyboard)
 * Usage:
 *		aarch64-linux-gnu-gcc virtual_keyboard.c -o virtual_keyboard
 *
 * 1. run ./virtual_keyboard as root in HOST PC console
 * 2. check TARGET terminal or hexdump -C /dev/input/event[X]
 *
 * 虚拟键盘(远程键盘):将主机上的物理键盘输入发送到开发板上
 * 来达到控制开发板的目的
 *
 * 开发主机和开发板之间通过串口连接
 * 1. 在开发主机中通过串口终端访问开发板，以root执行./virtual_keyboard程序
 *	  因为这里使用的是终端编程获取到按键的ascii
 *	  再将ascii映射为keycode
 *	  最后将keycode上报给开发板来实现的
 *
 * 2. 此时就会向开发板上报键盘按键事件，如果此时在开发板上打开一个终端
 * 即可实现使用接在主机上的物理键盘操控开发板，提升开发效率，不用两个键盘
 *
 * 参考文章:
 * https://www.runoob.com/w3cnote/c-get-keycode.html
 * https://blog.csdn.net/zhongkunjia/article/details/75142699#
 */
int main(int argc, char *argv[])
{
	int ret = 0;
	int key_ascii;
	int key_code;

	ret = creat_user_uinput();
	if (ret < 0) {
		printf("%s:%d\n", __func__, __LINE__);
		return -1;
	}

	init_keyboard();

	while (1)
	{
		kbhit();
		key_ascii = readch();
		key_code = keyascii_to_keycode(key_ascii);
		//printf("report keycode:%d\n", key_code);

		/* just simply handle A-Z */
		if (key_ascii >= 65 && key_ascii <= 90)
			input_sync(EV_KEY, key_code, 1);
		else
			input_sync(EV_KEY, key_code, 0);
	}

	close_keyboard();
	close(uinput_fd);

	return 0;
}
