#ifndef __LNXC__
#define __LNXC__

/* LNXC mean LinuxC */
#ifdef LNXC_DEBUG
#define lnxc_dbg(fmt, args...)\
	printf("DEBUG: [%s]: "fmt, __func__, ##args)
#else
#define lnxc_dbg(fmt, args...) do {} while (0)
#endif

#ifdef LNXC_INFO
#define lnxc_info(fmt, args...)\
	printf("INFO: [%s]: "fmt, __func__, ##args)
#else
#define lnxc_info(fmt, args...) do {} while (0)
#endif

#ifdef LNXC_WARN
#define lnxc_warn(fmt, args...)\
	printf("WARNING: [%s]: "fmt, __func__, ##args)
#else
#define lnxc_warn(fmt, args...) do {} while (0)
#endif

#ifdef LNXC_ERR
#define lnxc_err(fmt, args...)\
	printf("ERROR: [%s]: "fmt, __func__, ##args)
#else
#define lnxc_err(fmt, args...) do {} while (0)
#endif

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

#endif
