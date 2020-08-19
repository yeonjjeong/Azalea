#include <sys/types.h>
#include <sys/lock.h>

#include "signal.h"

typedef void * siginfo_t;
//typedef void * sigset_t;


struct sigaction {
	union {
		void (*sa_handler)(int);
		void (*sa_sigaction)(int, siginfo_t *, void *);
	};
	sigset_t sa_mask;
	int sa_flags;
	void (*sa_restorer)(void);
};

#define MAX_SIGNUM 32

int sys_rt_sigaction(int signum, struct sigaction *act, struct sigaction *oldact);

