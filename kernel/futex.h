#ifndef __FUTEX_H__
#define __FUTEX_H__

#include <time.h>

#include "thread.h"

#define FUTEX_WAIT 0
#define FUTEX_WAKE 1

#define FUTEX_PRIVATE_FLAG	128
#define FUTEX_CLOCK_REALTIME	256
#define FUTEX_CMD_MASK		~(FUTEX_PRIVATE_FLAG | FUTEX_CLOCK_REALTIME)

#define MAX_TASKS		(CONFIG_NUM_THREAD + MAX_PROCESSOR_COUNT)	

typedef struct __futex_q {
	int* futex;
	//TO_DO: waiters wille be changed dl_list
	int waiters[MAX_TASKS];
	struct __futex_q* next;
	struct __futex_q* prev;
} futex_q;

int sys_futex(int *uaddr, int op, int val, const struct timespec *timeout, int *uaddr2, int val3);

#endif  /* __FUTEX_H__ */
