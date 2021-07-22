#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG_PRINT

#ifdef DEBUG_PRINT
#define DBG_PRINT	printf
#else
#define DBG_PRINT
#endif

/*
 * 一条记录的格式
 */
typedef struct {
	int index;
	int year;
	int month;
	int date;
	int hh;
	int mm;
	int ss;
}Record_t;

/*
 * 每一秒中向log.txt中追加一条记录
 * 测试 tail -f log.txt
 */
int main(int argc, char *argv[])
{
	int isNotEmptyFile = 0;
	char tmpBuf[23] = {0};
	char *strIndex;
	int iIndex = 0;
	Record_t r;
	struct stat s;
	char buf[1024];
	struct tm *ptm;
	time_t t;

	/*
	 * 判断日志文件是否存在
	 * 不存在日志文件就新建
	 * 存在则读取最后一条记录继续添加记录
	 */
	FILE *fp = fopen("./log.txt", "a+");
	if (NULL == fp)
	{
		printf("fopen log file error\n");
		return -1;
	}

	/*
	 * 读入最后一条记录,读到最后一行为止
	 */
	while (fgets(buf, sizeof(Record_t), fp) != 0)
	{
		/*
		 * 读到最后一行才处理最后一行数据
		 * 如果是空文件就读不到最后一行
		 */
		isNotEmptyFile = 1;
	}

	if (isNotEmptyFile)
	{
		/* 处理最后一行记录 */
		strcpy(tmpBuf, buf);
		tmpBuf[22] = '\0';
		DBG_PRINT("tmpBuf = %s, buf = %s\n", tmpBuf, buf);
		strIndex = strtok(tmpBuf, " ");
		iIndex = atoi(strIndex);
		DBG_PRINT("index = %s[%d], %s\n", strIndex, iIndex, buf);
	}

	/*
	 * 主要处理文件读写位置
	 * 所以先关闭在打开
	 */
	fclose(fp);

	fp = fopen("./log.txt", "a+");
	if (NULL == fp)
	{
		printf("fopen log file error\n");
		return -1;
	}

	while (1)
	{
		/*
		 * 写一条记录
		 */
		t = time(NULL);
		ptm = localtime(&t);
		r.index = ++iIndex;
		r.year = ptm->tm_year + 1900;
		r.month = ptm->tm_mon + 1;
		r.date = ptm->tm_mday;
		r.hh = ptm->tm_hour;
		r.mm = ptm->tm_min;
		r.ss = ptm->tm_sec;

		sprintf(buf, "%d %4d-%02d-%02d %02d:%02d:%02d\n", r.index, r.year, r.month, r.date, r.hh, r.mm, r.ss);
		DBG_PRINT("buf = %s\n", buf);
		fputs(buf, fp);
		fflush(NULL);
		sleep(1);
		stat("./log.txt", &s);
		DBG_PRINT("Record_t = %ld, total size = %ld, %ld\n", sizeof(Record_t), s.st_size, s.st_size / 22);
	}

	fclose(fp);

	return 0;
}
