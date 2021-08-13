#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*
 * http://gcc.gnu.org/onlinedocs/gcc/Zero-Length.html
 *
 * 零长度数组和可变长数组
 *
 * data地址紧接在结构内部内
 * buff_st_1 or buff_st_2
 * |data_len |
 * -----------
 * | data[0] |
 * | data[1] |
 * | data[2] |
 * | ...     |
 *
 *
 * 而结构提里指针的地址不在结构提内部
 * 指向的是实际的数据位置
 *
 * buff_st_3
 * |data_len|
 * ----------
 * |data    |		[data[0], data[1], data[2]...]
 *   |              ^
 *   |              |
 *   |______________|
 *
 *
 *   http://edusagar.com/articles/view/19/zero-length-array-in-C
 *   在网络编程中,假设需要将结构体通过网络传递
 *   由于可变长结构体数据部分地址和结构体首地址
 *   相差一个偏移地址,所以可以很容易的进行按字节拷贝的方式传递数据
 *
 *   而指针的形式由于结构体地址和数据端地址不再同一片内存上
 *   无法按照字节拷贝的方式拷贝数据
 */
typedef struct
{
	int data_len;
	char data[0]; /* zero-length array */
} buff_st_1;

typedef struct
{
	int data_len;
	char data[]; /* flexible length array */
} buff_st_2;

typedef struct
{
	int data_len;
	char *data;
} buff_st_3;

typedef struct
{
	int age;
	char name[20];
} PersonInfo;

#if 0
void print_info(char *info)
{
	PersonInfo *p = (PersonInfo *)info;
	printf("name[%s]: age[%d]\n", p->name, p->age);
}
#else
void print_info(void *info)
{
	/* In C, no need to explicit do the cast */
	PersonInfo *p = info;
	printf("name[%s]: age[%d]\n", p->name, p->age);
}
#endif

int main(int argc, char *argv[])
{
	/* Basic infomation */
	printf("sizeof(buff_st_1)=%ld\n", sizeof(buff_st_1));
	printf("sizeof(buff_st_2)=%ld\n", sizeof(buff_st_2));
	printf("sizeof(buff_st_3)=%ld\n", sizeof(buff_st_3));

	buff_st_1 buff1;
	buff_st_2 buff2;
	buff_st_3 buff3;

	printf("buff1 address:%p,buff1.data_len address:%p,buff1.data address:%p\n",
			&buff1, &(buff1.data_len), buff1.data);

	printf("buff2 address:%p,buff2.data_len address:%p,buff2.data address:%p\n",
			&buff2, &(buff2.data_len), buff2.data);

	printf("buff3 address:%p,buff3.data_len address:%p,buff3.data address:%p\n",
			&buff3, &(buff3.data_len), buff3.data);

	/* Usage : example */
	PersonInfo tom;
	tom.age = 21;
	strcpy(tom.name, "Tom");

	/* buff_st_1 */
	buff_st_1 *pbuff1;
	pbuff1 = malloc(sizeof(buff_st_1) + sizeof(PersonInfo));
	if (NULL == pbuff1)
		perror("malloc error");

	pbuff1->data_len = sizeof(PersonInfo);

	/* 将格式化数据(结构体)转成流式数据(char *) */
	memcpy(pbuff1->data, &tom, pbuff1->data_len);

	print_info(pbuff1->data);
	free(pbuff1);

	/* buff_st_2 */
	buff_st_2 *pbuff2;
	pbuff2 = malloc(sizeof(buff_st_1) + sizeof(PersonInfo));
	if (NULL == pbuff2)
		perror("malloc error");

	pbuff2->data_len = sizeof(PersonInfo);
	memcpy(pbuff2->data, &tom, pbuff2->data_len);

	print_info(pbuff2->data);
	free(pbuff2);

	/* buff_st_3 */
	buff_st_3 *pbuff3;
	pbuff3 = malloc(sizeof(buff_st_3));
	if (NULL == pbuff3)
		perror("malloc error");

	pbuff3->data_len = sizeof(PersonInfo);
	pbuff3->data = malloc(pbuff3->data_len);
	if (NULL == pbuff3->data)
		perror("malloc error");

	memcpy(pbuff3->data, &tom, pbuff3->data_len);
	print_info(pbuff3->data);
	free(pbuff3->data);
	free(pbuff3);

	return 0;
}
