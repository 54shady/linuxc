#include <stdio.h>

/* Copy from uboot fastboot header file */
#define	FBT_ERR
#define FBT_WARN
#define FBT_INFO
#define FBT_DEBUG

#ifdef FBT_DEBUG
#define FBTDBG(fmt, args...)\
	printf("DEBUG: [%s]: "fmt, __func__, ##args)
#else
#define FBTDBG(fmt, args...) do {} while (0)
#endif

#ifdef FBT_INFO
#define FBTINFO(fmt, args...)\
	printf("INFO: [%s]: "fmt, __func__, ##args)
#else
#define FBTINFO(fmt, args...) do {} while (0)
#endif

#ifdef FBT_WARN
#define FBTWARN(fmt, args...)\
	printf("WARNING: [%s]: "fmt, __func__, ##args)
#else
#define FBTWARN(fmt, args...) do {} while (0)
#endif

#ifdef FBT_ERR
#define FBTERR(fmt, args...)\
	printf("ERROR: [%s]: "fmt, __func__, ##args)
#else
#define FBTERR(fmt, args...) do {} while (0)
#endif

int main(int argc, char *argv[])
{
	FBTDBG("fastboot debug\n");
	FBTINFO("fastboot info\n");
	FBTWARN("fastboot warn\n");
	FBTERR("fastboot error\n");

	return 0;
}
