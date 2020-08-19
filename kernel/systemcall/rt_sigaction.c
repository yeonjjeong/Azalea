#include <sys/types.h>
#include <sys/lock.h>

#include "signal.h"

#include "rt_sigaction.h"

struct sigaction *installed_sigactions[MAX_SIGNUM];

int sys_rt_sigaction(int signum, struct sigaction *act,
		struct sigaction *oldact) {

	if(oldact)
		oldact = installed_sigactions[signum];

	if(act) {
		signal_handler_t sa = act->sa_handler;
		sys_signal(sa);
		installed_sigactions[signum] = act;
	}

	return 0;
}
