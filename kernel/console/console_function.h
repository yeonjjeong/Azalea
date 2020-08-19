#ifndef __CONSOLE_FUNCTION_H__
#define __CONSOLE_FUNCTION_H__

#include <sys/types.h>
#include <sys/times.h>
#include <sys/uio.h>

#define	CONSOLE_PRINT	1
#define	CONSOLE_WRITE	2
#define	CONSOLE_GETCH	3
#define CONSOLE_TIMES   4
#define CONSOLE_WRITEV  5
#define	CONSOLE_EXIT	(-9)

int cs_boot_msg_print(int yloc);
int cs_printf(const char *parameter, ...);

ssize_t cs_write(int fd, void *buf, size_t count);
int cs_puts(void *buf);
int cs_exit(void);

clock_t cs_times(struct tms *buf);
ssize_t cs_writev(int fd, struct iovec *iov_param, int iovcnt_param);

#endif //__CONSOLE_FUNCTION_H__
