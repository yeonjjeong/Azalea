#include <sys/types.h>
#include <sys/lock.h>

#include "systemcall.h"
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

struct sigaction *installed_sigactions[MAX_SIGNUM];


/*
 * @brief examine and change a signal action
 * @param signum - signal 
 * @param act - the new action for signal signum
 * @param oldact - the previous action
 * @return success - (0), fail - (-1)
 */
int sys_rt_sigaction(int signum, struct sigaction *act, struct sigaction *oldact) 
{
  if(oldact)
    oldact = installed_sigactions[signum];

  if(act) {
    signal_handler_t sa = act->sa_handler;
    sys_signal(sa);
    installed_sigactions[signum] = act;
  }

  return 0;
}
